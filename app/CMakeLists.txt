cmake_minimum_required(VERSION 3.16)

idf_component_register(SRCS "main.cpp"
                       PRIV_REQUIRES nvs_interface wifi_extender_if rgbled_if
                       INCLUDE_DIRS "")