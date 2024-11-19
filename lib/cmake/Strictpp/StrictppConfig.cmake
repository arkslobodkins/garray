cmake_minimum_required(VERSION 3.12)
add_library(Strictpp INTERFACE IMPORTED)

target_include_directories(Strictpp INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../src)
target_compile_features(Strictpp INTERFACE cxx_std_20)
target_compile_definitions(Strictpp INTERFACE "$<$<CONFIG:Release>:STRICT_DEBUG_OFF>")

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
target_link_libraries(Strictpp INTERFACE ${MATH_LIBRARY})
endif()
