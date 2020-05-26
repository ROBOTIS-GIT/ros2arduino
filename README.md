# ros2arduino [![Build Status](https://travis-ci.org/ROBOTIS-GIT/ros2arduino.svg?branch=master)](https://travis-ci.org/ROBOTIS-GIT/ros2arduino/branches)
Arduino library for communicating with ROS2(DDS)

<br>

## Version-specific dependencies
We plan to support only LTS version for ROS2.

- Recommend version (present)

|ros2arduino|ROS2|Micro-XRCE-DDS Agent|
|:-:|:-:|:-:|
|0.2.1|[Dashing Diademata Patch6](https://github.com/ros2/ros2/releases/tag/release-dashing-20200319)|[1.3.0](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.3.0)|

For the Micro-XRCE-DDS Agent, please install it using following commands.
```bash
$ git clone https://github.com/eProsima/Micro-XRCE-DDS-Agent.git
$ cd Micro-XRCE-DDS-Agent && git checkout v1.3.0
$ mkdir build && cd build
$ source /opt/ros/dashing/setup.bash # to share libraries with ros2
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig /usr/local/lib/
```

- All

|ros2arduino|ROS2|Micro-XRCE-DDS Agent|
|:-:|:-:|:-:|
|0.2.1|[Dashing Diademata Patch6](https://github.com/ros2/ros2/releases/tag/release-dashing-20200319)|[1.3.0](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.3.0)|
|0.1.4|[Dashing Diademata Patch3](https://github.com/ros2/ros2/releases/tag/release-dashing-20190910)|[1.1.0](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.1.0)|
|0.1.3|[Dashing Diademata Patch3](https://github.com/ros2/ros2/releases/tag/release-dashing-20190910)|[1.1.0](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.1.0)|
|0.1.2|[Dashing Diademata Patch1](https://github.com/ros2/ros2/releases/tag/release-dashing-20190614)|[1.1.0](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.1.0)|
|0.1.1|[Dashing Diademata](https://github.com/ros2/ros2/releases/tag/release-dashing-20190531)|[1.1.0](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.1.0)|
|0.0.9|[Crystal Clemmys](https://github.com/ros2/ros2/releases/tag/release-crystal-20181214)|[1.0.1](https://github.com/eProsima/Micro-XRCE-DDS-Agent/releases/tag/v1.0.1)|

<br>

## Restrictions

#### Memory
- RAM : >= 32Kb
- Boards tested : Based on the normal behavior of publisher and subscriber.
  - [OpenCR](http://emanual.robotis.com/docs/en/parts/controller/opencr10/)
  - Arduino MKR ZERO
  - Arduino DUE
  - ESP32 (not support TCP yet)

#### Communication
 ||Implemented|Note|
 |:-:|:-:|:-:|
 |Serial|YES||
 |UDP|YES|ESP32, Ethernet|
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
 
 
#### Excute Micro-XRCE-DDS Agent

- Please refer to [eProsima manual](https://micro-xrce-dds.readthedocs.io/en/latest/agent.html) for Micro-XRCE-DDS-Agent usage.

- 0.2.1 or above (Micro-XRCE-DDS-Agent 1.3.0)
  - Serial
    ```bash
    $ MicroXRCEAgent serial --dev /dev/ttyACM0 -b 115200
    ```
  - UDP
    ```bash
    $ MicroXRCEAgent udp4 -p 2018
    ```
  - TCP
    ```bash
    $ MicroXRCEAgent tcp4 -p 2018
    ```

- 0.1.0 ~ 0.1.4 (Micro-XRCE-DDS-Agent 1.1.0)
  - Serial
    ```bash
    $ MicroXRCEAgent serial --dev /dev/ttyACM0 -b 115200
    ```
  - UDP
    ```bash
    $ MicroXRCEAgent udp -p 2018
    ```
  - TCP
    ```bash
    $ MicroXRCEAgent tcp -p 2018
    ```

- 0.0.9 (Micro-XRCE-DDS-Agent 1.0.1)
  - Serial
    ```bash
    $ MicroXRCEAgent --serial /dev/ttyACM0 115200
    ```
  - UDP
    ```bash
    $ MicroXRCEAgent --udp 2018
    ```
  - TCP
    ```bash
    $ MicroXRCEAgent --tcp 2018
    ```

#### Check topic on ROS2
```bash
$ ros2 topic echo /arduino_chatter
```

#### Appendix: How to configure entities from reference file. (available at 0.1.1 or above)
- Use the reference method supported by Client and Agent. Please refer to [eProsima manual](https://micro-xrce-dds.readthedocs.io/en/latest/agent.html#run-an-agent) for detailed usage.
- For this feature, you need to set `UXR_CREATE_ENTITIES_USING_REF` definition to `1`.
	```cpp
	#define UXR_CREATE_ENTITIES_USING_REF 1
	``` 
  - ros2arduino 0.1.1 ~ 0.1.4
    - You need to change the settings(library code) in ros2arduino library. (In the [user_config.h](https://github.com/ROBOTIS-GIT/ros2arduino/blob/master/src/user_config.h))

- Create .refs file(in XML format) and run the Agent with the following options:
	```bash
	$ MicroXRCEAgent serial --dev /dev/ttyACM0 -b 115200 -r ros2arduino.refs
	```
  - An example reference file is as follows.
    - ros2arduino.refs
		```xml
		<profiles>
			<participant profile_name="ros2arduino_xml_node">
				<rtps>
					<name>ros2arduino_basic_node</name>
					<builtin>
						<domainId>0</domainId>
					</builtin>
				</rtps>
			</participant>


			<data_writer profile_name="arduino_chatter">
				<topic>
					<kind>NO_KEY</kind>
					<name>rt/arduino_chatter</name>
					<dataType>std_msgs::msg::dds_::String_</dataType>
					<historyQos>
						<kind>KEEP_LAST</kind>
						<depth>10</depth>
					</historyQos>
				</topic>
			</data_writer>


			<data_reader profile_name="arduino_led">
				<topic>
					<name>rt/arduino_led</name>
					<dataType>std_msgs::msg::dds_::Bool_</dataType>
				</topic>
			</data_reader>


			<topic profile_name="Bool">
				<kind>NO_KEY</kind>
				<name>Bool</name>
				<dataType>std_msgs::msg::dds_::Bool_</dataType>
			</topic>

			<topic profile_name="String">
				<kind>NO_KEY</kind>
				<name>String</name>
				<dataType>std_msgs::msg::dds_::String_</dataType>
			</topic>
		</profiles>
		```

#### Appendix: How to use the Security feature. (available at 0.1.1 or above)

This method should be preceded by the function described in ["Appendix: How to configure entities from reference file."](https://github.com/ROBOTIS-GIT/ros2arduino#appendix-how-to-configure-entities-from-reference-file-available-at-011-or-above)

And please check eProsima's manual for how to use it.
- [Fast RTPS security documentation](https://fast-rtps.docs.eprosima.com/en/latest/security.html#example-configuring-the-participant)


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
    - TCP
 - **Enhancements**
    - Memory allocation / Management
    - Reduce memory usages
    - Reusable API interfaces
    - Reasonable SW structure
    
