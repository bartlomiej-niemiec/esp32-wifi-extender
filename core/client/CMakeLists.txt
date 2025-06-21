cmake_minimum_required(VERSION 3.16)

if(ESP_PLATFORM)
idf_component_register(SRCS "main.cpp"
                       PRIV_REQUIRES nvs_flash esp_wifi wifi_access_point
                       INCLUDE_DIRS "")
else()
    # TODO
endif()