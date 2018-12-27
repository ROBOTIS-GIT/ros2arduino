# ros2arduino [![Build Status](https://travis-ci.org/ROBOTIS-GIT/ros2arduino.svg?branch=master)](https://travis-ci.org/ROBOTIS-GIT/ros2arduino/branches)
Arduino library for communicating with ROS2(DDS)

<br>

## Version-specific dependencies

|ros2arduino|ROS2|Micro-XRCE-DDS Agent|
|:-:|:-:|:-:|
|0.0.4|[Crystal Clemmys](https://github.com/ros2/ros2/releases/tag/release-crystal-20181214)|[1.0.1](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.0.1)|
|0.0.3|[Crystal Clemmys](https://github.com/ros2/ros2/releases/tag/release-crystal-20181214)|[1.0.1](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.0.1)|
|0.0.2|[Crystal Clemmys](https://github.com/ros2/ros2/releases/tag/release-crystal-20181214)|[1.0.1](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.0.1)|
|0.0.1|[Crystal Clemmys](https://github.com/ros2/ros2/releases/tag/release-crystal-20181214)|[1.0.1](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.0.1)|


<br>

## Restrictions

#### Available boards (What we've tested on our own)
Based on the normal behavior of publisher and subscriber.

 - [OpenCR](http://emanual.robotis.com/docs/en/parts/controller/opencr10/)
 - [OpenCM9.04](http://emanual.robotis.com/docs/en/parts/controller/opencm904/)
 - Arduino MKR ZERO
 - Arduino DUE
 - ESP32 (not support TCP yet)
 - ESP8266 (not support TCP yet)

#### Communication
 ||Implemented|Note|
 |:-:|:-:|:-:|
 |Serial|YES||
 |UDP|YES|ESP32, ESP8266|
 |TCP|NO|Debugging...|

<br>

## Getting Start

#### Dependancy Installation
You must install ROS2 and XRCE-DDS Agent. (The version should be the same as the Version-specific dependencies above)
 - [ROS2](https://index.ros.org/doc/ros2/Installation/)
 - XRCE-DDS Agent
	 - [Micro-XRCE-DDS Agent](https://micro-xrce-dds.readthedocs.io/en/latest/installation.html#installing-the-agent-stand-alone) for FastRTPS

#### Upload Arduino sketch
 - [File] - [Examples] - [ros2arduino] - [publisher]
 - [Sketch] - [Upload]
 
 
#### Excute XRCE-DDS Agent
```bash
$ MicroXRCEAgent serial /dev/ttyACM0
```

#### Check topic on ROS2
```bash
$ ros2 topic echo /arduino_chatter
```
 
 <br>
 
## Development Note

#### Lastest release
 - **Feature**
    - Only one node available
    - Publisher
    - Subscriber
 - **Communication**
    - Serial
    - UDP

#### Plan
 - **Feature**
    - ROS2 service
 - **Communication**
    - Ethernet Shield (UDP, TCP)
    - Official Arduino WiFi (MKR1000, MKRWiFi, etc)
 - **Enhancements**
    - Memory allocation / Management
    - Reduce memory usages
    - Reusable API interfaces
    - Reasonable SW structure
    
