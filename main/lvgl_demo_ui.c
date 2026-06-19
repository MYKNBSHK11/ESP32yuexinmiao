#include "lvgl.h"

// LVGL GIF support - enabled via sdkconfig (CONFIG_LV_USE_GIF=y)

void example_lvgl_demo_ui(lv_display_t *disp)
{
    lv_obj_t *scr = lv_display_get_screen_active(disp);
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    // Step 1: Convert your GIF to C array using https://lvgl.io/tools/imageconverter
    //         Output format: "Raw G-I-F file"
    // Step 2: Put the generated .c file in main/ and #include it here
    // Step 3: Uncomment the line below and use your gif variable name

    // extern const lv_image_dsc_t my_gif;   // <-- replace "my_gif" with your variable name
    // lv_obj_t *gif = lv_gif_create(scr);
    // lv_gif_set_src(gif, &my_gif);
    // lv_obj_center(gif);

    // TODO: Replace the placeholder above with your GIF.
    // For now, show a hint text:
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label,
        "Put your GIF here!\n\n"
        "1. Go to lvgl.io/tools/imageconverter\n"
        "2. Upload your .gif file\n"
        "3. Output format: Raw G-I-F file\n"
        "4. Save as .c, put in main/\n"
        "5. #include it in this file\n"
        "6. Call lv_gif_create + lv_gif_set_src");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_center(label);
}
