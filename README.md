## CoreSense Understanding Examples

### Purpose

This module supplies examples of engines and usage of the CoreSense Understanding System

### Installation

#### Scripted
```bash
git clone https://github.com/CoreSenseEU/coresense_understanding_examples src/coresense_understanding_examples
git clone https://github.com/CoreSenseEU/coresense_understanding_bringup src/coresense_understanding_bringup
git clone https://github.com/CoreSenseEU/decision_system src/decision_system
cd coresense_understanding_bringup
./install.sh
cd ../coresense_understanding
./install.sh
cd ../triplestar_kb
./install.sh
cd ../decision_system
./install.sh
cd ../..
colcon build --symlink-install
source install/setup.bash
```

#### Manual
```bash
git clone https://github.com/CoreSenseEU/coresense_understanding_examples src/coresense_understanding_examples
git clone https://github.com/CoreSenseEU/coresense_understanding_bringup src/coresense_understanding_bringup
git clone https://github.com/CoreSenseEU/coresense_bringup src/coresense_bringup
git clone https://github.com/CoreSenseEU/decision_system src/decision_system
git clone https://github.com/CoreSenseEU/coresense_bt_controller src/coresense_bt_controller
git clone --branch=parallel_goals https://github.com/alex-quin-gabriel/BehaviorTree.ROS2.git src/BehaviorTree.ROS2
git clone --recursive https://github.com/CoreSenseEU/coresense_understanding src/coresense_understanding
git clone https://github.com/CoreSenseEU/coresense_msgs src/coresense_msgs
git clone https://github.com/CoreSenseEU/coresense_vampire src/coresense_vampire
git clone https://github.com/CoreSenseEU/triplestar_kb src/triplestar_kb
#dependencies for coresense_understanding:
sudo apt install -y --no-install-recommends python3-pip ros-jazzy-tinyxml2-vendor ros-jazzy-nlohmann-json-schema-validator-vendor swi-prolog
# dependencies for triplestar_kb:
pip install --user --break-system-packages pydantic pyoxigraph reasonable oxrdflib shapely copier returns pyswip
colcon build --symlink-install
source install/setup.bash
```


#### Dependencies
- tinyxml2_vendor
- nlohmann_json_schema_validator_vendor
- swi-prolog
- pyswig
- [BehaviorTree.ROS2](https://github.com/alex-quin-gabriel/BehaviorTree.ROS2.git)
- [decision_system](https://github.com/CoreSenseEU/decision_system)
- [coresense_understanding](https://github.com/CoreSenseEU/coresense_understanding)
- [coresense_bt_controller](https://github.com/CoreSenseEU/coresense_bt_controller)
- [understanding-logic](https://github.com/CoreSenseEU/understanding-logic)
- [coresense_msgs](https://github.com/CoreSenseEU/coresense_msgs)
- [coresense_vampire](https://github.com/CoreSenseEU/coresense_vampire)
- [triplestar_kb](https://github.com/CoreSenseEU/triplestar_kb)

This should work in any ROS2 distribution.

### Usage

1. Run the decision system
```bash
ros2 launch coresense_understanding_bringup coresense_decision_system.launch.py
```


2. Run the bt_controller
```bash
ros2 launch coresense_bringup bt_controller.launch.py
```

3. Run the understanding system
```bash
ros2 launch coresense_understanding_bringup coresense_understanding.launch.py
```

4. Run the example
```bash
ros2 run coresense_understanding_examples usage_example_node
```
