# mico_ros

A micro-ROS on Raspberry Pi Pico playground.

Based on [micro-ROS/micro_ros_raspberrypi_pico_sdk](https://github.com/micro-ROS/micro_ros_raspberrypi_pico_sdk).

## Getting started

Find out how to get started with this [related blog post](https://artivis.github.io/post/2021/pi-pico-uros-getting-started/).

## Available demo

- [src/temperature_node.cpp](src/temperature_node.cpp): Publishes the Pico onboard temperature as a `sensor_msgs/msg/Temperature` message.
- [src/range_node.cpp](src/range_node.cpp): Publishes a `sensor_msgs/msgs/Range` message from a wired up 'HC-SR04' sensor ([blog post](https://artivis.github.io/post/2021/pi-pico-uros-sonar)).
