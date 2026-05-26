#ifndef CS_UNDERSTANDING_EXAMPLES_MODEL_COMBINATOR_HPP_
#define CS_UNDERSTANDING_EXAMPLES_MODEL_COMBINATOR_HPP_

#include "ament_index_cpp/get_package_share_directory.hpp"

#include "example_interfaces/srv/add_two_ints.hpp"
#include <behaviortree_ros2/bt_service_node.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>


class ModelCombinatorNode : public BT::RosServiceNode<example_interfaces::srv::AddTwoInts>
{
public:
  explicit ModelCombinatorNode(const std::string& name, const BT::NodeConfig& config,
               const BT::RosNodeParams& params)
    : BT::RosServiceNode<example_interfaces::srv::AddTwoInts>(name, config, params)
  {}

  /**
   * @brief Required method that creates list of BT ports
   *
   * Must call providedBasicPorts in it.
   *
   * @return PortsList Containing basic ports along with node-specific ports
   */
  static BT::PortsList providedPorts()
  {    
    return providedBasicPorts({ 
        BT::InputPort<unsigned>(
            "modelA",
            "First model"),
        BT::InputPort<unsigned>(
            "modelB",
            "Second model"),
        BT::OutputPort<unsigned>(
            "modelC",
            "Resulting model")
        });
  }

  void set_coresense_parameter();
  /** 
   * @brief Required callback that allows the user to set the goal message.
   *
   * @param goal The goal to be sent to the action server.
   *
   * @return false if the request should not be sent. In that case,
   * RosActionNode::onFailure(INVALID_GOAL) will be called.
   */
  bool setRequest(Request::SharedPtr& request) override;

  /** 
   * @brief Required callback invoked when the result is received by the server.
   * 
   * It is up to the user to define if the action returns SUCCESS or FAILURE.
   */
  BT::NodeStatus onResponseReceived(const Response::SharedPtr& response) override;

  /**
   * @brief Optional callback invoked when something goes wrong. 
   *
   * It must return either SUCCESS or FAILURE.
   */
  BT::NodeStatus onFailure(BT::ServiceNodeErrorCode error) override;


private:

};

#endif
