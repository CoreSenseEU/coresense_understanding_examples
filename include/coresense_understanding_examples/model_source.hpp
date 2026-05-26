#ifndef CS_UNDERSTANDING_EXAMPLES_MODEL_SOURCE_HPP_
#define CS_UNDERSTANDING_EXAMPLES_MODEL_SOURCE_HPP_

#include "ament_index_cpp/get_package_share_directory.hpp"
#include <behaviortree_ros2/bt_service_node.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "coresense_msgs/msg/model_description.hpp"
#include "coresense_msgs/msg/models.hpp"
#include "triplestar_msgs/srv/sparql_query.hpp"


class ModelSourceNode : public BT::RosServiceNode<triplestar_msgs::srv::SPARQLQuery>
{
public:
  explicit ModelSourceNode(const std::string& name, const BT::NodeConfig& config,
               const BT::RosNodeParams& params)
    : BT::RosServiceNode<triplestar_msgs::srv::SPARQLQuery>(name, config, params)
  {
    setServiceName("/triplestar_core/query");
  }

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
        BT::InputPort<coresense_msgs::msg::ModelDescription>(
            "requirements",
            "Semantic requirements of the model to get."),
        BT::OutputPort<coresense_msgs::msg::Models>(
            "models",
            "An array of models that meet the requirements.")
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
