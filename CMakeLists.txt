cmake_minimum_required(VERSION 3.16)

set(OS freertos) 
set(TARGET_PLATFORM "esp32")
set(PRODUCT "client")

if(${TARGET_PLATFORM} STREQUAL "esp32")

    if (${PRODUCT} STREQUAL "client")
        set(EXTRA_COMPONENT_DIRS
            ${CMAKE_SOURCE_DIR}/app
            ${CMAKE_SOURCE_DIR}/libs
            ${CMAKE_SOURCE_DIR}/hw/wifi_extender/wifi_extender_if
            ${CMAKE_SOURCE_DIR}/hw/wifi_extender/platform/esp32_wifi_extender
            ${CMAKE_SOURCE_DIR}/hw/nvs/nvs_interface
            ${CMAKE_SOURCE_DIR}/hw/nvs/platform/esp32_nvs_flash
    )
    elseif(${TARGET_PLATFORM} STREQUAL "gateway")
        set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/platform/esp32/gateway)
    endif()

    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(wifi_extender)
    idf_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)
endif()
