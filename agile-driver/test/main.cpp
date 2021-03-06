/*
 * test_ros_wrapper.cpp
 *
 *  Created on: Dec 5, 2016
 *      Author: silence
 */

#include <iostream>
#include "mii_robot.h"

#include <foundation/utf.h>
#include <foundation/cfg_reader.h>
#include <foundation/auto_instor.h>

class TestMiiRobot : public agile_robot::MiiRobot {
  SINGLETON_DECLARE(TestMiiRobot, const std::string&)

  public:
    virtual void create_system_singleton() override;
    void halt();
  private:
    std::string       root_tag_;
};

SINGLETON_IMPL_NO_CREATE(TestMiiRobot)

TestMiiRobot* TestMiiRobot::create_instance(const std::string& __tag) {
  if (nullptr != s_inst_) {
    LOG_WARNING << "This method 'create_instance()' is called twice.";
  } else {
    s_inst_ = new TestMiiRobot(__tag);
  }
  return s_inst_;
}

TestMiiRobot::TestMiiRobot(const std::string& __tag)
  : MiiRobot(/*Label::make_label(__tag, "robot")*/), root_tag_(__tag)/*, alive_(false),
    rt_duration_(1000/50), ros_ctrl_duration_(1000/100), use_ros_control_(false)*/ {
  // LOG_DEBUG << "Enter the roswrapper construction";
  // google::InitGoogleLogging("qr_driver");
  // google::SetLogDestination(google::GLOG_INFO, "/path/to/log/INFO_");
  FLAGS_colorlogtostderr = true;
  // google::FlushLogFiles(google::GLOG_INFO);
  // LOG_DEBUG << "Leave the roswrapper construction";
  ; // Nothing to do here, all of variables initialize in the method @start()
}

TestMiiRobot::~TestMiiRobot() {
  // AutoInstor::destroy_instance();
  CfgReader::destroy_instance();
  // LOG_DEBUG << "Leave the roswrapper deconstruction";
  // google::ShutdownGoogleLogging();
}

void TestMiiRobot::create_system_singleton() {
  if (nullptr == CfgReader::create_instance())
    LOG_FATAL << "Create the singleton 'CfgReader' has failed.";

  if (nullptr == AutoInstor::create_instance())
    LOG_FATAL << "Create the singleton 'AutoInstor' has failed.";

  CfgReader::instance()->add_path("/home/bibei/Workspaces/qr_ws/src/qr-driver-0.2.9/config");

  std::string str = "robot_config.xml";
  CfgReader::instance()->add_config(str);

  str = "/home/bibei/Workspaces/qr_ws/devel/lib/libqr_driver_sys_platform.so";
  AutoInstor::instance()->add_library(str);

  // LOG_DEBUG << "==========RosWrapper::create_system_instance==========>>";
  MiiRobot::create_system_singleton();
}

int main(int argc, char* argv[]) {


  // google::InitGoogleLogging("qr_driver");
  // google::FlushLogFiles(google::GLOG_INFO);
  if (nullptr == TestMiiRobot::create_instance("qr.wrapper"))
    LOG_FATAL << "Can't get the instance of RosWrapper!";
  TestMiiRobot::instance()->start();

  ; // Waiting

  TestMiiRobot::destroy_instance();
  LOG_INFO << "The shutdown of qr_driver has finished... ...";
  // google::ShutdownGoogleLogging();
  return 0;
}
