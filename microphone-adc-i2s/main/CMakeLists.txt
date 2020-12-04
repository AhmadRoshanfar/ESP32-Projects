idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES sd_card microphone wave freertos
                     #REQUIRES lvgl_esp32_drivers lvgl lv_examples lvgl_tft lvgl_touch
                     )
