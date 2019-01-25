# ros2arduino [![Build Status](https://travis-ci.org/ROBOTIS-GIT/ros2arduino.svg?branch=master)](https://travis-ci.org/ROBOTIS-GIT/ros2arduino/branches)
Arduino library for communicating with ROS2(DDS)

<br>

## Version-specific dependencies

- Recommend version (current)

|ros2arduino|ROS2|Micro-XRCE-DDS Agent|
|:-:|:-:|:-:|
|0.0.6|[Crystal Clemmys](https://github.com/ros2/ros2/releases/tag/release-crystal-20181214)| [specific commit](2c136dc429178dab79b231cc5d67a7c229cca77a) |

For the Micro-XRCE-DDS Agent, please install it using following commands.
```bash
$ git clone https://github.com/eProsima/Micro-XRCE-DDS-Agent.git
$ cd Micro-XRCE-DDS-Agent && git checkout 2c136dc429178dab79b231cc5d67a7c229cca77a
$ mkdir build && cd build
$ cmake -DTHIRDPARTY=ON -DCONFIG_UDP_TRANSPORT_MTU=4096 -DCONFIG_SERIAL_TRANSPORT_MTU=4096 ..
$ sudo make install
$ sudo ldconfig /usr/local/lib/
```


- All

|ros2arduino|ROS2|Micro-XRCE-DDS Agent|
|:-:|:-:|:-:|
|0.0.6|[Crystal Clemmys](https://github.com/ros2/ros2/releases/tag/release-crystal-20181214)|[1.0.1](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.0.1) < |

<br>

## Restrictions

#### Available boards (What we've tested on our own)
Based on the normal behavior of publisher and subscriber.

 - [OpenCR](http://emanual.robotis.com/docs/en/parts/controller/opencr10/)
 - [OpenCM9.04](http://emanual.robotis.com/docs/en/parts/controller/opencm904/)
 - Arduino MKR ZERO
 - Arduino DUE
 - ESP32 (not support TCP yet)

#### Communication
 ||Implemented|Note|
 |:-:|:-:|:-:|
 |Serial|YES||
 |UDP|YES|ESP32, ESP8266, Ethernet|
 |TCP|NO|Debugging...|

<br>

## Getting Start

#### Dependancy Installation
You must install ROS2 and XRCE-DDS Agent. (The version should be the same as the Version-specific dependencies above)
 - [ROS2](https://index.ros.org/doc/ros2/Installation/)
 - XRCE-DDS Agent
	 - [Micro-XRCE-DDS Agent](https://micro-xrce-dds.readthedocs.io/en/latest/installation.html#installing-the-agent-stand-alone) for FastRTPS

#### Upload Arduino sketch
 - [File] - [Examples] - [ros2arduino]
   - Serial
     - [publisher]
   - UDP
     - [publisher_wifi_udp]
     - [publisher_ethernet_udp]
   - TCP
     - [publisher_wifi_tcp]
     - [publisher_ethernet_tcp]
 - [Sketch] - [Upload]
 
 
#### Excute XRCE-DDS Agent
 - Serial
   ```bash
   $ MicroXRCEAgent serial /dev/ttyACM0
   ```
 - UDP
   ```bash
   $ MicroXRCEAgent udp 2018
   ```
 - TCP
   ```bash
   $ MicroXRCEAgent tcp 2018
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
    - TCP (Debugging)
 - **Enhancements**
    - Memory allocation / Management
    - Reduce memory usages
    - Reusable API interfaces
    - Reasonable SW structure
    
