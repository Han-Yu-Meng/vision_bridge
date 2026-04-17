cmake_minimum_required(VERSION 3.10)
project(vision_bridge)

find_package(OpenCV REQUIRED)

fins_add_node(${PROJECT_NAME} nodes.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

fins_link_ros_dependencies(${PROJECT_NAME} sensor_msgs cv_bridge)