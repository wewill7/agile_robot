/*
 * qr_ros_wrapper.h
 *
 *  Created on: Dec 5, 2016
 *      Author: silence
 */

#ifndef INCLUDE_QR_ROS_WRAPPER_H_
#define INCLUDE_QR_ROS_WRAPPER_H_

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64MultiArray.h>
#include <realtime_tools/realtime_buffer.h>
#include <realtime_tools/realtime_publisher.h>
#include <controller_manager/controller_manager.h>

#include <chrono>
#include <boost/scoped_ptr.hpp>

#include "robot/mii_robot.h"
///! qr-next-control
#include <mii_control.h>

#define DEBUG_TOPIC
#ifdef DEBUG_TOPIC
#include <repository/resource/joint.h>
#endif

using namespace agile_robot;

class RobotWrapper : public MiiRobot {
SINGLETON_DECLARE(RobotWrapper, const std::string&)

public:
  virtual bool start() override;
  void halt();

private:
  virtual void create_system_instance() override;
  ///! This method publish the real-time message, e.g. "/joint_states", "imu", "foot_force"
  void publishRTMsg();
  // Just for test
#ifdef DEBUG_TOPIC
  void cbForDebug(const std_msgs::Float32ConstPtr&);
  ros::Subscriber cmd_sub_;
#endif

private:
  // The ROS handle
  ros::NodeHandle   nh_;
  std::string       root_tag_;

  bool alive_;
  std::chrono::milliseconds  rt_duration_;
};

#endif /* INCLUDE_QR_ROS_WRAPPER_H_ */