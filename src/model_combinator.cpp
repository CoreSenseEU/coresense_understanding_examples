

#include "behaviortree_ros2/plugins.hpp"

#include <coresense_understanding_examples/model_combinator.hpp>

void ModelCombinatorNode::set_coresense_parameter() {
  const std::string path = ament_index_cpp::get_package_share_directory("coresense_understanding_examples") + "/config/model_source.json";
  const std::ifstream input_stream(path, std::ios_base::binary);

  if (input_stream.fail()) {
    throw std::runtime_error("Failed to open model_source.json");
  }
  std::stringstream buffer;
  buffer << input_stream.rdbuf();
  if(auto node = node_.lock()) {
    node->declare_parameter("coresense_engine", buffer.str());
  }
}
  /** 
   * @brief Required callback that allows the user to set the goal message.
   *
   * @param goal The goal to be sent to the action server.
   *
   * @return false if the request should not be sent. In that case,
   * RosActionNode::onFailure(INVALID_GOAL) will be called.
   */
bool ModelCombinatorNode::setRequest(Request::SharedPtr& request) {
  getInput("modelA", request->a);
  getInput("modelB", request->b);
  // must return true if we are ready to send the request
  return true;
}

  /** 
   * @brief Required callback invoked when the result is received by the server.
   * 
   * It is up to the user to define if the action returns SUCCESS or FAILURE.
   */
BT::NodeStatus ModelCombinatorNode::onResponseReceived(const Response::SharedPtr& response) {
  setOutput("modelC", response->sum);
  return BT::NodeStatus::SUCCESS;
}

  /**
   * @brief Optional callback invoked when something goes wrong. 
   *
   * It must return either SUCCESS or FAILURE.
   */
BT::NodeStatus ModelCombinatorNode::onFailure(BT::ServiceNodeErrorCode error) {
    RCLCPP_ERROR(logger(), "Error: %d", error);
    return BT::NodeStatus::FAILURE;
}


// Register this node as a plugin with the BT factory
CreateRosNodePlugin(ModelCombinatorNode, "ModelCombinator");
