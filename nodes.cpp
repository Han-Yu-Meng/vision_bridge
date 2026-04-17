/*******************************************************************************
 * Copyright (c) 2025.
 * IWIN-FINS Lab.
 ******************************************************************************/

// nodes.cpp

#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/msg/image.hpp>
#include <string>
#include <stdexcept>
#include <fins/functional_node.hpp>

using fins::Input;
using fins::Output;
using fins::Parameter;

static auto opencv_to_ros = fins::Function("OpenCV2ROS",
  [](Input<cv::Mat> &input,
     Output<sensor_msgs::msg::Image> &output,
     Parameter<std::string> &encoding
    ) {
    
    if (input->empty()) {
      throw std::invalid_argument("Input OpenCV image is empty");
    }

    std_msgs::msg::Header header;
    header.frame_id = "fins_camera"; 
    
    try {
      cv_bridge::CvImage cv_image(header, encoding, *input);
      cv_image.toImageMsg(*output);
    } catch (cv_bridge::Exception& e) {
      throw std::runtime_error("cv_bridge exception: " + std::string(e.what()));
    }
  })
  .with_description("Converts an OpenCV cv::Mat to a ROS2 sensor_msgs::Image")
  .with_inputs_description({"image"})
  .with_outputs_description({"ros_image"})
  .with_parameter<std::string>("encoding", "bgr8")
  .with_category("Vision>Bridge")
  .build();

static auto ros_to_opencv = fins::Function("ROS2OpenCV",
  [](Input<sensor_msgs::msg::Image> &input,
     Output<cv::Mat> &output,
     Parameter<std::string> &encoding
    ) {
    
    if (input->data.empty()) {
      throw std::invalid_argument("Input ROS image message is empty");
    }

    std::string enc = encoding;
    if (enc.empty()) enc = "bgr8";

    try {
      auto cv_ptr = cv_bridge::toCvCopy(input.ptr(), enc);
      
      if (cv_ptr->image.empty()) {
        throw std::runtime_error("Converted OpenCV image is empty");
      }
      
      output = cv_ptr->image;
    } catch (cv_bridge::Exception& e) {
      throw std::runtime_error("cv_bridge exception: " + std::string(e.what()));
    }
  })
  .with_description("Converts a ROS 2 sensor_msgs::Image to an OpenCV cv::Mat")
  .with_inputs_description({"ros_image", "encoding"})
  .with_outputs_description({"image"})
  .with_parameter<std::string>("encoding", "bgr8")
  .with_category("Vision>Bridge")
  .build();
  
DEFINE_PLUGIN_ENTRY()