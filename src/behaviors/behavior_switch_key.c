#define DT_DRV_COMPAT zmk_behavior_switch_key

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
int zmk_behavior_switch_key_get_priority(void);
int zmk_behavior_switch_key_set_priority(uint8_t priority);

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

struct behavior_switch_key_config {
    struct zmk_behavior_binding bindings[2];
};

static int behavior_switch_key_init(const struct device *dev) {
    return 0;
}

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                   struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_switch_key_config *cfg = dev->config;

    uint8_t priority = zmk_behavior_switch_key_get_priority();

    LOG_DBG("Switch key pressed, priority: %d", priority);

    // Select the appropriate binding based on priority
    struct zmk_behavior_binding selected_binding = cfg->bindings[priority];

    return zmk_behavior_invoke_binding(&selected_binding, event, false);
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_switch_key_config *cfg = dev->config;

    uint8_t priority = zmk_behavior_switch_key_get_priority();

    LOG_DBG("Switch key released, priority: %d", priority);

    // Select the appropriate binding based on priority
    struct zmk_behavior_binding selected_binding = cfg->bindings[priority];

    return zmk_behavior_invoke_binding(&selected_binding, event, true);
}

static const struct zmk_behavior_driver_api behavior_switch_key_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

#define SWITCH_KEY_INST(n)                                                                        \
    static struct behavior_switch_key_config behavior_switch_key_config_##n = {                   \
        .bindings = {                                                                              \
            ZMK_KEYMAP_EXTRACT_BINDING(0, DT_DRV_INST(n)),                                       \
            ZMK_KEYMAP_EXTRACT_BINDING(1, DT_DRV_INST(n)),                                       \
        },                                                                                         \
    };                                                                                             \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_switch_key_init, NULL, NULL,                             \
                           &behavior_switch_key_config_##n, POST_KERNEL,                          \
                           CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_switch_key_driver_api);

DT_INST_FOREACH_STATUS_OKAY(SWITCH_KEY_INST)
