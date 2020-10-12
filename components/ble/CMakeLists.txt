set(COMPONENT_SRCDIRS "src")
set(COMPONENT_ADD_INCLUDEDIRS "src" 
        "$ENV{IDF_PATH}/components/bt/host/bluedroid/api/include/api"
        "C:/ESP32/arduino-esp32/cores/esp32"
        "C:/ESP32/esp-idf/components/bt/include"
)
list(APPEND COMPONENT_REQUIRES "nvs_flash")
register_component()



