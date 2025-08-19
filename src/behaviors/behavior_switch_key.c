#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <zmk/endpoints.h>
#include <zmk/behaviors/switch_key.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Global priority state
uint8_t zmk_switch_key_priority = 0;

static int behavior_switch_key_init(const struct device *dev) { return 0; }

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                   struct zmk_behavior_binding_event event) {
    uint16_t keycode1 = binding->param1;
    uint16_t keycode2 = binding->param2;
    uint16_t selected_keycode = zmk_switch_key_priority ? keycode2 : keycode1;

    LOG_INF("SW_KP: pressed, priority=%d, keycode=0x%04X",
            zmk_switch_key_priority, selected_keycode);

    // Add key to HID report
    int ret = zmk_hid_keyboard_press(selected_keycode);
    if (ret < 0) {
        LOG_ERR("Failed to press key: %d", ret);
        return ret;
    }

    return zmk_endpoints_send_report(ZMK_ENDPOINT_USB);
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event) {
    uint16_t keycode1 = binding->param1;
    uint16_t keycode2 = binding->param2;
    uint16_t selected_keycode = zmk_switch_key_priority ? keycode2 : keycode1;

    LOG_INF("SW_KP: released, priority=%d, keycode=0x%04X",
            zmk_switch_key_priority, selected_keycode);

    // Remove key from HID report
    int ret = zmk_hid_keyboard_release(selected_keycode);
    if (ret < 0) {
        LOG_ERR("Failed to release key: %d", ret);
        return ret;
    }

    return zmk_endpoints_send_report(ZMK_ENDPOINT_USB);
}

static const struct zmk_behavior_driver_api behavior_switch_key_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

BEHAVIOR_DT_DEFINE(sw_kp, behavior_switch_key_init, NULL, NULL, NULL,
                   POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                   &behavior_switch_key_driver_api);
