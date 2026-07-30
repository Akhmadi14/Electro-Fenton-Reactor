#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_tampil_numpad_rpm(lv_event_t * e);
extern void action_tampil_numpad_suhu(lv_event_t * e);
extern void action_tampil_numpad_arus_listrik(lv_event_t * e);
extern void action_tampil_numpad_jam(lv_event_t * e);
extern void action_tampil_numpad_menit(lv_event_t * e);
extern void action_sembunyikan_numpad(lv_event_t * e);
extern void action_pindah_ke_menu_penjadwalan_proses_utama(lv_event_t * e);
extern void action_pindah_ke_menu_monitor(lv_event_t * e);
extern void action_pindah_ke_menu_penjadwalan_proses_pilih(lv_event_t * e);
extern void action_start_dan_kirim_jadwal(lv_event_t * e);
extern void action_pindah_ke_menu_penjadwalan_proses_input_elektrofenton(lv_event_t * e);
extern void action_pindah_ke_menu_penjadwalan_proses_input_adsorpsi(lv_event_t * e);
extern void action_kembali_ke_menu_penjadwalan_proses_pilih(lv_event_t * e);
extern void action_tambahkan_jadwal_elektrofenton(lv_event_t * e);
extern void action_tambahkan_jadwal_adsorpsi(lv_event_t * e);
extern void action_lakukan_force_stop(lv_event_t * e);
extern void action_tutup_pesan_error_input_elektrofenton(lv_event_t * e);
extern void action_tutup_pesan_error_input_adsorpsi(lv_event_t * e);
extern void action_pindah_ke_menu_spektro(lv_event_t * e);
extern void action_pindah_ke_menu_flushing(lv_event_t * e);
extern void action_mulai_inisialisasi(lv_event_t * e);
extern void action_tutup_pesan_error_input_inisialisasi(lv_event_t * e);
extern void action_pindah_ke_menu_inisialisasi(lv_event_t * e);
extern void action_lakukan_flushing(lv_event_t * e);
extern void action_tampilkan_message_konfirmasi_flushing(lv_event_t * e);
extern void action_pindah_ke_menu_spektro_spektrum_warna(lv_event_t * e);
extern void action_ke_menu_degradasi_warna(lv_event_t * e);
extern void action_tambahkan_spektrofotometri_dan_kembali_ke_menu_penjadwalan_proses(lv_event_t * e);
extern void action_simpan_data_spektro_ke_sdcard(lv_event_t * e);
extern void action_tutup_pesan_status_penyimpanan_data_spektro(lv_event_t * e);
extern void action_next_page(lv_event_t * e);
extern void action_back_page(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/