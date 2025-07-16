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

add_library(kv_store STATIC ${SOURCES} ${HEADERS})

target_include_directories(kv_store PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(kv_store PUBLIC logger)


set(TEST_CONTROLLER ${CMAKE_CURRENT_SOURCE_DIR}/test/test_kv_controller.c
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/include/test_kv_controller.h
)
set(TEST_PARSER ${CMAKE_CURRENT_SOURCE_DIR}/test/test_kv_parser.c
                ${CMAKE_CURRENT_SOURCE_DIR}/test/include/test_kv_parser.h
)

add_executable(test_kv_parser ${TEST_PARSER} ${SOURCES} ${HEADERS})
add_executable(test_kv_controller ${TEST_CONTROLLER} ${SOURCES} ${HEADERS})

target_include_directories(test_kv_parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                                  ${CMAKE_CURRENT_SOURCE_DIR}/test/include)

target_include_directories(test_kv_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                                      ${CMAKE_CURRENT_SOURCE_DIR}/test/include)

target_compile_definitions(unity PUBLIC
  UNITY_INCLUDE_DOUBLE
  UNITY_DOUBLE_PRECISION=1e-15
)

target_link_libraries(test_kv_parser PRIVATE logger unity)
target_link_libraries(test_kv_controller PRIVATE logger unity)

enable_testing()

add_test(NAME test_kv_parser
         COMMAND test_kv_parser
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME test_kv_controller
         COMMAND test_kv_controller
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(test_kv_parser PRIVATE -fsanitize=address)
target_link_options(test_kv_parser PRIVATE -fsanitize=address)

target_compile_options(test_kv_controller PRIVATE -fsanitize=address)
target_link_options(test_kv_controller PRIVATE -fsanitize=address)
