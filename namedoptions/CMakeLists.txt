add_library(namedoptions OBJECT namedoptions.h namedoptions_p.h)
target_compile_features(namedoptions PUBLIC cxx_std_20)
target_compile_options(namedoptions PUBLIC -Wno-missing-field-initializers)
target_include_directories(namedoptions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(namedoptions PROPERTIES LINKER_LANGUAGE CXX)

