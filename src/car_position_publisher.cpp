#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include <fstream>
#include <iostream>
#include <Eigen/Eigen>
class parking
{
  private:
    // the node handle
    ros::NodeHandle nh_;
    int max_id;
    std::string map_file_path;
    Eigen::MatrixXd map_points;
    void read_map_coordinates(std::string map_file_path,int max_id);
    void poseCallback(const std_msgs::Int16& msg);
    ros::Subscriber sub;
    

  public:
    // constructor
    parking(ros::NodeHandle nh) : nh_(nh)
    {
        nh_.param<std::string>("map_file", map_file_path, "");
        nh_.param<int>("max_id", max_id, 3);
        read_map_coordinates(map_file_path,max_id);
        sub = nh_.subscribe(nh.resolveName("free_parking_place"), 10, &parking::poseCallback,this);
    }
};

void parking::read_map_coordinates(std::string map_file_path,int max_id)
{
    std::ifstream f_in(map_file_path.c_str());
    int id;
    map_points=Eigen::MatrixXd::Zero(max_id+1,6);

    if (!f_in)
        ROS_ERROR("Error in opening file %s",map_file_path.c_str());
     //ROS_ERROR("Error in reading file %s",f_in>>);
    while(f_in>>id)
    {   

        if (!(f_in>>map_points(id,0)>>map_points(id,1)>>map_points(id,2)>>map_points(id,3)>>map_points(id,4)>>map_points(id,5)))
            ROS_ERROR("Error in reading file %s",map_file_path.c_str());

        std::cout<<id<<" "<<map_points(id,0)<<" "<<map_points(id,1)<<" "<<std::endl;
    }
}


void parking::poseCallback(const std_msgs::Int16& msg){

  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(map_points(msg.data,0), map_points(msg.data,1) , map_points(msg.data,2)) );
  tf::Quaternion q;
  q.setRPY(map_points(msg.data,3), map_points(msg.data,4), map_points(msg.data,5));
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "drived_car/body_link"));

  // if (msg.data>(max_id-1))
  //   parked_id=
  // transform.setOrigin( tf::Vector3(0.0, 8.0 , 0.0) );
  // q.setRPY(0, 0, 0);
  // transform.setRotation(q);
  // br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "parked_car/body_link"));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "parking");

  ros::NodeHandle node;
  parking parking_node(node);

  ros::spin();
  return 0;
};