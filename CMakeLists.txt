cmake_minimum_required(VERSION 3.16)

set(OS freertos) 
set(TARGET_PLATFORM "esp32")
set(PRODUCT "client")

if(${TARGET_PLATFORM} STREQUAL "esp32")

    if (${PRODUCT} STREQUAL "client")
        set(EXTRA_COMPONENT_DIRS
            ${CMAKE_SOURCE_DIR}/core/client
            ${CMAKE_SOURCE_DIR}/hw/wifi_access_point
            ${CMAKE_SOURCE_DIR}/hw/implementations/esp32_wifi_access_point
            ${CMAKE_SOURCE_DIR}/hw/nvs
            ${CMAKE_SOURCE_DIR}/hw/implementations/esp32_nvs_flash
            ${CMAKE_SOURCE_DIR}/os/os_interface
            ${CMAKE_SOURCE_DIR}/os/implementations/freertos_espidf
    )
    elseif(${TARGET_PLATFORM} STREQUAL "gateway")
        set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/platform/esp32/gateway)
    endif()

    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(wifi_extender)
    idf_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)
endif()
