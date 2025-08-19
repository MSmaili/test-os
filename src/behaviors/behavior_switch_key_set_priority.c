#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/behaviors/switch_key.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

int zmk_switch_key_set_priority(uint8_t priority) {
    if (priority > 1) {
        LOG_ERR("Invalid priority: %d (must be 0 or 1)", priority);
        return -EINVAL;
    }

    zmk_switch_key_priority = priority;
    LOG_DBG("Priority set to: %d", priority);
    return 0;
}

uint8_t zmk_switch_key_get_priority(void) {
    return zmk_switch_key_priority;
}

static int behavior_switch_key_set_init(const struct device *dev) {
    return 0;
}

static int on_set_binding_pressed(struct zmk_behavior_binding *binding,
                                struct zmk_behavior_binding_event event) {
    uint8_t priority = binding->param1;

    if (priority > 1) {
        LOG_ERR("Invalid priority in binding: %d", priority);
        return -EINVAL;
    }

    return zmk_switch_key_set_priority(priority);
}

static int on_set_binding_released(struct zmk_behavior_binding *binding,
                                 struct zmk_behavior_binding_event event) {
    // No action on release
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct zmk_behavior_driver_api behavior_switch_key_set_driver_api = {
    .binding_pressed = on_set_binding_pressed,
    .binding_released = on_set_binding_released,
};

BEHAVIOR_DT_DEFINE(sw_set, behavior_switch_key_set_init, NULL, NULL, NULL,
                   POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                   &behavior_switch_key_set_driver_api);
