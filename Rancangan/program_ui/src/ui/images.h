#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_icons8_monitor_64;
extern const lv_img_dsc_t img_icons8_play_64__1_;
extern const lv_img_dsc_t img_icons8_schedule_64;
extern const lv_img_dsc_t img_icons8_add_64;
extern const lv_img_dsc_t img_icons8_add_50;
extern const lv_img_dsc_t img_icons8_return_64;
extern const lv_img_dsc_t img_icons8_lens_100;
extern const lv_img_dsc_t img_icons8_sand_100;
extern const lv_img_dsc_t img_icons8_quick_mode_on_100;
extern const lv_img_dsc_t img_stop;
extern const lv_img_dsc_t img_icons8_bowel_flush_64;
extern const lv_img_dsc_t img_icons8_graph_64;
extern const lv_img_dsc_t img_icons8_plus_64;
extern const lv_img_dsc_t img_scroll;
extern const lv_img_dsc_t img_next;
extern const lv_img_dsc_t img_effiton_logo;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[16];

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/