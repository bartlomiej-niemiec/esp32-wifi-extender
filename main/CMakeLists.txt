cmake_minimum_required(VERSION 3.16)

idf_component_register(SRCS "main.cpp"
                       PRIV_REQUIRES wifi_extender_if rgbled_if data_storer_if mongoose
                       INCLUDE_DIRS "")