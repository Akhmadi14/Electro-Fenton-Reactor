/*
 * taskManajemenPompa.c
 *
 * Task FreeRTOS untuk mengelola pompa pada reaktor Fenton.
 *
 * Aktif pada state:
 *   STATE_INISIALISASI_EKSEKUSI — pompa sampel, H2SO4, FeSO4, kalibrasi spektro
 *   STATE_FLUSHING              — pompa flushing (buang isi reaktor)
 *
 * Saat ini menggunakan simulasi dummy (pompa fisik belum terpasang).
 * Setiap fase disimulasikan dengan delay bertahap, progress diupdate
 * ke shared variable yang dibaca oleh taskKomunikasi untuk di-stream ke UI.
 *
 * Protokol JSON yang di-stream oleh taskKomunikasi berdasarkan data dari task ini:
 *   Inisialisasi ongoing : {"status":"ongoing","sampel":70,"h2so4":0,"feso4":0,"kalibrasi":0}
 *   Inisialisasi done    : {"status":"done","sampel":100,"h2so4":1,"feso4":1,"kalibrasi":1}
 *   Flushing ongoing     : {"status":"flushing","progress":70}
 *   Flushing done        : {"status":"flushing_done","progress":100}
 */

#include "PompaShared.h"
#include "Config.h"
#include "driver/ledc.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* mutexSystemState didefinisikan di main.cpp, dipakai untuk proteksi semua shared state */
extern SemaphoreHandle_t mutexSystemState;
extern void pompaExecutionHardwareInit(void);
extern int pompaIsExecutionActive(int *airflowLevel);
extern int pompaIsKuvetNeeded(void);
extern void pompaSetAeratorDuty(uint32_t duty);

/* ====================================================================
 * Kontrol Pompa Aktual
 * ==================================================================== */

#define AERATOR_PWM_DUTY_LEVEL_1 130
#define AERATOR_PWM_DUTY_LEVEL_2 179
#define AERATOR_PWM_DUTY_LEVEL_3 214
#define PUMP_PWM_FREQ_HZ 5000
#define PUMP_PWM_MODE LEDC_HIGH_SPEED_MODE
#define PUMP_PWM_TIMER LEDC_TIMER_2
#define PUMP_PWM_RESOLUTION LEDC_TIMER_8_BIT
#define PUMP_UPDATE_MS 100

#define FLOWRATE_HIGH_ML_MIN 30.0f
#define FLOWRATE_LOW_ML_MIN 120.0f

/* Flowrate saat kick PWM 255 (lebih tinggi dari flowrate target).
 * Dipakai untuk memperhitungkan volume yang sudah terpompa selama 100 ms kick. */
#define FLOWRATE_KICK_HIGH_ML_MIN 45.0f
#define FLOWRATE_KICK_LOW_ML_MIN 150.0f

#define PUMP_PWM_SAMPLE 127
#define PUMP_PWM_H2SO4 170
#define PUMP_PWM_FESO4 187
#define PUMP_PWM_WASTE_CONTAINER 255
#define PUMP_PWM_AIR 200
#define PUMP_PWM_KUVET 255 // TODO: cari PWM agar 30 mL/min, kondisi kedap air

#define PUMP_KICK_DUTY 255
#define PUMP_KICK_MS 50

/* Durasi flush kuvet spektrofotometer saat flushing (ms).
 * Volume kuvet jauh lebih kecil dari reaktor, jadi durasinya tetap (bukan
 * dari volume sampel). Tune agar air cukup mengganti isi kuvet + selang. */
#define PUMP_FLUSH_KUVET_MS 5000

/* --------------------------------------------------------------------
 * Pengisian awal pipa (prefill) saat boot
 * Setelah seluruh perangkat keras pompa siap (sistem selesai boot),
 * semua pompa dijalankan pada PWM 255 untuk mendorong fluida melewati
 * pipa sehingga sudah dekat ke wadah tujuan saat pompa benar-benar dipakai.
 * Durasi (ms) per pompa diatur agar fluida pas mencapai ujung pipa;
 * set 0 untuk melewati pompa itu.
 * ------------------------------------------------------------------ */
#define PUMP_PREFILL_DUTY 255

#define PUMP_PREFILL_MS_SAMPLE 2480
#define PUMP_PREFILL_MS_H2SO4 4050
#define PUMP_PREFILL_MS_FESO4 4560
#define PUMP_PREFILL_MS_WASTE_CONTAINER 0
#define PUMP_PREFILL_MS_AIR 0
#define PUMP_PREFILL_MS_KUVET 0
#define PUMP_PREFILL_MS_AERATOR 0

#define PUMP_CH_H2SO4 LEDC_CHANNEL_2
#define PUMP_CH_FESO4 LEDC_CHANNEL_3
#define PUMP_CH_SAMPLE LEDC_CHANNEL_4
#define PUMP_CH_WASTE_CONTAINER LEDC_CHANNEL_5
#define PUMP_CH_AIR LEDC_CHANNEL_6
#define PUMP_CH_KUVET LEDC_CHANNEL_7

static float lastSampleVolMl = 0.0f;

static uint32_t elapsedMsSince(TickType_t startTick)
{
    TickType_t now = xTaskGetTickCount();
    return (uint32_t)((now - startTick) * portTICK_PERIOD_MS);
}

static uint32_t calcDurationMs(float volumeMl, float flowrateMlMin)
{
    if (volumeMl <= 0.0f || flowrateMlMin <= 0.0f)
    {
        return 0;
    }
    float minutes = volumeMl / flowrateMlMin;
    float ms = minutes * 60000.0f;
    if (ms < 0.0f)
    {
        ms = 0.0f;
    }
    return (uint32_t)(ms + 0.5f);
}

/* Hitung durasi fase target setelah memperhitungkan volume yang sudah
 * terpompa selama kick PWM 255 (100 ms) pada flowrate kick yang lebih tinggi.
 * Volume kick dikurangkan dari volume target sebelum menghitung durasi. */
static uint32_t calcTargetDurationMs(float volumeMl, float targetFlowrateMlMin, float kickFlowrateMlMin)
{
    float kickVolMl = kickFlowrateMlMin * ((float)PUMP_KICK_MS / 60000.0f);
    float remainingMl = volumeMl - kickVolMl;
    if (remainingMl < 0.0f)
    {
        remainingMl = 0.0f;
    }
    return calcDurationMs(remainingMl, targetFlowrateMlMin);
}

static void pumpSetDuty(ledc_channel_t channel, uint32_t duty)
{
    ledc_set_duty(PUMP_PWM_MODE, channel, duty);
    ledc_update_duty(PUMP_PWM_MODE, channel);
}

static void pumpKickStart(ledc_channel_t channel, uint32_t targetDuty)
{
    pumpSetDuty(channel, PUMP_KICK_DUTY);
    vTaskDelay(pdMS_TO_TICKS(PUMP_KICK_MS));
    pumpSetDuty(channel, targetDuty);
}

static void aeratorKickStart(uint32_t targetDuty)
{
    pompaSetAeratorDuty(PUMP_KICK_DUTY);
    vTaskDelay(pdMS_TO_TICKS(PUMP_KICK_MS));
    pompaSetAeratorDuty(targetDuty);
}

static void setupPumpChannel(ledc_channel_t channel, int gpioNum)
{
    ledc_channel_config_t ch = {
        .gpio_num = gpioNum,
        .speed_mode = PUMP_PWM_MODE,
        .channel = channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = PUMP_PWM_TIMER,
        .duty = 0,
        .hpoint = 0};
    ledc_channel_config(&ch);
}

static void setupPumpPwm(void)
{
    ledc_timer_config_t timer = {
        .speed_mode = PUMP_PWM_MODE,
        .duty_resolution = PUMP_PWM_RESOLUTION,
        .timer_num = PUMP_PWM_TIMER,
        .freq_hz = PUMP_PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&timer);

    setupPumpChannel(PUMP_CH_H2SO4, GPIO_PWM_H2SO4);
    setupPumpChannel(PUMP_CH_FESO4, GPIO_PWM_FESO4);
    setupPumpChannel(PUMP_CH_SAMPLE, GPIO_PWM_SAMPLE);
    setupPumpChannel(PUMP_CH_WASTE_CONTAINER, GPIO_PWM_WASTE_CONTAINER);
    setupPumpChannel(PUMP_CH_AIR, GPIO_PWM_AIR);
    setupPumpChannel(PUMP_CH_KUVET, GPIO_PWM_KUVET);
}

/* ------------------------------------------------------------------ */
/* Jalankan pompa-pompa untuk mengisi pipa pada PWM 255, lalu matikan tiap
 * pompa setelah durasi spesifiknya tercapai.
 * includeH2SO4 / includeFeSO4: 1 = ikut di-prefill, 0 = dilewati. */
static void runPrefill(int includeH2SO4, int includeFeSO4)
{
    typedef struct
    {
        ledc_channel_t channel;
        uint32_t durationMs;
        bool active;
    } PrefillPump;

    PrefillPump pumps[] = {
        {PUMP_CH_SAMPLE, PUMP_PREFILL_MS_SAMPLE, (PUMP_PREFILL_MS_SAMPLE > 0)},
        {PUMP_CH_H2SO4, PUMP_PREFILL_MS_H2SO4, (PUMP_PREFILL_MS_H2SO4 > 0) && (includeH2SO4 != 0)},
        {PUMP_CH_FESO4, PUMP_PREFILL_MS_FESO4, (PUMP_PREFILL_MS_FESO4 > 0) && (includeFeSO4 != 0)},
        {PUMP_CH_WASTE_CONTAINER, PUMP_PREFILL_MS_WASTE_CONTAINER, (PUMP_PREFILL_MS_WASTE_CONTAINER > 0)},
        {PUMP_CH_AIR, PUMP_PREFILL_MS_AIR, (PUMP_PREFILL_MS_AIR > 0)},
        {PUMP_CH_KUVET, PUMP_PREFILL_MS_KUVET, (PUMP_PREFILL_MS_KUVET > 0)},
    };
    const int count = sizeof(pumps) / sizeof(pumps[0]);
    bool aeratorActive = (PUMP_PREFILL_MS_AERATOR > 0);

    printf("[Init] Prefill pipa...\n");

    /* Nyalakan semua pompa pada PWM 255 secara bersamaan */
    for (int i = 0; i < count; i++)
    {
        if (pumps[i].active)
        {
            pumpSetDuty(pumps[i].channel, PUMP_PREFILL_DUTY);
        }
    }
    if (aeratorActive)
    {
        pompaSetAeratorDuty(PUMP_PREFILL_DUTY);
    }

    /* Matikan tiap pompa setelah durasi spesifiknya tercapai */
    TickType_t startTick = xTaskGetTickCount();
    bool anyActive = true;
    while (anyActive)
    {
        uint32_t elapsedMs = elapsedMsSince(startTick);
        anyActive = false;

        for (int i = 0; i < count; i++)
        {
            if (pumps[i].active)
            {
                if (elapsedMs >= pumps[i].durationMs)
                {
                    pumpSetDuty(pumps[i].channel, 0);
                    pumps[i].active = false;
                }
                else
                {
                    anyActive = true;
                }
            }
        }

        if (aeratorActive)
        {
            if (elapsedMs >= PUMP_PREFILL_MS_AERATOR)
            {
                pompaSetAeratorDuty(0);
                aeratorActive = false;
            }
            else
            {
                anyActive = true;
            }
        }

        if (anyActive)
        {
            vTaskDelay(pdMS_TO_TICKS(PUMP_UPDATE_MS));
        }
    }

    printf("[Init] Prefill pipa selesai.\n");
}

static uint32_t aeratorDutyFromLevel(int level)
{
    if (level <= 0)
    {
        return 0;
    }
    if (level == 1)
    {
        return AERATOR_PWM_DUTY_LEVEL_1;
    }
    if (level == 2)
    {
        return AERATOR_PWM_DUTY_LEVEL_2;
    }
    return AERATOR_PWM_DUTY_LEVEL_3;
}

static void updateExecutionPumps(void)
{
    static bool kuvetWasActive = false;
    static bool aeratorWasActive = false;
    int airflowLevel = 0;

    if (pompaIsExecutionActive(&airflowLevel))
    {
        if (pompaIsKuvetNeeded())
        {
            if (!kuvetWasActive)
            {
                pumpKickStart(PUMP_CH_KUVET, PUMP_PWM_KUVET);
                kuvetWasActive = true;
            }
            else
            {
                pumpSetDuty(PUMP_CH_KUVET, PUMP_PWM_KUVET);
            }
        }
        else
        {
            pumpSetDuty(PUMP_CH_KUVET, 0);
            kuvetWasActive = false;
        }

        uint32_t aeratorDuty = aeratorDutyFromLevel(airflowLevel);
        if (aeratorDuty > 0)
        {
            if (!aeratorWasActive)
            {
                aeratorKickStart(aeratorDuty);
                aeratorWasActive = true;
            }
            else
            {
                pompaSetAeratorDuty(aeratorDuty);
            }
        }
        else
        {
            /* airflow level 0: aerator mati, reset agar kick lagi saat dinyalakan */
            pompaSetAeratorDuty(0);
            aeratorWasActive = false;
        }
        return;
    }

    pumpSetDuty(PUMP_CH_KUVET, 0);
    pompaSetAeratorDuty(0);
    kuvetWasActive = false;
    aeratorWasActive = false;
}

/* ------------------------------------------------------------------ */
static void runInisialisasi(const InisialisasiConfig *cfg)
{
    InisialisasiProgress prog;
    memset(&prog, 0, sizeof(prog));

    prog.sampelPercent = 0;
    prog.h2so4Status = (cfg->ph < 0) ? -1 : 0;
    prog.feso4Status = (cfg->fe < 0) ? -1 : 0;
    prog.kalibrasiStatus = (cfg->kalibrasi == 0) ? -1 : 0;
    prog.prefillStatus = (cfg->prefilling == 1) ? 0 : -1;

    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
    {
        currentInitProgress = prog;
        initDoneFlag = 0;
        xSemaphoreGive(mutexSystemState);
    }

    /* === Fase 0 : Prefill pipa (opsional) ========================== */
    if (cfg->prefilling == 1)
    {
        runPrefill((cfg->ph >= 0.0f) ? 1 : 0, (cfg->fe >= 0.0f) ? 1 : 0);
        if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
        {
            currentInitProgress.prefillStatus = 1;
            xSemaphoreGive(mutexSystemState);
        }
    }

    lastSampleVolMl = (cfg->vol > 0) ? (float)cfg->vol : 0.0f;

    float sampleMl = lastSampleVolMl;
    float h2so4Ml = (cfg->ph >= 0.0f) ? cfg->ph : 0.0f;
    float feMl = 0.0f;

    if (cfg->fe >= 0.0f)
    {
        if (strncmp(cfg->fe_type, "mm", 2) == 0)
        {
            feMl = (sampleMl * cfg->fe) / 40.0f;
        }
        else
        {
            feMl = cfg->fe;
        }
    }

    uint32_t sampleMs = calcTargetDurationMs(sampleMl, FLOWRATE_LOW_ML_MIN, FLOWRATE_KICK_LOW_ML_MIN);
    uint32_t h2so4Ms = calcTargetDurationMs(h2so4Ml, FLOWRATE_HIGH_ML_MIN, FLOWRATE_KICK_HIGH_ML_MIN);
    uint32_t feMs = calcTargetDurationMs(feMl, FLOWRATE_HIGH_ML_MIN, FLOWRATE_KICK_HIGH_ML_MIN);

    int h2so4Status = prog.h2so4Status;
    int feStatus = prog.feso4Status;
    int sampelPercent = 0;

    bool sampleOn = (sampleMs > 0);
    bool h2so4On = (h2so4Status == 0 && h2so4Ms > 0);
    bool feOn = (feStatus == 0 && feMs > 0);

    if (!sampleOn)
    {
        sampelPercent = 100;
    }
    if (h2so4Status == 0 && !h2so4On)
    {
        h2so4Status = 1;
    }
    if (feStatus == 0 && !feOn)
    {
        feStatus = 1;
    }

    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
    {
        currentInitProgress.sampelPercent = sampelPercent;
        currentInitProgress.h2so4Status = h2so4Status;
        currentInitProgress.feso4Status = feStatus;
        currentInitProgress.kalibrasiStatus = prog.kalibrasiStatus;
        xSemaphoreGive(mutexSystemState);
    }

    if (sampleOn)
    {
        pumpSetDuty(PUMP_CH_SAMPLE, PUMP_KICK_DUTY);
    }
    if (h2so4On)
    {
        pumpSetDuty(PUMP_CH_H2SO4, PUMP_KICK_DUTY);
    }
    if (feOn)
    {
        pumpSetDuty(PUMP_CH_FESO4, PUMP_KICK_DUTY);
    }
    if (sampleOn || h2so4On || feOn)
    {
        vTaskDelay(pdMS_TO_TICKS(PUMP_KICK_MS));
    }
    if (sampleOn)
    {
        pumpSetDuty(PUMP_CH_SAMPLE, PUMP_PWM_SAMPLE);
    }
    if (h2so4On)
    {
        pumpSetDuty(PUMP_CH_H2SO4, PUMP_PWM_H2SO4);
    }
    if (feOn)
    {
        pumpSetDuty(PUMP_CH_FESO4, PUMP_PWM_FESO4);
    }

    TickType_t startTick = xTaskGetTickCount();
    while (sampleOn || h2so4On || feOn)
    {
        uint32_t elapsedMs = elapsedMsSince(startTick);

        if (sampleOn)
        {
            if (elapsedMs >= sampleMs)
            {
                sampleOn = false;
                pumpSetDuty(PUMP_CH_SAMPLE, 0);
                sampelPercent = 100;
            }
            else
            {
                sampelPercent = (int)((elapsedMs * 100UL) / sampleMs);
                if (sampelPercent > 100)
                {
                    sampelPercent = 100;
                }
            }
        }

        if (h2so4On && elapsedMs >= h2so4Ms)
        {
            h2so4On = false;
            pumpSetDuty(PUMP_CH_H2SO4, 0);
            h2so4Status = 1;
        }

        if (feOn && elapsedMs >= feMs)
        {
            feOn = false;
            pumpSetDuty(PUMP_CH_FESO4, 0);
            feStatus = 1;
        }

        if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
        {
            currentInitProgress.sampelPercent = sampelPercent;
            currentInitProgress.h2so4Status = h2so4Status;
            currentInitProgress.feso4Status = feStatus;
            xSemaphoreGive(mutexSystemState);
        }

        vTaskDelay(pdMS_TO_TICKS(PUMP_UPDATE_MS));
    }

    /* === Fase 4 : Kalibrasi spektrofotometer ======================== */
    if (cfg->kalibrasi == 1)
    {
        runKalibrasiSpektro(); /* implementasi ada di taskKalibrasiSpektro.c */
        if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
        {
            currentInitProgress.kalibrasiStatus = 1;
            xSemaphoreGive(mutexSystemState);
        }
    }

    /* Tandai inisialisasi selesai — taskKomunikasi akan stream "done" */
    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
    {
        initDoneFlag = 1;
        xSemaphoreGive(mutexSystemState);
    }
}

/* ------------------------------------------------------------------ */
/* Jalankan satu fase flushing: kick-start pompa, jalankan selama phaseDurMs,
 * sambil stream progress total ke UI, lalu matikan pompa. */
static void runFlushPhase(ledc_channel_t channel, uint32_t targetDuty,
                          uint32_t phaseDurMs, TickType_t totalStart, uint32_t totalMs)
{
    pumpKickStart(channel, targetDuty);
    TickType_t phaseStart = xTaskGetTickCount();
    while (elapsedMsSince(phaseStart) < phaseDurMs)
    {
        uint32_t totalElapsed = elapsedMsSince(totalStart);
        int pct = (totalMs > 0) ? (int)((totalElapsed * 100UL) / totalMs) : 100;
        if (pct > 100)
        {
            pct = 100;
        }
        if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
        {
            currentFlushProgress.progressPercent = pct;
            xSemaphoreGive(mutexSystemState);
        }
        vTaskDelay(pdMS_TO_TICKS(PUMP_UPDATE_MS));
    }
    pumpSetDuty(channel, 0);
}

/* ------------------------------------------------------------------ */
static void runFlushing(void)
{
    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
    {
        currentFlushProgress.progressPercent = 0;
        flushDoneFlag = 0;
        xSemaphoreGive(mutexSystemState);
    }

    uint32_t phaseMs = calcDurationMs(lastSampleVolMl, FLOWRATE_LOW_ML_MIN);
    uint32_t kuvetMs = PUMP_FLUSH_KUVET_MS;
    /* 3 fase berbasis volume sampel (buang, isi air, buang) + 1 fase flush kuvet */
    uint32_t totalMs = phaseMs * 3U + kuvetMs;

    if (phaseMs == 0 || totalMs == 0)
    {
        if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
        {
            currentFlushProgress.progressPercent = 100;
            flushDoneFlag = 1;
            xSemaphoreGive(mutexSystemState);
        }
        return;
    }

    TickType_t totalStart = xTaskGetTickCount();

    /* Fase 1: buang sample ke waste container */
    runFlushPhase(PUMP_CH_WASTE_CONTAINER, PUMP_PWM_WASTE_CONTAINER, phaseMs, totalStart, totalMs);

    /* Fase 2: pompa air untuk mengisi reaktor */
    runFlushPhase(PUMP_CH_AIR, PUMP_PWM_AIR, phaseMs, totalStart, totalMs);

    /* Fase 3: alirkan air melalui kuvet spektrofotometer untuk membilasnya */
    runFlushPhase(PUMP_CH_KUVET, PUMP_PWM_KUVET, kuvetMs, totalStart, totalMs);

    /* Fase 4: buang seluruh air ke waste container */
    runFlushPhase(PUMP_CH_WASTE_CONTAINER, PUMP_PWM_WASTE_CONTAINER, phaseMs, totalStart, totalMs);

    /* Tandai flushing selesai — taskKomunikasi akan stream "flushing_done" */
    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE)
    {
        currentFlushProgress.progressPercent = 100;
        flushDoneFlag = 1;
        xSemaphoreGive(mutexSystemState);
    }
}

/* ====================================================================
 * Entry point FreeRTOS task
 * ==================================================================== */
void taskManajemenPompa(void *pvParameters)
{
    (void)pvParameters;
    printf("[Init] Task Manajemen Pompa Siap!\n");

    setupPumpPwm();
    pompaExecutionHardwareInit();

    InisialisasiConfig initCfg;
    int flushCmd;

    for (;;)
    {
        /* Cek perintah inisialisasi dari taskKomunikasi */
        if (xQueueReceive(queueInisialisasiCmd, &initCfg, 0) == pdTRUE)
        {
            runInisialisasi(&initCfg);
        }

        /* Cek perintah flushing dari taskKomunikasi */
        if (xQueueReceive(queueFlushingCmd, &flushCmd, 0) == pdTRUE)
        {

            runFlushing();
        }

        updateExecutionPumps();

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
