cmake_minimum_required(VERSION 3.16)

if(ESP_PLATFORM)
    idf_component_register(
        INCLUDE_DIRS "include"
    )
else()
    add_library(NvsInterface INTERFACE)

    target_include_directories(NvsInterface
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif()
