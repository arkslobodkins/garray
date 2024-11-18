add_library(Strictpp INTERFACE IMPORTED)
target_include_directories(Strictpp INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../src)
target_compile_definitions(Strictpp INTERFACE "$<$<CONFIG:Release>:STRICT_DEBUG_OFF>")
