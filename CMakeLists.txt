cmake_minimum_required(VERSION 3.8)
project(coresense_understanding_examples)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_index_cpp REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(behaviortree_ros2 REQUIRED)
find_package(btcpp_ros2_interfaces REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(triplestar_msgs REQUIRED)
find_package(coresense_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(pluginlib REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()
  #behaviortree_cpp::behaviortree_cpp

set(DEPENDENCIES
  ${triplestar_msgs_TARGETS}
  ${coresense_msgs_TARGETS}
  behaviortree_ros2::behaviortree_ros2
  ${btcpp_ros2_interfaces_TARGETS}
  ${example_interfaces_TARGETS}
)


# ModelSourceExample BT::ActionNode (Plugin version)
add_library(model_source_plugin SHARED src/model_source.cpp)
target_compile_definitions(model_source_plugin PRIVATE  BT_PLUGIN_EXPORT )
target_link_libraries(model_source_plugin
  ${pluginlib_TARGETS}
  ${DEPENDENCIES}
  ament_index_cpp::ament_index_cpp
)

target_include_directories(model_source_plugin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
#ament_target_dependencies(model_source_plugin ${DEPENDENCIES})


# ModelCombinatorExample BT::ActionNode (Plugin version)
add_library(model_combinator_plugin SHARED src/model_combinator.cpp)
target_compile_definitions(model_combinator_plugin PRIVATE BT_PLUGIN_EXPORT)
target_link_libraries(model_combinator_plugin 
  ${pluginlib_TARGETS}
  ${DEPENDENCIES}
  ament_index_cpp::ament_index_cpp
)
target_include_directories(model_combinator_plugin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
#ament_target_dependencies(model_combinator_plugin ${DEPENDENCIES})



add_executable(usage_example_node src/usage_example.cpp)
target_include_directories(usage_example_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(usage_example_node PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

target_link_libraries(usage_example_node PUBLIC
  ${std_srvs_TARGETS}
  ${coresense_msgs_TARGETS}
  ${triplestar_msgs_TARGETS}
  ${btcpp_ros2_interfaces_TARGETS}
  ament_index_cpp::ament_index_cpp
  rclcpp::rclcpp
  rclcpp_action::rclcpp_action
  )

###############
### Install targets and directories
install(TARGETS
  model_source_plugin
  model_combinator_plugin
  usage_example_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  behavior_trees
  config
  # launch
  DESTINATION share/${PROJECT_NAME}
)


# Install plugins for other packages to load
install(TARGETS
  model_source_plugin
  model_combinator_plugin
  LIBRARY DESTINATION share/${PROJECT_NAME}/bt_plugins
  ARCHIVE DESTINATION share/${PROJECT_NAME}/bt_plugins
  RUNTIME DESTINATION share/${PROJECT_NAME}/bt_plugins
)


# Install headers for plugins
install(DIRECTORY
  include/
  DESTINATION include
)

# Export plugin dependencies for downstream packages
ament_export_dependencies(${DEPENDENCIES})
ament_export_include_directories(
  include
)

ament_package()
