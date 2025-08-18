if ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_SWITCH_KEY app PRIVATE src/behaviors/behavior_switch_key.c)
  target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_SWITCH_KEY app PRIVATE src/behaviors/behavior_switch_key_set_priority.c)
endif()
