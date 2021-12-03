#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <ros/console.h>
#include <ros/package.h>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>

/////////////////////////////////
#include "ros_tengine/Object.h"
#include "timer.hpp"
#include "yolo.hpp"
/////////////////////////////////

ros_tengine::Object objMsg;
ros::Publisher obj_pub;
std::vector<Object> objects;
cv_bridge::CvImagePtr cv_ptr;
ros::Time last_time;
bool display_output;
double prob_threshold;

#define MODE_WIDTH 416
#define MODE_HEIGHT 416
// #define DET_MODEL "assets/models/yolov3_uint8.tmfile"
std::array<float, 3> image_scale = {1.f / 255.f, 1.f / 255.f, 1.f / 255.f};
std::array<float, 3> image_shift = {0.f, 0.f, 0.f};
const std::string package_name = "ros_tengine";
std::string DET_MODEL = ros::package::getPath(package_name) + ("/assets/models/yolov3_uint8.tmfile");

YOLO detector(DET_MODEL, MODE_WIDTH, MODE_HEIGHT, image_scale, image_shift);

void imageCallback(const sensor_msgs::ImageConstPtr &msg, int n_threads)
{
  try
  {
    ros::Time current_time = ros::Time::now();
    // Do Yolo v3 Detect
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    detector.detect(cv_ptr->image, objects);
    ROS_INFO("Detect");

    for (size_t i = 0; i < objects.size(); i++)
    {
      const Object &obj = objects[i];
      if (obj.score > prob_threshold)
      {
        ROS_INFO("%s \t = %.5f at %.2f %.2f %.2f x %.2f", class_names[obj.label], obj.score,
                 obj.box.x, obj.box.y, obj.box.width, obj.box.height);
        objMsg.header.seq++;
        objMsg.header.stamp = current_time;
        objMsg.probability = obj.score;
        objMsg.label = class_names[obj.label];
        objMsg.boundingbox.position.x = obj.box.x;
        objMsg.boundingbox.position.y = obj.box.y;
        objMsg.boundingbox.size.x = obj.box.width;
        objMsg.boundingbox.size.y = obj.box.height;
        obj_pub.publish(objMsg);
      }
    }

    if (display_output)
    {
      //   engine.draw(cv_ptr->image, objects, (current_time-last_time).toSec());
      for (auto &object : objects)
      {
        // box
        cv::Rect2f rect(object.box.x, object.box.y, object.box.width, object.box.height);

        const unsigned char *color = colors[object.label];

        cv::rectangle(cv_ptr->image, rect, cv::Scalar(color[0], color[1], color[2]), 2);

        char text[256];
        sprintf(text, "%s %.1f%%", class_names[object.label], object.score * 100);

        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

        int x = rect.x;
        int y = rect.y - label_size.height - baseLine;
        if (y < 0)
          y = 0;
        if (x + label_size.width > cv_ptr->image.cols)
          x = cv_ptr->image.cols - label_size.width;

        cv::rectangle(cv_ptr->image, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                      cv::Scalar(255, 255, 255), -1);

        cv::putText(cv_ptr->image, text, cv::Point(x, y + label_size.height), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    cv::Scalar(0, 0, 0));
      }
      cv::imshow("YOLO v3", cv_ptr->image);
      cv::waitKey(1);
    }
    last_time = current_time;
  }
  catch (cv_bridge::Exception &e)
  {
    ROS_ERROR("CV bridge exception: %s", e.what());
    return;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "yolov3_node"); /**/
  ros::NodeHandle nhLocal("~");
  ros::NodeHandle n;
  std::string node_name = ros::this_node::getName();

  nhLocal.param("display_output", display_output, true);
  obj_pub = n.advertise<ros_tengine::Object>(node_name + "/objects", 50);

  image_transport::ImageTransport it(n);
  image_transport::Subscriber video = it.subscribe("/usb_cam/image_raw", 1, boost::bind(&imageCallback, _1, 1));

  while (ros::ok())
  {
    ros::spinOnce();
  }

  return 0;
}
