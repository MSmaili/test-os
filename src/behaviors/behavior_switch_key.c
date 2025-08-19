#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/keymap.h>
#include <zmk/matrix.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/modifiers_state_changed.h>
#include <zmk/hid.h>
#include <zmk/behaviors/switch_key.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Global priority state
uint8_t zmk_switch_key_priority = 0;

static int behavior_switch_key_init(const struct device *dev) { return 0; }

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                   struct zmk_behavior_binding_event event) {
    // Extract keycodes from binding parameters
    uint16_t keycode1 = binding->param1;
    uint16_t keycode2 = binding->param2;

    // Select keycode based on current priority
    uint16_t selected_keycode = zmk_switch_key_priority ? keycode2 : keycode1;

    LOG_DBG("Switch key pressed: priority=%d, keycode1=0x%02X, keycode2=0x%02X, selected=0x%02X",
            zmk_switch_key_priority, keycode1, keycode2, selected_keycode);

    // Create new binding for the selected keycode
    struct zmk_behavior_binding new_binding = {
        .behavior_dev = "kp",
        .param1 = selected_keycode,
        .param2 = 0,
    };

    return zmk_behavior_invoke_binding(&new_binding, event, true);
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event) {
    // Extract keycodes from binding parameters
    uint16_t keycode1 = binding->param1;
    uint16_t keycode2 = binding->param2;

    // Select same keycode as when pressed
    uint16_t selected_keycode = zmk_switch_key_priority ? keycode2 : keycode1;

    LOG_DBG("Switch key released: priority=%d, selected=0x%02X",
            zmk_switch_key_priority, selected_keycode);

    // Create new binding for the selected keycode
    struct zmk_behavior_binding new_binding = {
        .behavior_dev = "kp",
        .param1 = selected_keycode,
        .param2 = 0,
    };

    return zmk_behavior_invoke_binding(&new_binding, event, false);
}

static const struct zmk_behavior_driver_api behavior_switch_key_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

BEHAVIOR_DT_DEFINE(sw_kp, behavior_switch_key_init, NULL, NULL, NULL,
                   POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                   &behavior_switch_key_driver_api);
