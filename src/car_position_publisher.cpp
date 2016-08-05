#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/String.h>

std::string car;



void poseCallback(const std_msgs::String& msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(8.0, 8.0 , 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, 0);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "drived_car/body_link"));

  transform.setOrigin( tf::Vector3(0.0, 8.0 , 0.0) );
  q.setRPY(0, 0, 0);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "parked_car/body_link"));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");

  car="body_link";

  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("free_parking_place", 10, &poseCallback);

  ros::spin();
  return 0;
};