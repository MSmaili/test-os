if(CONFIG_ZMK_BEHAVIOR_SWITCH_KEY)
  zephyr_library_sources(src/behaviors/behavior_switch_key.c)
  zephyr_library_include_directories(include)
endif()

if(CONFIG_ZMK_BEHAVIOR_SWITCH_KEY_SET_PRIORITY)
  zephyr_library_sources(src/behaviors/behavior_switch_key_set_priority.c)
endif()
