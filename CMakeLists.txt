cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS
    ${CMAKE_SOURCE_DIR}/main
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/utils
    ${CMAKE_SOURCE_DIR}/components/wifi_extender/wifi_extender_if
    ${CMAKE_SOURCE_DIR}/components/wifi_extender/wifi_extender_espidf_impl
    ${CMAKE_SOURCE_DIR}/components/rgbled/rgbled_if
    ${CMAKE_SOURCE_DIR}/components/rgbled/rgbled_sk68xxminihs
    ${CMAKE_SOURCE_DIR}/components/data_storer/data_storer_if
    ${CMAKE_SOURCE_DIR}/components/data_storer/data_storer_nvs_impl
)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(wifi_extender)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)
