/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zephyr/kernel.h>

/**
 * Get the current global priority state
 * @return Current priority (0 or 1)
 */
uint8_t zmk_switch_key_get_priority(void);

/**
 * Set the global priority state
 * @param priority New priority value (0 or 1)
 * @return 0 on success, negative errno on failure
 */
int zmk_switch_key_set_priority(uint8_t priority);
