#define DT_DRV_COMPAT zmk_behavior_switch_key_set_priority

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/keymap.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/modifiers_state_changed.h>
#include <zmk/keys.h>
#include <zmk/hid.h>
#include <zmk/keymap.h>
// Forward declarations - no need for separate header
// Forward declaration
int zmk_behavior_switch_key_set_priority(uint8_t priority);

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int behavior_switch_key_set_priority_init(const struct device *dev) {
    return 0;
}

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                   struct zmk_behavior_binding_event event) {
    // The parameter (binding->param1) should be 0 or 1
    uint8_t priority = binding->param1;

    LOG_DBG("Setting switch key priority to: %d", priority);

    int ret = zmk_behavior_switch_key_set_priority(priority);
    if (ret < 0) {
        LOG_ERR("Failed to set switch key priority: %d", ret);
        return ret;
    }

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event) {
    // No action needed on release
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct zmk_behavior_driver_api behavior_switch_key_set_priority_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

#define SET_PRIORITY_INST(n)                                                                      \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_switch_key_set_priority_init, NULL, NULL, NULL,         \
                           POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                      \
                           &behavior_switch_key_set_priority_driver_api);

DT_INST_FOREACH_STATUS_OKAY(SET_PRIORITY_INST)
