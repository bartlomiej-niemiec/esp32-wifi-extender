cmake_minimum_required(VERSION 3.16)

if(ESP_PLATFORM)

idf_component_register(INCLUDE_DIRS "./include"
                       SRCS "./src/ApActiveClients/ApActiveClients.cpp")
else()
    # TODO
endif()