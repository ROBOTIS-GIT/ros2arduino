#ifndef SKETCH_USER_CONFIG_H_
#define SKETCH_USER_CONFIG_H_

/** 
 * Select to enable eProsima's Micro-XRCE-DDS reference function.
 * Please refer to https://github.com/ROBOTIS-GIT/ros2arduino#appendix-how-to-configure-entities-from-reference-file-available-at-011-or-above
 **/
#define UXR_CREATE_ENTITIES_USING_REF  0 


/* The options below are needed to reduce the usage of static memory */

// Select only one communication method you wish to use.
// options: USER_TRANSPORT_TYPE_UDP, USER_TRANSPORT_TYPE_TCP, USER_TRANSPORT_TYPE_SERIAL
#define USER_TRANSPORT_TYPE   USER_TRANSPORT_TYPE_UDP

// Set the MTU of the transport.
#define USER_TRANSPORT_MTU    2048

// Set the maximum number of Pub/Subs to be created on the node.
#define USER_ROS2_PUBLISHER_MAX  5 
#define USER_ROS2_SUBSCRIBER_MAX 5


#endif /* SKETCH_USER_CONFIG_H_ */
