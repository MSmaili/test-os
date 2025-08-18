#pragma once

#include <zephyr/kernel.h>

/**
 * Get the current switch key priority state.
 * @return 0 for first option, 1 for second option
 */
int zmk_behavior_switch_key_get_priority(void);

/**
 * Set the switch key priority state.
 * @param priority 0 for first option, 1 for second option
 * @return 0 on success, negative error code on failure
 */
int zmk_behavior_switch_key_set_priority(uint8_t priority);
