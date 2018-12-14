# ros2arduino
Arduino library for communicating with ROS2(DDS)

<br>

## Version-specific dependencies

|ros2arduino|ROS2|Micro-XRCE-DDS Agent|
|:-:|:-:|:-:|
|0.0.1|[Crystal Clemmys](https://github.com/ros2/ros2/releases/tag/prerelease-crystal-2018-12-08)|[1.0.1](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.0.1)|
||||

<br>

## Restrictions

#### Available boards (What we've tested on our own)
Based on the normal behavior of publisher and subscriber.

 - [OpenCR](http://emanual.robotis.com/docs/en/parts/controller/opencr10/)
 - [OpenCM9.04](http://emanual.robotis.com/docs/en/parts/controller/opencm904/)
 - Arduino MKR ZERO
 - Arduino DUE

#### Communication
 ||Implemented|Note|
 |:-:|:-:|:-:|
 |Serial|YES|only Serial (NOT Serial1,2,3..etc)|
 |UDP|NO||
 |TCP|NO||

<br>

## Getting Start

#### Dependancy Installation
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
    - Only one node avaliable
    - Publisher
    - Subscriber
 - **Communication**
    - Serial (Only Serial instance avaliable)

#### Plan
 - **Feature**
    - ROS2 service
 - **Communication**
    - expand Serial port (To use other Serial instance)
    - Ethernet Shield (UDP, TCP)
    - ESP32 (UDP, TCP)
 - **Enhancements**
    - Memory allocation / Management
    - Reduce memory usages
    - Reusable API interfaces
    - Reasonable SW structure
    
