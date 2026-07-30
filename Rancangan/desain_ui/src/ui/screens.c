#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_menu_intro() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_intro = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 144, 110);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_effiton_logo);
        }
    }
    
    tick_screen_menu_intro();
}

void tick_screen_menu_intro() {
}

void create_screen_menu_penjadwalan_proses_utama() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_penjadwalan_proses_utama = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_start_jadwal
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_start_jadwal = obj;
                    lv_obj_set_pos(obj, 543, 358);
                    lv_obj_set_size(obj, 116, 74);
                    lv_obj_add_event_cb(obj, action_start_dan_kirim_jadwal, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_start_jadwal
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_start_jadwal = obj;
                            lv_obj_set_pos(obj, 32, -6);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_play_64__1_);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            // teks_start
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.teks_start = obj;
                            lv_obj_set_pos(obj, -10, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "START");
                        }
                    }
                }
                {
                    // button_tambah
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_tambah = obj;
                    lv_obj_set_pos(obj, 553, 189);
                    lv_obj_set_size(obj, 114, 103);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_pilih, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_1
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_1 = obj;
                            lv_obj_set_pos(obj, 3, 7);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_add_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Tambah");
                        }
                    }
                }
                {
                    // button_next_page
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_next_page = obj;
                    lv_obj_set_pos(obj, 385, 399);
                    lv_obj_set_size(obj, 59, 60);
                    lv_obj_add_event_cb(obj, action_next_page, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_2
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_2 = obj;
                            lv_obj_set_pos(obj, -12, -6);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_next);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                    }
                }
                {
                    // button_back_page
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_back_page = obj;
                    lv_obj_set_pos(obj, 49, 399);
                    lv_obj_set_size(obj, 59, 60);
                    lv_obj_add_event_cb(obj, action_back_page, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_radius(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_6
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_6 = obj;
                            lv_obj_set_pos(obj, -20, -7);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_next);
                            lv_img_set_angle(obj, 1800);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu = obj;
            lv_obj_set_pos(obj, 271, -48);
            lv_obj_set_size(obj, 363, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "PENJADWALAN PROSES");
                }
            }
        }
        {
            // list_jadwal
            lv_obj_t *obj = lv_list_create(parent_obj);
            objects.list_jadwal = obj;
            lv_obj_set_pos(obj, 154, 65);
            lv_obj_set_size(obj, 395, 345);
        }
        {
            // container_menu_10
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_10 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_21
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_21 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_25
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_25 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_22
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_22 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_26
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_26 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_6
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_6 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_14
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_14 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_23
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_23 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_27
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_27 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_24
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_24 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_28
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_28 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
        {
            // teks_jumlah_halaman
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_jumlah_halaman = obj;
            lv_obj_set_pos(obj, 343, 437);
            lv_obj_set_size(obj, 52, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "1/1");
        }
    }
    
    tick_screen_menu_penjadwalan_proses_utama();
}

void tick_screen_menu_penjadwalan_proses_utama() {
}

void create_screen_menu_penjadwalan_proses_pilih() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_penjadwalan_proses_pilih = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_tambahkan_spektrofotometri_dan_kembali_ke_menu_penjadwalan_proses, LV_EVENT_PRESSED, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_1 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // container_pilihan_proses
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.container_pilihan_proses = obj;
                    lv_obj_set_pos(obj, 28, 111);
                    lv_obj_set_size(obj, 640, 238);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_SPACE_EVENLY, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // button_menu_elektrofenton
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.button_menu_elektrofenton = obj;
                            lv_obj_set_pos(obj, 50, 10);
                            lv_obj_set_size(obj, 175, 223);
                            lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_input_elektrofenton, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // simbol_monitor_3
                                    lv_obj_t *obj = lv_img_create(parent_obj);
                                    objects.simbol_monitor_3 = obj;
                                    lv_obj_set_pos(obj, 18, 37);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_img_set_src(obj, &img_icons8_quick_mode_on_100);
                                    lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                                }
                                {
                                    // teks_elektrofenton
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_elektrofenton = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "ELEKTRO FENTON");
                                }
                            }
                        }
                        {
                            // button_menu_adsorpsi
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.button_menu_adsorpsi = obj;
                            lv_obj_set_pos(obj, 50, 10);
                            lv_obj_set_size(obj, 175, 223);
                            lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_input_adsorpsi, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // simbol_monitor_4
                                    lv_obj_t *obj = lv_img_create(parent_obj);
                                    objects.simbol_monitor_4 = obj;
                                    lv_obj_set_pos(obj, 13, 46);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_img_set_src(obj, &img_icons8_sand_100);
                                    lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                                }
                                {
                                    // teks_elektrofenton_1
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_elektrofenton_1 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "ADSORPSI");
                                }
                            }
                        }
                        {
                            // button_menu_spektrofotometri
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.button_menu_spektrofotometri = obj;
                            lv_obj_set_pos(obj, 50, 10);
                            lv_obj_set_size(obj, 175, 223);
                            lv_obj_add_event_cb(obj, action_tambahkan_spektrofotometri_dan_kembali_ke_menu_penjadwalan_proses, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // simbol_monitor_5
                                    lv_obj_t *obj = lv_img_create(parent_obj);
                                    objects.simbol_monitor_5 = obj;
                                    lv_obj_set_pos(obj, 18, 37);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_img_set_src(obj, &img_icons8_lens_100);
                                    lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                                }
                                {
                                    // teks_spektrofotometer
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_spektrofotometer = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "SPEKTROFOTOMETER");
                                }
                            }
                        }
                    }
                }
                {
                    // button_kembali
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_kembali = obj;
                    lv_obj_set_pos(obj, 28, 46);
                    lv_obj_set_size(obj, 77, 53);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_kembali
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_kembali = obj;
                            lv_obj_set_pos(obj, -13, -16);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_return_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_1 = obj;
            lv_obj_set_pos(obj, 271, -48);
            lv_obj_set_size(obj, 363, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_1 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "PENJADWALAN PROSES");
                }
            }
        }
        {
            // container_menu_11
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_11 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_25
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_25 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_29
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_29 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_26
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_26 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_30
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_30 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_7
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_7 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_15
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_15 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_27
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_27 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_31
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_31 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_28
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_28 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_32
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_32 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_menu_penjadwalan_proses_pilih();
}

void tick_screen_menu_penjadwalan_proses_pilih() {
}

void create_screen_menu_penjadwalan_proses_input_elektrofenton() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_penjadwalan_proses_input_elektrofenton = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff5f5f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content_2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_2 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // container_pilihan_proses_1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.container_pilihan_proses_1 = obj;
                    lv_obj_set_pos(obj, 28, 111);
                    lv_obj_set_size(obj, 640, 348);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_SPACE_EVENLY, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // container_aturmagnetic_stirrer_1
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_aturmagnetic_stirrer_1 = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_kecepatan_magnetic_stirrer_1
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_kecepatan_magnetic_stirrer_1 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Kecepatan Magnetic Stirrer");
                                }
                                {
                                    // textarea_input_magnetic_stirrer_speed
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_magnetic_stirrer_speed = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 150, 35);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "700");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_rpm, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_rpm_1
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_rpm_1 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "RPM");
                                }
                            }
                        }
                        {
                            // container_aturmagnetic_stirrer_2
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_aturmagnetic_stirrer_2 = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_suhu
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_suhu = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Suhu Reaktor");
                                }
                                {
                                    // textarea_input_suhu
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_suhu = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 150, 35);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "25");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_suhu, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_celcius
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_celcius = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Celcius");
                                }
                            }
                        }
                        {
                            // container_arus_listrik
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_arus_listrik = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_arus_listrik
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_arus_listrik = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Arus Listrik");
                                }
                                {
                                    // textarea_input_arus
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_arus = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 150, 35);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "67");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_arus_listrik, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_celcius_1
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_celcius_1 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "mA");
                                }
                            }
                        }
                        {
                            // container_durasi
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_durasi = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_durasi
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_durasi = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Durasi");
                                }
                                {
                                    // textarea_input_jam
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_jam = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 94, 42);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "Jam");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_jam, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_durasi_1
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_durasi_1 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 1, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, ":");
                                }
                                {
                                    // textarea_input_menit
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_menit = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 96, 42);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "Menit");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_menit, LV_EVENT_FOCUSED, (void *)0);
                                }
                            }
                        }
                        {
                            // container_airflow
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_airflow = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_airflow
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_airflow = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Airflow level");
                                }
                                {
                                    // roller_input_level_airflow
                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                    objects.roller_input_level_airflow = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 80, 100);
                                    lv_roller_set_options(obj, "0\n1\n2\n3", LV_ROLLER_MODE_NORMAL);
                                }
                            }
                        }
                        {
                            // container_ukur_degradasi
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_ukur_degradasi = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_ukur_degradasi
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_ukur_degradasi = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Ukur degradasi");
                                }
                                {
                                    lv_obj_t *obj = lv_switch_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 80, 25);
                                }
                            }
                        }
                    }
                }
                {
                    // button_kembali_1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_kembali_1 = obj;
                    lv_obj_set_pos(obj, 28, 46);
                    lv_obj_set_size(obj, 77, 53);
                    lv_obj_add_event_cb(obj, action_kembali_ke_menu_penjadwalan_proses_pilih, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_kembali_1
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_kembali_1 = obj;
                            lv_obj_set_pos(obj, -13, -15);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_return_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                    }
                }
                {
                    // teks_keterangan_input_parameter_elektrofenton
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_input_parameter_elektrofenton = obj;
                    lv_obj_set_pos(obj, -11, -156);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "ELEKTRO FENTON");
                }
                {
                    // button_kembali_2
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_kembali_2 = obj;
                    lv_obj_set_pos(obj, 567, 47);
                    lv_obj_set_size(obj, 77, 53);
                    lv_obj_add_event_cb(obj, action_tambahkan_jadwal_elektrofenton, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff21f354), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_kembali_2
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_kembali_2 = obj;
                            lv_obj_set_pos(obj, -6, -10);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_add_50);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_2 = obj;
            lv_obj_set_pos(obj, 271, -48);
            lv_obj_set_size(obj, 363, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_2 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "PENJADWALAN PROSES");
                }
            }
        }
        {
            // keyboard_elektrofenton
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard_elektrofenton = obj;
            lv_obj_set_pos(obj, 576, 206);
            lv_obj_set_size(obj, 224, 238);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_event_cb(obj, action_sembunyikan_numpad, LV_EVENT_READY, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // teks_peringatan1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1 = obj;
            lv_obj_set_pos(obj, 501, 154);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "(0, 100-1200) RPM");
        }
        {
            // teks_peringatan1_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_1 = obj;
            lv_obj_set_pos(obj, 505, 215);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "(25 - 40)");
        }
        {
            // teks_peringatan1_2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_2 = obj;
            lv_obj_set_pos(obj, 502, 274);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "(0 - 220)");
        }
        {
            // teks_peringatan1_3
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_3 = obj;
            lv_obj_set_pos(obj, 349, 291);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "MAX 2");
        }
        {
            // teks_peringatan1_4
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_4 = obj;
            lv_obj_set_pos(obj, 466, 289);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "MAX 59");
        }
        {
            // messagebox_peringatan_elektrofenton
            lv_obj_t *obj = lv_msgbox_create(parent_obj, "", "", 0, true);
            objects.messagebox_peringatan_elektrofenton = obj;
            lv_obj_set_pos(obj, 275, 8);
            lv_obj_set_size(obj, 355, 100);
            lv_obj_add_event_cb(obj, action_tutup_pesan_error_input_elektrofenton, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff7b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xffa69e9e), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pesan_peringatan
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pesan_peringatan = obj;
                    lv_obj_set_pos(obj, 485, -192);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "PERINGATAN:\n\n\n");
                }
            }
        }
        {
            // container_menu_12
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_12 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_29
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_29 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_33
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_33 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_30
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_30 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_34
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_34 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_8
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_8 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_16
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_16 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_31
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_31 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_35
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_35 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_32
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_32 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_36
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_36 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 372, 398);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_scroll);
            lv_img_set_zoom(obj, 200);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 372, 344);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_scroll);
            lv_img_set_zoom(obj, 200);
            lv_img_set_angle(obj, 1800);
        }
    }
    lv_keyboard_set_textarea(objects.keyboard_elektrofenton, objects.textarea_input_arus);
    
    tick_screen_menu_penjadwalan_proses_input_elektrofenton();
}

void tick_screen_menu_penjadwalan_proses_input_elektrofenton() {
}

void create_screen_menu_penjadwalan_proses_input_adsorpsi() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_penjadwalan_proses_input_adsorpsi = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content_3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_3 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_tambah
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_tambah = obj;
                    lv_obj_set_pos(obj, 575, 106);
                    lv_obj_set_size(obj, 62, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Tambah");
                }
                {
                    // container_pilihan_proses_2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.container_pilihan_proses_2 = obj;
                    lv_obj_set_pos(obj, 28, 111);
                    lv_obj_set_size(obj, 640, 348);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_SPACE_EVENLY, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // container_aturmagnetic_stirrer_3
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_aturmagnetic_stirrer_3 = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_kecepatan_magnetic_stirrer_2
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_kecepatan_magnetic_stirrer_2 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Kecepatan Magnetic Stirrer");
                                }
                                {
                                    // textarea_input_magnetic_stirrer_speed_1
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_magnetic_stirrer_speed_1 = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 150, 35);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "67");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_rpm, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_rpm_2
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_rpm_2 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "RPM");
                                }
                            }
                        }
                        {
                            // container_aturmagnetic_stirrer_4
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_aturmagnetic_stirrer_4 = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_suhu_1
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_suhu_1 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Suhu Reaktor");
                                }
                                {
                                    // textarea_input_suhu_1
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_suhu_1 = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 150, 35);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "67");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_suhu, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_celcius_2
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_celcius_2 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Celcius");
                                }
                            }
                        }
                        {
                            // container_durasi_1
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_durasi_1 = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_durasi_2
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_durasi_2 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Durasi");
                                }
                                {
                                    // textarea_input_jam_1
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_jam_1 = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 94, 42);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "Jam");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_jam, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_durasi_3
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_durasi_3 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 1, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, ":");
                                }
                                {
                                    // textarea_input_menit_1
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_menit_1 = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 96, 42);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "Menit");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_menit, LV_EVENT_FOCUSED, (void *)0);
                                }
                            }
                        }
                        {
                            // container_ukur_degradasi_1
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_ukur_degradasi_1 = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_ukur_degradasi_1
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_ukur_degradasi_1 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Ukur degradasi");
                                }
                                {
                                    lv_obj_t *obj = lv_switch_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 80, 25);
                                }
                            }
                        }
                    }
                }
                {
                    // button_kembali_3
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_kembali_3 = obj;
                    lv_obj_set_pos(obj, 28, 46);
                    lv_obj_set_size(obj, 77, 53);
                    lv_obj_add_event_cb(obj, action_kembali_ke_menu_penjadwalan_proses_pilih, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_kembali_3
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_kembali_3 = obj;
                            lv_obj_set_pos(obj, -13, -15);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_return_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                    }
                }
                {
                    // teks_keterangan_input_parameter_elektrofenton_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_input_parameter_elektrofenton_1 = obj;
                    lv_obj_set_pos(obj, -11, -156);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "ADSORPSI");
                }
                {
                    // button_kembali_4
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_kembali_4 = obj;
                    lv_obj_set_pos(obj, 567, 47);
                    lv_obj_set_size(obj, 77, 53);
                    lv_obj_add_event_cb(obj, action_tambahkan_jadwal_adsorpsi, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff21f354), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_kembali_4
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_kembali_4 = obj;
                            lv_obj_set_pos(obj, -6, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_add_50);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_3 = obj;
            lv_obj_set_pos(obj, 271, -48);
            lv_obj_set_size(obj, 363, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_3 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "PENJADWALAN PROSES");
                }
            }
        }
        {
            // keyboard_adsorpsi
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard_adsorpsi = obj;
            lv_obj_set_pos(obj, 576, 206);
            lv_obj_set_size(obj, 224, 238);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_event_cb(obj, action_sembunyikan_numpad, LV_EVENT_READY, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // teks_peringatan1_5
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_5 = obj;
            lv_obj_set_pos(obj, 501, 178);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "(0,100-1200)");
        }
        {
            // teks_peringatan1_6
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_6 = obj;
            lv_obj_set_pos(obj, 510, 261);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "(25 - 40)");
        }
        {
            // teks_peringatan1_7
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_7 = obj;
            lv_obj_set_pos(obj, 351, 358);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "MAX 2");
        }
        {
            // teks_peringatan1_8
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_8 = obj;
            lv_obj_set_pos(obj, 466, 360);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "MAX 59");
        }
        {
            // messagebox_peringatan_adsorpsi
            lv_obj_t *obj = lv_msgbox_create(parent_obj, "", "", 0, true);
            objects.messagebox_peringatan_adsorpsi = obj;
            lv_obj_set_pos(obj, 275, 8);
            lv_obj_set_size(obj, 355, 100);
            lv_obj_add_event_cb(obj, action_tutup_pesan_error_input_adsorpsi, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff7b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xffa69e9e), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pesan_peringatan_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pesan_peringatan_1 = obj;
                    lv_obj_set_pos(obj, 485, -192);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "PERINGATAN:\n\n\n");
                }
            }
        }
        {
            // container_menu_13
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_13 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_33
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_33 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_37
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_37 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_34
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_34 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_38
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_38 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_9
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_9 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_17
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_17 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_35
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_35 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_39
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_39 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_36
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_36 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_40
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_40 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
    }
    lv_keyboard_set_textarea(objects.keyboard_adsorpsi, objects.textarea_input_arus);
    
    tick_screen_menu_penjadwalan_proses_input_adsorpsi();
}

void tick_screen_menu_penjadwalan_proses_input_adsorpsi() {
}

void create_screen_menu_monitor() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_monitor = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content_4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_4 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 44, 51);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "PROSES:");
                }
                {
                    // button_force_stop
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_force_stop = obj;
                    lv_obj_set_pos(obj, 529, 358);
                    lv_obj_set_size(obj, 138, 72);
                    lv_obj_add_event_cb(obj, action_lakukan_force_stop, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff32121), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_stop
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_stop = obj;
                            lv_obj_set_pos(obj, 49, -8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_stop);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            // teks_stop
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.teks_stop = obj;
                            lv_obj_set_pos(obj, 3, -10);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "STOP");
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_4 = obj;
            lv_obj_set_pos(obj, 271, -48);
            lv_obj_set_size(obj, 363, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_4
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_4 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "PENJADWALAN PROSES");
                }
            }
        }
        {
            // container_menu_14
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_14 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_37
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_37 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_41
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_41 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_38
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_38 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_42
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_42 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_10
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_10 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_18
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_18 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_39
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_39 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_43
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_43 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_40
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_40 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_44
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_44 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_menu_monitor();
}

void tick_screen_menu_monitor() {
}

void create_screen_menu_inisialisasi() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_inisialisasi = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_menu_5
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_5 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_3
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_3 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_7
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_7 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_1 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_5
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_5 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_1 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_9
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_9 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_2
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_2 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_6
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_6 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_4
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_4 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_8
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_8 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
        {
            // container_content_5
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_5 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_start_jadwal_1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_start_jadwal_1 = obj;
                    lv_obj_set_pos(obj, 574, 37);
                    lv_obj_set_size(obj, 94, 88);
                    lv_obj_add_event_cb(obj, action_mulai_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_start_jadwal_1
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_start_jadwal_1 = obj;
                            lv_obj_set_pos(obj, -5, -5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_play_64__1_);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            // teks_start_1
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.teks_start_1 = obj;
                            lv_obj_set_pos(obj, -16, 5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "INISIALISASI");
                        }
                    }
                }
                {
                    // container_pilihan_proses_3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.container_pilihan_proses_3 = obj;
                    lv_obj_set_pos(obj, 28, 51);
                    lv_obj_set_size(obj, 538, 215);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // container_input_volume_sampel
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_input_volume_sampel = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_input_volume_sampel
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_input_volume_sampel = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Input Volume Sampel");
                                }
                                {
                                    // textarea_input_volume
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_volume = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 150, 35);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "67");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_rpm, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_ml
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_ml = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "mL");
                                }
                            }
                        }
                        {
                            // container_ph_target
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_ph_target = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_ph_taget
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_ph_taget = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Volume H2SO4");
                                }
                                {
                                    // textarea_input_ph
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_ph = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 150, 35);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "6.7");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_suhu, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // teks_ml_1
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_ml_1 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "mL");
                                }
                            }
                        }
                        {
                            // container_konsentrasi_fe
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_konsentrasi_fe = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_konsentrasi_fe
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_konsentrasi_fe = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Fe2+");
                                }
                                {
                                    // textarea_input_arus_1
                                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                                    objects.textarea_input_arus_1 = obj;
                                    lv_obj_set_pos(obj, 373, -48);
                                    lv_obj_set_size(obj, 150, 35);
                                    lv_textarea_set_max_length(obj, 128);
                                    lv_textarea_set_placeholder_text(obj, "6.7");
                                    lv_textarea_set_one_line(obj, true);
                                    lv_textarea_set_password_mode(obj, false);
                                    lv_obj_add_event_cb(obj, action_tampil_numpad_arus_listrik, LV_EVENT_FOCUSED, (void *)0);
                                }
                                {
                                    // fe_type_scrollbar
                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                    objects.fe_type_scrollbar = obj;
                                    lv_obj_set_pos(obj, 535, 246);
                                    lv_obj_set_size(obj, 80, 100);
                                    lv_roller_set_options(obj, "mm\nml", LV_ROLLER_MODE_NORMAL);
                                }
                            }
                        }
                        {
                            // container_kalibrasi_spektro
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_kalibrasi_spektro = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_preflill_pompa
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_preflill_pompa = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Prefill Pompa");
                                }
                                {
                                    // switch_prefill_pompa
                                    lv_obj_t *obj = lv_switch_create(parent_obj);
                                    objects.switch_prefill_pompa = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 80, 25);
                                }
                            }
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_5
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_5 = obj;
            lv_obj_set_pos(obj, 271, -48);
            lv_obj_set_size(obj, 363, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_5 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "INSIALISASI");
                }
            }
        }
        {
            // keyboard_elektrofenton_1
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard_elektrofenton_1 = obj;
            lv_obj_set_pos(obj, 559, 223);
            lv_obj_set_size(obj, 224, 238);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_event_cb(obj, action_sembunyikan_numpad, LV_EVENT_READY, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // teks_peringatan1_9
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_9 = obj;
            lv_obj_set_pos(obj, 505, 94);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "(50-250)");
        }
        {
            // teks_peringatan1_10
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_10 = obj;
            lv_obj_set_pos(obj, 347, 220);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "(Fe2+ yang ditambahkan ke Reaktor)");
        }
        {
            // teks_peringatan1_11
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_peringatan1_11 = obj;
            lv_obj_set_pos(obj, 504, 155);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffed0b0b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Volume  dari beaker H2SO4");
        }
        {
            // messagebox_peringatan_input_inisialisasi
            lv_obj_t *obj = lv_msgbox_create(parent_obj, "", "", 0, true);
            objects.messagebox_peringatan_input_inisialisasi = obj;
            lv_obj_set_pos(obj, 275, 8);
            lv_obj_set_size(obj, 355, 100);
            lv_obj_add_event_cb(obj, action_tutup_pesan_error_input_inisialisasi, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff7b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xffa69e9e), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pesan_peringatan_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pesan_peringatan_2 = obj;
                    lv_obj_set_pos(obj, 485, -192);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "PERINGATAN:\n\n\n");
                }
            }
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 535, 220);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_scroll);
            lv_img_set_zoom(obj, 200);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 533, 161);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_scroll);
            lv_img_set_zoom(obj, 200);
            lv_img_set_angle(obj, 1800);
        }
    }
    lv_keyboard_set_textarea(objects.keyboard_elektrofenton_1, objects.textarea_input_arus);
    
    tick_screen_menu_inisialisasi();
}

void tick_screen_menu_inisialisasi() {
}

void create_screen_menu_melakukan_inisialisasi() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_melakukan_inisialisasi = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content_6
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_6 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // status_inisialisasi
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.status_inisialisasi = obj;
                    lv_obj_set_pos(obj, 26, 57);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Melakukan Inisialisasi...");
                }
                {
                    // container_pilihan_proses_4
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.container_pilihan_proses_4 = obj;
                    lv_obj_set_pos(obj, 26, 107);
                    lv_obj_set_size(obj, 538, 292);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // container_progress_mengisi_reaktor
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_progress_mengisi_reaktor = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_progress_mengisi_reaktor
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_progress_mengisi_reaktor = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Mengisi Reaktor (0%)");
                                }
                                {
                                    // progress_bar_mengisi_reaktor
                                    lv_obj_t *obj = lv_bar_create(parent_obj);
                                    objects.progress_bar_mengisi_reaktor = obj;
                                    lv_obj_set_pos(obj, 0, 27);
                                    lv_obj_set_size(obj, 319, 17);
                                    lv_bar_set_value(obj, 25, LV_ANIM_OFF);
                                }
                            }
                        }
                        {
                            // container_mengatur_ph
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_mengatur_ph = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_mengatur_ph
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_mengatur_ph = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Mengatur pH");
                                }
                                {
                                    // teks_status_mengatur_ph
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_status_mengatur_ph = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffdc1313), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Tidak diatur");
                                }
                            }
                        }
                        {
                            // container_mengatur_konsentrasi_fe2+
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_mengatur_konsentrasi_fe2_ = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_mengatur_konsentrasi_fe2
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_mengatur_konsentrasi_fe2 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Mengatur Konsentrasi Fe2+");
                                }
                                {
                                    // teks_status_konsentrasi_fe2
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_status_konsentrasi_fe2 = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff13dc6c), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Selesai");
                                }
                            }
                        }
                        {
                            // container_status_kalibrasi_spektro
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_status_kalibrasi_spektro = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_kalibrasi_spektro_bagaimana
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_kalibrasi_spektro_bagaimana = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Status Kalibrasi Spektro");
                                }
                                {
                                    // teks_status_kalibrasi_spektro
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_status_kalibrasi_spektro = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Mengkalibrasikan...");
                                }
                            }
                        }
                        {
                            // container_status_prefilling
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.container_status_prefilling = obj;
                            lv_obj_set_pos(obj, 0, -3295);
                            lv_obj_set_size(obj, 640, 47);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // teks_prefilling
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_prefilling = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Status Prefill Pompa");
                                }
                                {
                                    // teks_status_prefilling
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.teks_status_prefilling = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, 200, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Prefilling...");
                                }
                            }
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_6
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_6 = obj;
            lv_obj_set_pos(obj, 271, -48);
            lv_obj_set_size(obj, 363, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_6 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "INSIALISASI");
                }
            }
        }
        {
            // keyboard_elektrofenton_2
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard_elektrofenton_2 = obj;
            lv_obj_set_pos(obj, 567, 164);
            lv_obj_set_size(obj, 224, 238);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_event_cb(obj, action_sembunyikan_numpad, LV_EVENT_READY, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // container_menu_6
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_6 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_5
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_5 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_9
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_9 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_6
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_6 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_10
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_10 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_2
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_2 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_10
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_10 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_7
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_7 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_11
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_11 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_8
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_8 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_12
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_12 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
        {
            // pindah_ke_jadwal
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.pindah_ke_jadwal = obj;
            lv_obj_set_pos(obj, 602, 373);
            lv_obj_set_size(obj, 139, 69);
            lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // status_inisialisasi_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.status_inisialisasi_1 = obj;
                    lv_obj_set_pos(obj, 15, 9);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "NEXT");
                }
            }
        }
    }
    lv_keyboard_set_textarea(objects.keyboard_elektrofenton_2, objects.textarea_input_arus);
    
    tick_screen_menu_melakukan_inisialisasi();
}

void tick_screen_menu_melakukan_inisialisasi() {
}

void create_screen_menu_spektro_degradasi_warna() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_spektro_degradasi_warna = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
    lv_obj_set_style_layout(obj, LV_LAYOUT_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content_7
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_7 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_menu_degradasi_warna
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_menu_degradasi_warna = obj;
                    lv_obj_set_pos(obj, 274, 46);
                    lv_obj_set_size(obj, 139, 40);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff40b91d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Degradasi Warna");
                        }
                    }
                }
                {
                    // button_menu_spektrum_warna
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_menu_spektrum_warna = obj;
                    lv_obj_set_pos(obj, 78, 48);
                    lv_obj_set_size(obj, 139, 40);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro_spektrum_warna, LV_EVENT_PRESSED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektrum Warna");
                        }
                    }
                }
                {
                    // button_simpan_grafik_1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_simpan_grafik_1 = obj;
                    lv_obj_set_pos(obj, 462, 46);
                    lv_obj_set_size(obj, 170, 40);
                    lv_obj_add_event_cb(obj, action_simpan_data_spektro_ke_sdcard, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1b9dec), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_simpan_data_spektro_ke_sdcard, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Simpan Data Spektro");
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_7
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_7 = obj;
            lv_obj_set_pos(obj, 170, -48);
            lv_obj_set_size(obj, 557, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_7
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_7 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "GRAFIK PENGUKURAN SPEKTROFOTOMETRI");
                }
            }
        }
        {
            // chart_degradasi_warna
            lv_obj_t *obj = lv_chart_create(parent_obj);
            objects.chart_degradasi_warna = obj;
            lv_obj_set_pos(obj, 170, 104);
            lv_obj_set_size(obj, 599, 269);
        }
        {
            // list_grafik_tersedia
            lv_obj_t *obj = lv_list_create(parent_obj);
            objects.list_grafik_tersedia = obj;
            lv_obj_set_pos(obj, 134, 425);
            lv_obj_set_size(obj, 635, 45);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // container_menu_7
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_7 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_9
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_9 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_13
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_13 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_10
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_10 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_14
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_14 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_3
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_3 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_11
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_11 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_11
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_11 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_15
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_15 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_12
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_12 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_16
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_16 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
        {
            // messagebox_status_penyimpanan_2
            lv_obj_t *obj = lv_msgbox_create(parent_obj, "", "", 0, true);
            objects.messagebox_status_penyimpanan_2 = obj;
            lv_obj_set_pos(obj, 275, 10);
            lv_obj_set_size(obj, 355, 100);
            lv_obj_add_event_cb(obj, action_tutup_pesan_status_penyimpanan_data_spektro, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb6f7c4), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xffa69e9e), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pesan_peringatan_6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pesan_peringatan_6 = obj;
                    lv_obj_set_pos(obj, 249, 256);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "PERINGATAN:\n\n\n");
                }
            }
        }
        {
            // teks_menit
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_menit = obj;
            lv_obj_set_pos(obj, 448, 409);
            lv_obj_set_size(obj, 44, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "menit");
        }
        {
            // teks_degradasi
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_degradasi = obj;
            lv_obj_set_pos(obj, 105, 91);
            lv_obj_set_size(obj, 97, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "%degradasi");
        }
        {
            // pesan_peringatan_3
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pesan_peringatan_3 = obj;
            lv_obj_set_pos(obj, 485, -192);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "PERINGATAN:\n\n\n");
        }
    }
    
    tick_screen_menu_spektro_degradasi_warna();
}

void tick_screen_menu_spektro_degradasi_warna() {
}

void create_screen_menu_spektro_spektrum_warna() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_spektro_spektrum_warna = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_simpan_data_spektro_ke_sdcard, LV_EVENT_RELEASED, (void *)0);
    lv_obj_set_style_layout(obj, LV_LAYOUT_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content_8
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_8 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_menu_degradasi_warna_1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_menu_degradasi_warna_1 = obj;
                    lv_obj_set_pos(obj, 274, 46);
                    lv_obj_set_size(obj, 139, 40);
                    lv_obj_add_event_cb(obj, action_ke_menu_degradasi_warna, LV_EVENT_RELEASED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Degradasi Warna");
                        }
                    }
                }
                {
                    // button_menu_spektrum_warna_1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_menu_spektrum_warna_1 = obj;
                    lv_obj_set_pos(obj, 78, 48);
                    lv_obj_set_size(obj, 139, 40);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff40b91d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektrum Warna");
                        }
                    }
                }
                {
                    // button_simpan_grafik
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_simpan_grafik = obj;
                    lv_obj_set_pos(obj, 452, 46);
                    lv_obj_set_size(obj, 170, 40);
                    lv_obj_add_event_cb(obj, action_simpan_data_spektro_ke_sdcard, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1b9dec), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_simpan_data_spektro_ke_sdcard, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Simpan Data Spektro");
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_8
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_8 = obj;
            lv_obj_set_pos(obj, 170, -48);
            lv_obj_set_size(obj, 557, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_8
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_8 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "GRAFIK PENGUKURAN SPEKTROFOTOMETRI");
                }
            }
        }
        {
            // chart_degradasi_warna_1
            lv_obj_t *obj = lv_chart_create(parent_obj);
            objects.chart_degradasi_warna_1 = obj;
            lv_obj_set_pos(obj, 170, 104);
            lv_obj_set_size(obj, 599, 269);
        }
        {
            // list_grafik_tersedia_1
            lv_obj_t *obj = lv_list_create(parent_obj);
            objects.list_grafik_tersedia_1 = obj;
            lv_obj_set_pos(obj, 136, 425);
            lv_obj_set_size(obj, 633, 45);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // container_menu_8
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_8 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_13
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_13 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_17
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_17 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_14
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_14 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_18
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_18 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_4
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_4 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_12
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_12 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_15
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_15 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_19
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_19 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_16
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_16 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_20
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_20 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
        {
            // messagebox_status_penyimpanan
            lv_obj_t *obj = lv_msgbox_create(parent_obj, "", "", 0, true);
            objects.messagebox_status_penyimpanan = obj;
            lv_obj_set_pos(obj, 275, 8);
            lv_obj_set_size(obj, 355, 100);
            lv_obj_add_event_cb(obj, action_tutup_pesan_status_penyimpanan_data_spektro, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb6f7c4), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xffa69e9e), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pesan_peringatan_5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pesan_peringatan_5 = obj;
                    lv_obj_set_pos(obj, 249, 256);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "PERINGATAN:\n\n\n");
                }
            }
        }
        {
            // teks_nm
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_nm = obj;
            lv_obj_set_pos(obj, 455, 410);
            lv_obj_set_size(obj, 30, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "nm");
        }
        {
            // teks_ads
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.teks_ads = obj;
            lv_obj_set_pos(obj, 105, 91);
            lv_obj_set_size(obj, 97, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "adsorbansi");
        }
    }
    
    tick_screen_menu_spektro_spektrum_warna();
}

void tick_screen_menu_spektro_spektrum_warna() {
}

void create_screen_menu_flushing() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.menu_flushing = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
    lv_obj_set_style_layout(obj, LV_LAYOUT_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container_content_9
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_content_9 = obj;
            lv_obj_set_pos(obj, 105, 11);
            lv_obj_set_size(obj, 686, 459);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // container_progress_flushing
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.container_progress_flushing = obj;
                    lv_obj_set_pos(obj, 14, 314);
                    lv_obj_set_size(obj, 662, 68);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Progress Flushing (0%)");
                        }
                        {
                            // progress_bar_proses_flushing
                            lv_obj_t *obj = lv_bar_create(parent_obj);
                            objects.progress_bar_proses_flushing = obj;
                            lv_obj_set_pos(obj, 189, 354);
                            lv_obj_set_size(obj, 538, 19);
                            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
                        }
                    }
                }
                {
                    // teks_keterangan_menu_10
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_10 = obj;
                    lv_obj_set_pos(obj, 3, 49);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Lakukan Flushing");
                }
                {
                    // button_start_jadwal_2
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_start_jadwal_2 = obj;
                    lv_obj_set_pos(obj, 298, 168);
                    lv_obj_set_size(obj, 94, 88);
                    lv_obj_add_event_cb(obj, action_tampilkan_message_konfirmasi_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_start_jadwal_2
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_start_jadwal_2 = obj;
                            lv_obj_set_pos(obj, -5, 1);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_play_64__1_);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                    }
                }
            }
        }
        {
            // container_keterangan_menu_9
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_keterangan_menu_9 = obj;
            lv_obj_set_pos(obj, 170, -48);
            lv_obj_set_size(obj, 557, 96);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_keterangan_menu_9
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_keterangan_menu_9 = obj;
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "FLUSHING");
                }
            }
        }
        {
            // messagebox_konfirmasi_flushing
            lv_obj_t *obj = lv_msgbox_create(parent_obj, "", "", 0, true);
            objects.messagebox_konfirmasi_flushing = obj;
            lv_obj_set_pos(obj, 262, 103);
            lv_obj_set_size(obj, 378, 216);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_SPACE_EVENLY, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_konfirmasi_flushing
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_konfirmasi_flushing = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Perubahan akan hilang\nApakah tetap flushing?");
                }
                {
                    // button_konfirmasi_flushing
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_konfirmasi_flushing = obj;
                    lv_obj_set_pos(obj, -147, 113);
                    lv_obj_set_size(obj, 134, 50);
                    lv_obj_add_event_cb(obj, action_lakukan_flushing, LV_EVENT_RELEASED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // teks_konfirmasi_flushing_dibutton
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.teks_konfirmasi_flushing_dibutton = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Lakukan Flushing");
                        }
                    }
                }
            }
        }
        {
            // container_menu_9
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container_menu_9 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 105, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_penjadwalan_proses_input_elektrofenton_17
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_17 = obj;
                    lv_obj_set_pos(obj, 187, -572);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_inisialisasi, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_21
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_21 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_plus_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inisialisasi");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_18
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_18 = obj;
                    lv_obj_set_pos(obj, 5, -463);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_penjadwalan_proses_utama, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_22
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_22 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_schedule_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Jadwal");
                        }
                    }
                }
                {
                    // button_monitor_proses_input_elektrofenton_5
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_monitor_proses_input_elektrofenton_5 = obj;
                    lv_obj_set_pos(obj, 5, -1138);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_monitor, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_monitor_13
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_monitor_13 = obj;
                            lv_obj_set_pos(obj, -1, -3);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_monitor_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Monitor");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_19
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_19 = obj;
                    lv_obj_set_pos(obj, 187, -530);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_spektro, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_23
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_23 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_graph_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Spektro");
                        }
                    }
                }
                {
                    // button_penjadwalan_proses_input_elektrofenton_20
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_penjadwalan_proses_input_elektrofenton_20 = obj;
                    lv_obj_set_pos(obj, 187, -615);
                    lv_obj_set_size(obj, 100, 74);
                    lv_obj_add_event_cb(obj, action_pindah_ke_menu_flushing, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_shadow_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0c329c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // simbol_jadwal_24
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.simbol_jadwal_24 = obj;
                            lv_obj_set_pos(obj, -1, -11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_icons8_bowel_flush_64);
                            lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "flushing");
                        }
                    }
                }
            }
        }
        {
            // message_flushing_sukses
            lv_obj_t *obj = lv_msgbox_create(parent_obj, "", "", 0, true);
            objects.message_flushing_sukses = obj;
            lv_obj_set_pos(obj, 351, 7);
            lv_obj_set_size(obj, 214, 127);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff9cf28f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW_WRAP, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // teks_flushing_sukses
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.teks_flushing_sukses = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Flushing Sukses!");
                }
            }
        }
    }
    
    tick_screen_menu_flushing();
}

void tick_screen_menu_flushing() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_menu_intro,
    tick_screen_menu_penjadwalan_proses_utama,
    tick_screen_menu_penjadwalan_proses_pilih,
    tick_screen_menu_penjadwalan_proses_input_elektrofenton,
    tick_screen_menu_penjadwalan_proses_input_adsorpsi,
    tick_screen_menu_monitor,
    tick_screen_menu_inisialisasi,
    tick_screen_menu_melakukan_inisialisasi,
    tick_screen_menu_spektro_degradasi_warna,
    tick_screen_menu_spektro_spektrum_warna,
    tick_screen_menu_flushing,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_menu_intro();
    create_screen_menu_penjadwalan_proses_utama();
    create_screen_menu_penjadwalan_proses_pilih();
    create_screen_menu_penjadwalan_proses_input_elektrofenton();
    create_screen_menu_penjadwalan_proses_input_adsorpsi();
    create_screen_menu_monitor();
    create_screen_menu_inisialisasi();
    create_screen_menu_melakukan_inisialisasi();
    create_screen_menu_spektro_degradasi_warna();
    create_screen_menu_spektro_spektrum_warna();
    create_screen_menu_flushing();
}