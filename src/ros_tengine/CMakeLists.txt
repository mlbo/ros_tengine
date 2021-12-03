cmake_minimum_required(VERSION 3.0.2)
project(ros_tengine)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
  cv_bridge
  image_transport
  image_geometry
  sensor_msgs
  message_generation
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)
# add_library(tengine SHARED IMPORTED)
# set_target_properties(tengine PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libtengine-lite.so)


################################################
## Declare ROS messages ##
################################################

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  Vector2D.msg
  Rectangle.msg
  FaceObject.msg
  Object.msg
  Euler.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs#   std_msgs
)


###################################
## catkin specific configuration ##
###################################

catkin_package(
    CATKIN_DEPENDS
    roscpp
    roslib
    cv_bridge
    image_transport
    image_geometry
    sensor_msgs
    message_generation
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  
  ${catkin_INCLUDE_DIRS} include/ros_tengine
)
link_directories(
  ${catkin_LIB_DIRS} lib
)


# PFLD
add_executable(yolov3_node src/yolov3_node.cpp src/yolo_layer.cpp src/yolo.cpp src/timer.cc)
add_dependencies( yolov3_node ros_tengine_generate_messages_cpp)
target_link_libraries(yolov3_node ${catkin_LIBRARIES} tengine-lite ${OpenCV_LIBS})
# target_link_libraries(yolov3_node ${catkin_LIBRARIES} ${OpenCV_LIBS})


#############
## Install ##
#############



