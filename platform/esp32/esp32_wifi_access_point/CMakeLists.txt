cmake_minimum_required(VERSION 3.16)

if(ESP_PLATFORM)
    idf_component_register(
        SRCS "./src/wifi_access_point_impl.cpp"
             "./src/wifi_access_point_factory.cpp"
        PRIV_REQUIRES "wifi_access_point"
    )
else()
    # TODO
endif()
