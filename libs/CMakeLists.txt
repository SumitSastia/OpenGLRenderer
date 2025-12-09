set(LIB_NAME lib1)

add_library(${LIB_NAME} SHARED f1.cpp)
target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(${LIB_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
)