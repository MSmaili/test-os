#define DT_DRV_COMPAT zmk_behavior_switch_key

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <drivers/behavior.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// State stored in RAM only - no flash wear, resets on power cycle
static uint8_t switch_key_priority = 0;

int zmk_behavior_switch_key_get_priority(void) {
    return switch_key_priority;
}

int zmk_behavior_switch_key_set_priority(uint8_t priority) {
    if (priority > 1) {
        return -EINVAL;
    }
    switch_key_priority = priority;
    LOG_DBG("Switch key priority set to: %d", priority);
    return 0;
}

static int behavior_switch_key_init(const struct device *dev) {
    return 0;
}

static int on_switch_key_binding_pressed(struct zmk_behavior_binding *binding,
                                        struct zmk_behavior_binding_event event) {
    uint8_t priority = zmk_behavior_switch_key_get_priority();

    LOG_DBG("Switch key pressed, priority: %d, params: %d,%d", priority, binding->param1, binding->param2);

    // Select keycode based on priority: param1 for priority 0, param2 for priority 1
    uint32_t selected_keycode = (priority == 0) ? binding->param1 : binding->param2;

    // Create a new binding for the selected keycode (using kp behavior)
    struct zmk_behavior_binding selected_binding = {
        .behavior_dev = "kp",
        .param1 = selected_keycode,
        .param2 = 0,
    };

    return zmk_behavior_invoke_binding(&selected_binding, event, false);
}

static int on_switch_key_binding_released(struct zmk_behavior_binding *binding,
                                         struct zmk_behavior_binding_event event) {
    uint8_t priority = zmk_behavior_switch_key_get_priority();

    LOG_DBG("Switch key released, priority: %d, params: %d,%d", priority, binding->param1, binding->param2);

    // Select keycode based on priority: param1 for priority 0, param2 for priority 1
    uint32_t selected_keycode = (priority == 0) ? binding->param1 : binding->param2;

    // Create a new binding for the selected keycode (using kp behavior)
    struct zmk_behavior_binding selected_binding = {
        .behavior_dev = "kp",
        .param1 = selected_keycode,
        .param2 = 0,
    };

    return zmk_behavior_invoke_binding(&selected_binding, event, true);
}

static const struct behavior_driver_api behavior_switch_key_driver_api = {
    .binding_pressed = on_switch_key_binding_pressed,
    .binding_released = on_switch_key_binding_released,
};

#define SWITCH_KEY_INST(n)                                                                        \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_switch_key_init, NULL, NULL, NULL,                       \
                           POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                      \
                           &behavior_switch_key_driver_api);

DT_INST_FOREACH_STATUS_OKAY(SWITCH_KEY_INST)
