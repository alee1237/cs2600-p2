cmake_minimum_required(VERSION 3.26.4)
project(mqtt_led VERSION 1.0.0 LANGUAGES C)

find_package(eclipse-paho-mqtt-c REQUIRED)

add_executable(mqtt_led mqtt_led.c)
target_link_libraries(mqtt_led eclipse-paho-mqtt-c::paho-mqtt3c)