cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)

include(FetchContent)

FetchContent_Declare(logger
  GIT_REPOSITORY https://github.com/rijegaro287/logger.git
  GIT_TAG        origin/main
)
FetchContent_MakeAvailable(logger)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/linked_list.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/hash_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/string_conversion.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/kv_parser.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/kv_controller.c
)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/constants.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/kv_parser.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/kv_controller.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/string_conversion.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/linked_list.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/hash_table.h
)

add_library(kv_store STATIC ${SOURCES} ${HEADERS})

target_include_directories(kv_store PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(kv_store PRIVATE logger)
