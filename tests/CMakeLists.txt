add_compile_definitions(TASCHENTEST_USING_MACROS)

add_library(taschentest OBJECT taschentest.h taschentest.cpp)
target_compile_features(taschentest PUBLIC cxx_std_20)
target_include_directories(taschentest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tst_namedoptions tst_namedoptions.cpp)
target_link_libraries(tst_namedoptions PRIVATE namedoptions taschentest)
add_test(NAME tst_namedoptions COMMAND $<TARGET_FILE:tst_namedoptions>)

