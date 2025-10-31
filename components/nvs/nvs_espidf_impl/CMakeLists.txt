cmake_minimum_required(VERSION 3.16)

idf_component_register(
        SRCS "./src/nvs.cpp"
        PRIV_REQUIRES nvs_flash
                      "nvs_interface"
)