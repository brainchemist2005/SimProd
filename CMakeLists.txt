# Global parameters
# -----------------

cmake_minimum_required(VERSION 3.16)
project(simprod)
enable_testing()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Dependencies
# ------------

find_package(Git QUIET)
message(STATUS "Updating submodules")
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
execute_process(COMMAND ${GIT_EXECUTABLE} checkout cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/libtap)

# Jansson
# -------

option(JANSSON_BUILD_DOCS "Build documentation (uses python-sphinx)." OFF)
option(JANSSON_WITHOUT_TESTS "Don't build tests ('make test' to execute tests)" ON)
add_subdirectory(external/jansson)

# Main executable
# ---------------

add_executable(simprod
    src/component/link.c
    src/component/link.h
    src/component/plant.c
    src/component/plant.h
    src/component/zone.c
    src/component/zone.h
    src/plan.c
    src/plan.h
    src/scenario.c
    src/scenario.h
    src/simprod.c
    src/timeline.c
    src/timeline.h
    src/utils/string_array.c
    src/utils/string_array.h
    src/utils/treemap.c
    src/utils/treemap.h
    src/validation.c
    src/validation.h)
target_include_directories(simprod
    PRIVATE src
            ${CMAKE_CURRENT_BINARY_DIR}/external/jansson/include)
target_link_libraries(simprod jansson)

# Examples
# --------

add_custom_target(copy-examples
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/examples
            ${CMAKE_CURRENT_BINARY_DIR}/examples)

# Libtap
# ------

add_subdirectory(external/libtap)

# Tests
# -----

macro(add_test_executable name path)
    add_executable(test_${name}
        ${path}
        src/component/link.c
        src/component/link.h
        src/component/plant.c
        src/component/plant.h
        src/component/zone.c
        src/component/zone.h
        src/plan.c
        src/plan.h
        src/scenario.c
        src/scenario.h
        src/timeline.c
        src/timeline.h
        src/utils/string_array.c
        src/utils/string_array.h
        src/utils/treemap.c
        src/utils/treemap.h
        src/validation.c
        src/validation.h)
    target_include_directories(test_${name}
        PRIVATE src
                ${CMAKE_CURRENT_BINARY_DIR}/external/jansson/include
                ${CMAKE_CURRENT_BINARY_DIR}/external/libtap)
    target_link_libraries(test_${name}
        jansson
        tap)
    add_test(NAME test_${name} COMMAND test_${name})
    add_custom_target(exec_test_${name}
        COMMAND ./test_${name})
endmacro(add_test_executable)

add_test_executable(link src/component/test_link.c)
add_test_executable(plan src/test_plan.c)
add_test_executable(plant src/component/test_plant.c)
add_test_executable(scenario src/test_scenario.c)
add_test_executable(timeline src/test_timeline.c)
add_test_executable(treemap src/utils/test_treemap.c)
add_test_executable(zone src/component/test_zone.c)

add_custom_target(test-unit
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target exec_test_link
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target exec_test_plan
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target exec_test_plant
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target exec_test_scenario
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target exec_test_timeline
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target exec_test_treemap
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target exec_test_zone)

# Bats
# ----

macro(add_bats_test name)
    add_custom_target(${name}-bats
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/external/bats-core/bin/bats ../tests/${name}.bats)
endmacro(add_bats_test)

add_bats_test(simprod)
add_bats_test(scenario)
add_bats_test(plan)

add_custom_target(test-bats
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target plan-bats
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target scenario-bats
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target simprod-bats)
add_dependencies(test-bats copy-examples)
