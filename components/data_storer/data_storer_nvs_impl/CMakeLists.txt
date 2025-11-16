cmake_minimum_required(VERSION 3.16)

idf_component_register(
        SRCS    "./src/nvs_data_storage_impl.cpp"
                "./src/nvs_init.cpp"
        PRIV_REQUIRES   "data_storer_if"
                        "utils"
                        nvs_flash
)