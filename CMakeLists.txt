cmake_minimum_required(VERSION 3.16)

# set(PRODUCT gateway)  # Lub client – do przełączania
set(OS freertos) 
set(TARGET_PLATFORM "esp32")


if(${TARGET_PLATFORM} STREQUAL "esp32")

    set(EXTRA_COMPONENT_DIRS
        ${CMAKE_SOURCE_DIR}/core/client
        ${CMAKE_SOURCE_DIR}/hw/wifi_access_point
        ${CMAKE_SOURCE_DIR}/platform/esp32
        ${CMAKE_SOURCE_DIR}/os/os_interface
        ${CMAKE_SOURCE_DIR}/os/implementations/freertos_espidf
    )

    include($ENV{IDF_PATH}/tools/cmake/project.cmake)

    project(wifi_extender)

    idf_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)

elseif(${TARGET_PLATFORM} STREQUAL "linux")
    add_subdirectory(core/client)
    add_subdirectory(platform/linux)
endif()
