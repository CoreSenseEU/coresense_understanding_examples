#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <exception>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "ament_index_cpp/get_package_share_directory.hpp"

#include "std_srvs/srv/trigger.hpp"

#include "coresense_msgs/action/understand.hpp"
#include "coresense_msgs/srv/start_session.hpp"
#include "coresense_msgs/srv/end_session.hpp"
#include <btcpp_ros2_interfaces/action/execute_tree.hpp>

#include "triplestar_msgs/srv/sparql_query.hpp"
#include "triplestar_msgs/srv/select_query.hpp"

using namespace std::chrono_literals;



namespace coresense::understanding_examples::usage_example {
class UsageExampleNode : public rclcpp::Node {
  using UnderstandAction = coresense_msgs::action::Understand;
  using UnderstandActionGoalHandle = rclcpp_action::ClientGoalHandle<UnderstandAction>;
  using ExecuteTreeAction = btcpp_ros2_interfaces::action::ExecuteTree;
  using ExecuteTreeActionGoalHandle = rclcpp_action::ClientGoalHandle<ExecuteTreeAction>;

public:
  UsageExampleNode() : Node("understanding_user") {
    set_coresense_parameter();
    if (true) {
      start_session_client_ptr = create_client<coresense_msgs::srv::StartSession>("/understanding/start_session");
      end_session_client_ptr = create_client<coresense_msgs::srv::EndSession>("/understanding/end_session");
      mark_agent_model_dirty_client_ptr = create_client<std_srvs::srv::Trigger>("/understanding/mark_agent_model_dirty");
      mark_knowledge_model_dirty_client_ptr = create_client<std_srvs::srv::Trigger>("/understanding/mark_knowledge_model_dirty");
      //query_triplestar_kb_ptr = create_client<triplestar_msgs::srv::SPARQLQuery>("/triplestar_core/query");
      //get_modelets_client_ptr = create_client<triplestar_msgs::srv::SelectQuery>("/triplestar_core/query_services/get_modelets");
      understanding_action_client_ptr = rclcpp_action::create_client<UnderstandAction>(this, "/understanding/understand");
      execute_tree_client_ptr = rclcpp_action::create_client<btcpp_ros2_interfaces::action::ExecuteTree>(this, "/coresense/run_bt");
      // wait for clients to become available 
      while (!start_session_client_ptr->wait_for_service(1s))
 //        && !end_session_client_ptr->wait_for_service(1s) 
      {
        if (!rclcpp::ok()) {
          RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the start_session/end_session services. Exiting.");
          return; //TODO: find better way to kill this.
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "/understanding/start_session service not available, waiting again...");
      }
      RCLCPP_INFO(get_logger(), "Connected to coresense_understanding node");
    }
    run_example();
  }

private:
  rclcpp::Client<coresense_msgs::srv::StartSession>::SharedPtr start_session_client_ptr;
  rclcpp::Client<coresense_msgs::srv::EndSession>::SharedPtr end_session_client_ptr;
  rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr mark_agent_model_dirty_client_ptr;
  rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr mark_knowledge_model_dirty_client_ptr;
  //rclcpp::Client<triplestar_msgs::srv::SPARQLQuery>::SharedPtr query_triplestar_kb_ptr;
  rclcpp_action::Client<UnderstandAction>::SharedPtr understanding_action_client_ptr;
  rclcpp_action::Client<ExecuteTreeAction>::SharedPtr execute_tree_client_ptr;



  // Methods
  void set_coresense_parameter() {
    const std::string path = ament_index_cpp::get_package_share_directory("coresense_understanding") + "/config/coresense_engine.json";
    const std::ifstream input_stream(path, std::ios_base::binary);

    if (input_stream.fail()) {
      throw std::runtime_error("Failed to open coresense_engine.json");
    }
    std::stringstream buffer;
    buffer << input_stream.rdbuf();
    declare_parameter("coresense_engine", buffer.str());
  }
  


  void run_example() {
    //1. start service, get id
    auto request = std::make_shared<coresense_msgs::srv::StartSession::Request>();
    auto callback = [this](rclcpp::Client<coresense_msgs::srv::StartSession>::SharedFuture session_future) {
      //2. run understanding action
      auto goal_msg = UnderstandAction::Goal();
      goal_msg.session_id = session_future.get()->session_id;
      goal_msg.target_modelet = "?[M : modelet]: ( modelet_has_representation_class(M, decision_rc) & modelet_has_property(M, absolute_size_property) )";
      RCLCPP_INFO(get_logger(), "Sending understanding goal");


      auto send_goal_options = rclcpp_action::Client<UnderstandAction>::SendGoalOptions();
      send_goal_options.goal_response_callback = [this](const UnderstandActionGoalHandle::SharedPtr & goal_handle) {
        if (!goal_handle) {
          RCLCPP_ERROR(get_logger(), "Query goal was rejected by Understanding System");
          return;
        } else {
          RCLCPP_INFO(get_logger(), "Understanding goal accepted by Understanding System, waiting for result");
        }
      };

      send_goal_options.feedback_callback = [this](UnderstandActionGoalHandle::SharedPtr, const std::shared_ptr<const UnderstandAction::Feedback> feedback) { // this is so odd, but corresponds to documentation
        //std::stringstream ss;
        //ss << "Current state of reasoning is " << feedback->status;
        //RCLCPP_INFO(get_logger(), ss.str().c_str());
        //TODO PRIORITY forward feedback in some form
      };

      send_goal_options.result_callback = [this](const UnderstandActionGoalHandle::WrappedResult & wrapped_result) {
        RCLCPP_INFO(get_logger(), "Understanding result:\n%s", wrapped_result.result->result.c_str());
        //3. send BT to coresense_bt_controller to run
        //auto goal_msg = ExecuteTreeAction::Goal();
        //goal_msg.target_tree = wrapped_result.result->result;
        //goal_msg.payload = "";
        //RCLCPP_INFO(get_logger(), "Sending execute tree goal");
        //auto send_goal_options = rclcpp_action::Client<ExecuteTreeAction>::SendGoalOptions();
        //send_goal_options.goal_response_callback = [this](const ExecuteTreeActionGoalHandle::SharedPtr & goal_handle) {
        //  if (!goal_handle) {
        //    RCLCPP_ERROR(get_logger(), "BT execution goal was rejected by coresense_bt_controller");
        //    return;
        //  } else {
        //    RCLCPP_INFO(get_logger(), "BT execution goal accepted by coresense_bt_controller, waiting for result");
        //  }
//        };

 //       send_goal_options.feedback_callback = [this](ExecuteTreeActionGoalHandle::SharedPtr, const std::shared_ptr<const ExecuteTreeAction::Feedback> feedback) { // this is so odd, but corresponds to documentation
          //std::stringstream ss;
          //ss << "Current state of reasoning is " << feedback->status;
          //RCLCPP_INFO(get_logger(), ss.str().c_str());
          //TODO PRIORITY forward feedback in some form
//        };
//        send_goal_options.result_callback = [this](const ExecuteTreeActionGoalHandle::WrappedResult & wrapped_result) {
//          RCLCPP_INFO(get_logger(), "Tree execution result:\n%s", wrapped_result.result->return_message.c_str());
//        };
//        auto tree_execution_future =  execute_tree_client_ptr->async_send_goal(goal_msg, send_goal_options);

      }; 
      rclcpp::sleep_for(std::chrono::milliseconds(5000));
      auto understand_future =  understanding_action_client_ptr->async_send_goal(goal_msg, send_goal_options);
    };
    start_session_client_ptr->async_send_request(request, callback);
  }
  



};
}

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  RCLCPP_INFO(rclcpp::get_logger("Initializing Understanding User"), "pid: %i", getpid());
  rclcpp::executors::MultiThreadedExecutor executor;

  auto node = std::make_shared<coresense::understanding_examples::usage_example::UsageExampleNode>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
