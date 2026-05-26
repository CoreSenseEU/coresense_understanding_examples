

#include "behaviortree_ros2/plugins.hpp"

#include <coresense_understanding_examples/model_source.hpp>

void ModelSourceNode::set_coresense_parameter() {
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
bool ModelSourceNode::setRequest(Request::SharedPtr& request) {
  coresense_msgs::msg::ModelDescription description;
  getInput("requirements", description);
  request->query = R"A(PREFIX xsd:  <http://www.w3.org/2001/XMLSchema#>
PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>
PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>
PREFIX cso: <https://github.com/CoreSenseEU/CoreSense/blob/main/models/ontology/coresense_ontologies/CSO/cso.sysml#>
PREFIX csu: <https://github.com/CoreSenseEU/coresense_understanding_bringup/blob/main/preload/understanding.ttl#>

SELECT *
WHERE {
  ?modelet rdf:type cso:Modelet;
    csu:hasFormalism ?formalism.
  OPTIONAL { ?modelet csu:modelsConcept ?concept. }
  OPTIONAL { ?modelet csu:hasRepresentationClass ?representationClass. }
  OPTIONAL {
    ?modelet csu:hasProperty ?property.
    ?property csu:hasName ?propertyName;
              csu:hasType ?propertyType;
              csu:hasValue ?propertyValue.
  }
}

#  OPTIONAL { ?modelet csu:hasCreator ?creator. }
#  OPTIONAL { ?modelet csu:hasLocation ?location. }
#  OPTIONAL { ?modelet csu:hasScope ?scope. })A";
  //TODO convert 
  // must return true if we are ready to send the request
  return true;
}

  /** 
   * @brief Required callback invoked when the result is received by the server.
   * 
   * It is up to the user to define if the action returns SUCCESS or FAILURE.
   */
BT::NodeStatus ModelSourceNode::onResponseReceived(const Response::SharedPtr& response) {
  std::string json = response->result;
  //TODO construct models from result

  coresense_msgs::msg::Models models;
  coresense_msgs::msg::ModelDescription description;
  description.id = "id";
  description.creation_date = "date";
  description.location = "location";
  description.extent = "extent";
  description.concepts.push_back("concept");
  description.representation_classes.push_back("repclass");
  coresense_msgs::msg::Property property;
  description.properties.push_back(property);



  models.models.push_back(description);
  setOutput("models", models);
  return BT::NodeStatus::SUCCESS;
}

  /**
   * @brief Optional callback invoked when something goes wrong. 
   *
   * It must return either SUCCESS or FAILURE.
   */
BT::NodeStatus ModelSourceNode::onFailure(BT::ServiceNodeErrorCode error) {
    RCLCPP_ERROR(logger(), "Error: %d", error);
    return BT::NodeStatus::FAILURE;

}


// Register this node as a plugin with the BT factory
CreateRosNodePlugin(ModelSourceNode, "ModelSource");
