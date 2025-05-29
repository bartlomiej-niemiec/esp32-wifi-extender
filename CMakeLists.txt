cmake_minimum_required(VERSION 3.16)

# set(PRODUCT gateway)  # Lub client – do przełączania

if(NOT DEFINED TARGET_PLATFORM)
    set(TARGET_PLATFORM esp32)  # lub 'linux'
endif()

if(${TARGET_PLATFORM} STREQUAL "esp32")

    set(EXTRA_COMPONENT_DIRS
        ${CMAKE_SOURCE_DIR}/core/gateway
        ${CMAKE_SOURCE_DIR}/core/client
        ${CMAKE_SOURCE_DIR}/platform/esp32
    )
    
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)

    project(wifi_extender)

    idf_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)

elseif(${TARGET_PLATFORM} STREQUAL "linux")
    add_subdirectory(core/gateway)
    add_subdirectory(core/client)
    add_subdirectory(platform/linux)
endif()
