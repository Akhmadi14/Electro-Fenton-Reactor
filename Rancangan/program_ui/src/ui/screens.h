#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MENU_INTRO = 1,
    SCREEN_ID_MENU_PENJADWALAN_PROSES_UTAMA = 2,
    SCREEN_ID_MENU_PENJADWALAN_PROSES_PILIH = 3,
    SCREEN_ID_MENU_PENJADWALAN_PROSES_INPUT_ELEKTROFENTON = 4,
    SCREEN_ID_MENU_PENJADWALAN_PROSES_INPUT_ADSORPSI = 5,
    SCREEN_ID_MENU_MONITOR = 6,
    SCREEN_ID_MENU_INISIALISASI = 7,
    SCREEN_ID_MENU_MELAKUKAN_INISIALISASI = 8,
    SCREEN_ID_MENU_SPEKTRO_DEGRADASI_WARNA = 9,
    SCREEN_ID_MENU_SPEKTRO_SPEKTRUM_WARNA = 10,
    SCREEN_ID_MENU_FLUSHING = 11,
    _SCREEN_ID_LAST = 11
};

typedef struct _objects_t {
    lv_obj_t *menu_intro;
    lv_obj_t *menu_penjadwalan_proses_utama;
    lv_obj_t *menu_penjadwalan_proses_pilih;
    lv_obj_t *menu_penjadwalan_proses_input_elektrofenton;
    lv_obj_t *menu_penjadwalan_proses_input_adsorpsi;
    lv_obj_t *menu_monitor;
    lv_obj_t *menu_inisialisasi;
    lv_obj_t *menu_melakukan_inisialisasi;
    lv_obj_t *menu_spektro_degradasi_warna;
    lv_obj_t *menu_spektro_spektrum_warna;
    lv_obj_t *menu_flushing;
    lv_obj_t *container_content;
    lv_obj_t *button_start_jadwal;
    lv_obj_t *simbol_start_jadwal;
    lv_obj_t *teks_start;
    lv_obj_t *button_tambah;
    lv_obj_t *simbol_monitor_1;
    lv_obj_t *button_next_page;
    lv_obj_t *simbol_monitor_2;
    lv_obj_t *button_back_page;
    lv_obj_t *simbol_monitor_6;
    lv_obj_t *container_keterangan_menu;
    lv_obj_t *teks_keterangan_menu;
    lv_obj_t *list_jadwal;
    lv_obj_t *container_menu_10;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_21;
    lv_obj_t *simbol_jadwal_25;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_22;
    lv_obj_t *simbol_jadwal_26;
    lv_obj_t *button_monitor_proses_input_elektrofenton_6;
    lv_obj_t *simbol_monitor_14;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_23;
    lv_obj_t *simbol_jadwal_27;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_24;
    lv_obj_t *simbol_jadwal_28;
    lv_obj_t *teks_jumlah_halaman;
    lv_obj_t *container_content_1;
    lv_obj_t *container_pilihan_proses;
    lv_obj_t *button_menu_elektrofenton;
    lv_obj_t *simbol_monitor_3;
    lv_obj_t *teks_elektrofenton;
    lv_obj_t *button_menu_adsorpsi;
    lv_obj_t *simbol_monitor_4;
    lv_obj_t *teks_elektrofenton_1;
    lv_obj_t *button_menu_spektrofotometri;
    lv_obj_t *simbol_monitor_5;
    lv_obj_t *teks_spektrofotometer;
    lv_obj_t *button_kembali;
    lv_obj_t *simbol_kembali;
    lv_obj_t *container_keterangan_menu_1;
    lv_obj_t *teks_keterangan_menu_1;
    lv_obj_t *container_menu_11;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_25;
    lv_obj_t *simbol_jadwal_29;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_26;
    lv_obj_t *simbol_jadwal_30;
    lv_obj_t *button_monitor_proses_input_elektrofenton_7;
    lv_obj_t *simbol_monitor_15;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_27;
    lv_obj_t *simbol_jadwal_31;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_28;
    lv_obj_t *simbol_jadwal_32;
    lv_obj_t *container_content_2;
    lv_obj_t *container_pilihan_proses_1;
    lv_obj_t *container_aturmagnetic_stirrer_1;
    lv_obj_t *teks_kecepatan_magnetic_stirrer_1;
    lv_obj_t *textarea_input_magnetic_stirrer_speed;
    lv_obj_t *teks_rpm_1;
    lv_obj_t *container_aturmagnetic_stirrer_2;
    lv_obj_t *teks_suhu;
    lv_obj_t *textarea_input_suhu;
    lv_obj_t *teks_celcius;
    lv_obj_t *container_arus_listrik;
    lv_obj_t *teks_arus_listrik;
    lv_obj_t *textarea_input_arus;
    lv_obj_t *teks_celcius_1;
    lv_obj_t *container_durasi;
    lv_obj_t *teks_durasi;
    lv_obj_t *textarea_input_jam;
    lv_obj_t *teks_durasi_1;
    lv_obj_t *textarea_input_menit;
    lv_obj_t *container_airflow;
    lv_obj_t *teks_airflow;
    lv_obj_t *roller_input_level_airflow;
    lv_obj_t *container_ukur_degradasi;
    lv_obj_t *teks_ukur_degradasi;
    lv_obj_t *button_kembali_1;
    lv_obj_t *simbol_kembali_1;
    lv_obj_t *teks_keterangan_input_parameter_elektrofenton;
    lv_obj_t *button_kembali_2;
    lv_obj_t *simbol_kembali_2;
    lv_obj_t *container_keterangan_menu_2;
    lv_obj_t *teks_keterangan_menu_2;
    lv_obj_t *keyboard_elektrofenton;
    lv_obj_t *teks_peringatan1;
    lv_obj_t *teks_peringatan1_1;
    lv_obj_t *teks_peringatan1_2;
    lv_obj_t *teks_peringatan1_3;
    lv_obj_t *teks_peringatan1_4;
    lv_obj_t *messagebox_peringatan_elektrofenton;
    lv_obj_t *pesan_peringatan;
    lv_obj_t *container_menu_12;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_29;
    lv_obj_t *simbol_jadwal_33;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_30;
    lv_obj_t *simbol_jadwal_34;
    lv_obj_t *button_monitor_proses_input_elektrofenton_8;
    lv_obj_t *simbol_monitor_16;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_31;
    lv_obj_t *simbol_jadwal_35;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_32;
    lv_obj_t *simbol_jadwal_36;
    lv_obj_t *container_content_3;
    lv_obj_t *teks_tambah;
    lv_obj_t *container_pilihan_proses_2;
    lv_obj_t *container_aturmagnetic_stirrer_3;
    lv_obj_t *teks_kecepatan_magnetic_stirrer_2;
    lv_obj_t *textarea_input_magnetic_stirrer_speed_1;
    lv_obj_t *teks_rpm_2;
    lv_obj_t *container_aturmagnetic_stirrer_4;
    lv_obj_t *teks_suhu_1;
    lv_obj_t *textarea_input_suhu_1;
    lv_obj_t *teks_celcius_2;
    lv_obj_t *container_durasi_1;
    lv_obj_t *teks_durasi_2;
    lv_obj_t *textarea_input_jam_1;
    lv_obj_t *teks_durasi_3;
    lv_obj_t *textarea_input_menit_1;
    lv_obj_t *container_ukur_degradasi_1;
    lv_obj_t *teks_ukur_degradasi_1;
    lv_obj_t *button_kembali_3;
    lv_obj_t *simbol_kembali_3;
    lv_obj_t *teks_keterangan_input_parameter_elektrofenton_1;
    lv_obj_t *button_kembali_4;
    lv_obj_t *simbol_kembali_4;
    lv_obj_t *container_keterangan_menu_3;
    lv_obj_t *teks_keterangan_menu_3;
    lv_obj_t *keyboard_adsorpsi;
    lv_obj_t *teks_peringatan1_5;
    lv_obj_t *teks_peringatan1_6;
    lv_obj_t *teks_peringatan1_7;
    lv_obj_t *teks_peringatan1_8;
    lv_obj_t *messagebox_peringatan_adsorpsi;
    lv_obj_t *pesan_peringatan_1;
    lv_obj_t *container_menu_13;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_33;
    lv_obj_t *simbol_jadwal_37;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_34;
    lv_obj_t *simbol_jadwal_38;
    lv_obj_t *button_monitor_proses_input_elektrofenton_9;
    lv_obj_t *simbol_monitor_17;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_35;
    lv_obj_t *simbol_jadwal_39;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_36;
    lv_obj_t *simbol_jadwal_40;
    lv_obj_t *container_content_4;
    lv_obj_t *button_force_stop;
    lv_obj_t *simbol_stop;
    lv_obj_t *teks_stop;
    lv_obj_t *container_keterangan_menu_4;
    lv_obj_t *teks_keterangan_menu_4;
    lv_obj_t *container_menu_14;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_37;
    lv_obj_t *simbol_jadwal_41;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_38;
    lv_obj_t *simbol_jadwal_42;
    lv_obj_t *button_monitor_proses_input_elektrofenton_10;
    lv_obj_t *simbol_monitor_18;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_39;
    lv_obj_t *simbol_jadwal_43;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_40;
    lv_obj_t *simbol_jadwal_44;
    lv_obj_t *container_menu_5;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_3;
    lv_obj_t *simbol_jadwal_7;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_1;
    lv_obj_t *simbol_jadwal_5;
    lv_obj_t *button_monitor_proses_input_elektrofenton_1;
    lv_obj_t *simbol_monitor_9;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_2;
    lv_obj_t *simbol_jadwal_6;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_4;
    lv_obj_t *simbol_jadwal_8;
    lv_obj_t *container_content_5;
    lv_obj_t *button_start_jadwal_1;
    lv_obj_t *simbol_start_jadwal_1;
    lv_obj_t *teks_start_1;
    lv_obj_t *container_pilihan_proses_3;
    lv_obj_t *container_input_volume_sampel;
    lv_obj_t *teks_input_volume_sampel;
    lv_obj_t *textarea_input_volume;
    lv_obj_t *teks_ml;
    lv_obj_t *container_ph_target;
    lv_obj_t *teks_ph_taget;
    lv_obj_t *textarea_input_ph;
    lv_obj_t *teks_ml_1;
    lv_obj_t *container_konsentrasi_fe;
    lv_obj_t *teks_konsentrasi_fe;
    lv_obj_t *textarea_input_arus_1;
    lv_obj_t *fe_type_scrollbar;
    lv_obj_t *container_kalibrasi_spektro;
    lv_obj_t *teks_preflill_pompa;
    lv_obj_t *switch_prefill_pompa;
    lv_obj_t *container_keterangan_menu_5;
    lv_obj_t *teks_keterangan_menu_5;
    lv_obj_t *keyboard_elektrofenton_1;
    lv_obj_t *teks_peringatan1_9;
    lv_obj_t *teks_peringatan1_10;
    lv_obj_t *teks_peringatan1_11;
    lv_obj_t *messagebox_peringatan_input_inisialisasi;
    lv_obj_t *pesan_peringatan_2;
    lv_obj_t *container_content_6;
    lv_obj_t *status_inisialisasi;
    lv_obj_t *container_pilihan_proses_4;
    lv_obj_t *container_progress_mengisi_reaktor;
    lv_obj_t *teks_progress_mengisi_reaktor;
    lv_obj_t *progress_bar_mengisi_reaktor;
    lv_obj_t *container_mengatur_ph;
    lv_obj_t *teks_mengatur_ph;
    lv_obj_t *teks_status_mengatur_ph;
    lv_obj_t *container_mengatur_konsentrasi_fe2_;
    lv_obj_t *teks_mengatur_konsentrasi_fe2;
    lv_obj_t *teks_status_konsentrasi_fe2;
    lv_obj_t *container_status_kalibrasi_spektro;
    lv_obj_t *teks_kalibrasi_spektro_bagaimana;
    lv_obj_t *teks_status_kalibrasi_spektro;
    lv_obj_t *container_status_prefilling;
    lv_obj_t *teks_prefilling;
    lv_obj_t *teks_status_prefilling;
    lv_obj_t *container_keterangan_menu_6;
    lv_obj_t *teks_keterangan_menu_6;
    lv_obj_t *keyboard_elektrofenton_2;
    lv_obj_t *container_menu_6;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_5;
    lv_obj_t *simbol_jadwal_9;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_6;
    lv_obj_t *simbol_jadwal_10;
    lv_obj_t *button_monitor_proses_input_elektrofenton_2;
    lv_obj_t *simbol_monitor_10;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_7;
    lv_obj_t *simbol_jadwal_11;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_8;
    lv_obj_t *simbol_jadwal_12;
    lv_obj_t *pindah_ke_jadwal;
    lv_obj_t *status_inisialisasi_1;
    lv_obj_t *container_content_7;
    lv_obj_t *button_menu_degradasi_warna;
    lv_obj_t *button_menu_spektrum_warna;
    lv_obj_t *button_simpan_grafik_1;
    lv_obj_t *container_keterangan_menu_7;
    lv_obj_t *teks_keterangan_menu_7;
    lv_obj_t *chart_degradasi_warna;
    lv_obj_t *list_grafik_tersedia;
    lv_obj_t *container_menu_7;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_9;
    lv_obj_t *simbol_jadwal_13;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_10;
    lv_obj_t *simbol_jadwal_14;
    lv_obj_t *button_monitor_proses_input_elektrofenton_3;
    lv_obj_t *simbol_monitor_11;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_11;
    lv_obj_t *simbol_jadwal_15;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_12;
    lv_obj_t *simbol_jadwal_16;
    lv_obj_t *messagebox_status_penyimpanan_2;
    lv_obj_t *pesan_peringatan_6;
    lv_obj_t *teks_menit;
    lv_obj_t *teks_degradasi;
    lv_obj_t *pesan_peringatan_3;
    lv_obj_t *container_content_8;
    lv_obj_t *button_menu_degradasi_warna_1;
    lv_obj_t *button_menu_spektrum_warna_1;
    lv_obj_t *button_simpan_grafik;
    lv_obj_t *container_keterangan_menu_8;
    lv_obj_t *teks_keterangan_menu_8;
    lv_obj_t *chart_degradasi_warna_1;
    lv_obj_t *list_grafik_tersedia_1;
    lv_obj_t *container_menu_8;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_13;
    lv_obj_t *simbol_jadwal_17;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_14;
    lv_obj_t *simbol_jadwal_18;
    lv_obj_t *button_monitor_proses_input_elektrofenton_4;
    lv_obj_t *simbol_monitor_12;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_15;
    lv_obj_t *simbol_jadwal_19;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_16;
    lv_obj_t *simbol_jadwal_20;
    lv_obj_t *messagebox_status_penyimpanan;
    lv_obj_t *pesan_peringatan_5;
    lv_obj_t *teks_nm;
    lv_obj_t *teks_ads;
    lv_obj_t *container_content_9;
    lv_obj_t *container_progress_flushing;
    lv_obj_t *progress_bar_proses_flushing;
    lv_obj_t *teks_keterangan_menu_10;
    lv_obj_t *button_start_jadwal_2;
    lv_obj_t *simbol_start_jadwal_2;
    lv_obj_t *container_keterangan_menu_9;
    lv_obj_t *teks_keterangan_menu_9;
    lv_obj_t *messagebox_konfirmasi_flushing;
    lv_obj_t *teks_konfirmasi_flushing;
    lv_obj_t *button_konfirmasi_flushing;
    lv_obj_t *teks_konfirmasi_flushing_dibutton;
    lv_obj_t *container_menu_9;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_17;
    lv_obj_t *simbol_jadwal_21;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_18;
    lv_obj_t *simbol_jadwal_22;
    lv_obj_t *button_monitor_proses_input_elektrofenton_5;
    lv_obj_t *simbol_monitor_13;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_19;
    lv_obj_t *simbol_jadwal_23;
    lv_obj_t *button_penjadwalan_proses_input_elektrofenton_20;
    lv_obj_t *simbol_jadwal_24;
    lv_obj_t *message_flushing_sukses;
    lv_obj_t *teks_flushing_sukses;
} objects_t;

extern objects_t objects;

void create_screen_menu_intro();
void tick_screen_menu_intro();

void create_screen_menu_penjadwalan_proses_utama();
void tick_screen_menu_penjadwalan_proses_utama();

void create_screen_menu_penjadwalan_proses_pilih();
void tick_screen_menu_penjadwalan_proses_pilih();

void create_screen_menu_penjadwalan_proses_input_elektrofenton();
void tick_screen_menu_penjadwalan_proses_input_elektrofenton();

void create_screen_menu_penjadwalan_proses_input_adsorpsi();
void tick_screen_menu_penjadwalan_proses_input_adsorpsi();

void create_screen_menu_monitor();
void tick_screen_menu_monitor();

void create_screen_menu_inisialisasi();
void tick_screen_menu_inisialisasi();

void create_screen_menu_melakukan_inisialisasi();
void tick_screen_menu_melakukan_inisialisasi();

void create_screen_menu_spektro_degradasi_warna();
void tick_screen_menu_spektro_degradasi_warna();

void create_screen_menu_spektro_spektrum_warna();
void tick_screen_menu_spektro_spektrum_warna();

void create_screen_menu_flushing();
void tick_screen_menu_flushing();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/