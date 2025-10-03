add_executable(main main.c)
target_compile_features(main PRIVATE c_std_23)
set_target_properties(main PROPERTIES C_EXTENSIONS OFF)
install(TARGETS main RUNTIME DESTINATION .)
