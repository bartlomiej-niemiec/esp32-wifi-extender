cmake_minimum_required(VERSION 3.16)

if(ESP_PLATFORM)
    idf_component_register(
        SRCS "./src/wifi_ap.cpp"
             "./src/wifi_sta.cpp"
             "./src/wifi_extender_factory_impl.cpp"
             "./src/wifi_extender_impl.cpp"
             "./src/wifi_manager.cpp"
        PRIV_REQUIRES esp_wifi "wifi_extender_if" "libs" esp_netif
    )
else()
    # TODO
endif()
