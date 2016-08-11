# parking managment
roslaunch parking_management display.launch

rostopic pub -r 1 /free_parking_place std_msgs/Int16 "{data: 0}"
