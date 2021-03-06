/*
 * gz_agile_leg_plugin.h
 *
 *  Created on: Mar 7, 2018
 *      Author: bibei
 */

#ifndef PLUGINS_INCLUDE_GZ_AGILE_LEG_PLUGIN_H_
#define PLUGINS_INCLUDE_GZ_AGILE_LEG_PLUGIN_H_

#include "foundation/utf.h"
#include "foundation/thread/threadpool.h"

#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

#include <vector>
#include <boost/lockfree/queue.hpp>
#include <platform/proto/agile_proto.h>

//! The default style is msgq.
// #define USE_SHM
///! Forward declare
class MsgQueue;
class SharedMem;
class Pid;

namespace agile_gazebo {

class GzAgileLegPlugin : public gazebo::ModelPlugin {
public:
  GzAgileLegPlugin();
  virtual ~GzAgileLegPlugin();
public:
  void Load(gazebo::physics::ModelPtr _parent, sdf::ElementPtr _sdf);
  // Called by the world update start event
  void event_update();

protected:
  /*!
   * @brief This method is the thread function for the control of joints.
   */
  void joint_control();
  /**
   * @brief This method completes the function that convert the Packet
   *        to the form of message which be needed by the specific communication
   *        type, and writing the converted message to robot.
   * @param pkt[in] The Packet object need to convert and write
   * @return Return true if successful, or return false
   */
  virtual bool write(const Packet& pkt);
  /**
   * @brief This method must be implemented by subclass, and convert the message
   *        which received from the specific communication type to the form of Packet,
   * @param pkt[out] The Packet object converted from the message
   * @return Return true if read successful, or return false
   */
  virtual bool read(Packet& pkt);
  /*!
   * @brief This method will launch from a thread to write the message to the message queue.
   */
  void msg_w_update();
  /*!
   * @brief This method will launch from a thread to read the message from the message queue.
   */
  void msg_r_update();

private:
  void __parse_command_pkg(const Packet&);
  void __write_command_to_sim(const Packet&);

  void __update_robot_stats();

// Pointer to the model
private:
  gazebo::physics::WorldPtr world_;
  gazebo::physics::ModelPtr model;
  // Pointer to the update event connection
  gazebo::event::ConnectionPtr updateConnection;

  bool                            thread_alive_;
  ///! The message queue or the shared memory
  Packet                          pkt_rw_;
#ifdef USE_SHM
  SharedMem*                      ipc_;
  void*                           shm_r_buf_;
  void*                           shm_w_buf_;
#else
  MsgQueue*                       ipc_;
#endif
  std::string                     cmd_ipc_name_;
  std::string                     leg_node_ipc_name_;
  boost::lockfree::queue<Packet>* swap_r_buffer_;
  boost::lockfree::queue<Packet>* swap_w_buffer_;
  ///! These interfaces for Joint
  std::vector<gazebo::physics::JointPtr> joints_;
  ///! These interfaces for the control of joint.
  std::vector<class Pid*>                joints_control_;
  std::vector<class JntTarget*>          joints_target_;
  ///! These interfaces for Actor
  // MiiVector<gazebo::physics::ActorPtr> actors_;
  ///! NODE_ID map to LEG
  LegType             leg_type_;
  unsigned char       leg_id_;
  class LinearParams* linear_params_;
};

} /* namespace agile_gazebo */

#endif /* PLUGINS_INCLUDE_GZ_AGILE_LEG_PLUGIN_H_ */
