cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)

project(kv_store C)

include(FetchContent)

FetchContent_Declare(logger
  GIT_REPOSITORY https://github.com/rijegaro287/logger.git
  GIT_TAG        origin/main
)

FetchContent_Declare(unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        origin/master
)

FetchContent_MakeAvailable(logger)
FetchContent_MakeAvailable(unity)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/linked_list.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/hash_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/string_conversion.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/kv_parser.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/kv_controller.c
)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/constants.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/linked_list.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/hash_table.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/string_conversion.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/kv_parser.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/kv_controller.h
)

set(TEST ${CMAKE_CURRENT_SOURCE_DIR}/test/tests.c
         ${CMAKE_CURRENT_SOURCE_DIR}/test/test_kv_parser.c
         ${CMAKE_CURRENT_SOURCE_DIR}/test/include/test_kv_parser.h
)

add_executable(test_kv_store ${TEST} ${SOURCES} ${HEADERS})

add_library(kv_store STATIC ${SOURCES} ${HEADERS})

target_include_directories(test_kv_store PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/test/include)

target_include_directories(kv_store PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(test_kv_store PRIVATE logger unity)
target_link_libraries(kv_store PRIVATE logger)
