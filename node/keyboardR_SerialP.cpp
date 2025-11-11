#include <memory>
#include <string>

#include <iostream>
#include <fcntl.h>    // File control definitions
#include <termios.h>  // POSIX terminal control definitions
#include <unistd.h>   // UNIX standard function definitions


#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"


using std::placeholders::_1;

class SerialP : public rclcpp::Node
{
public:
  SerialP()
  : Node("SerialP")
  {
    // Create subscriber to /cmd_vel_key
    subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel_key",           // topic name
      10,                       // QoS history depth
      std::bind(&SerialP::cmdVelCallback, this, std::placeholders::_1)
    );

    RCLCPP_INFO(this->get_logger(), "CmdVelKeyListener node started. Listening to /cmd_vel_key...");
  }
private:

  void cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
  {
    RCLCPP_INFO(this->get_logger(),
      "Received /cmd_vel_key: linear=(%.2f, %.2f, %.2f), angular=(%.2f, %.2f, %.2f)",
      msg->linear.x, msg->linear.y, msg->linear.z,
      msg->angular.x, msg->angular.y, msg->angular.z);

    int fd = open("/dev/ttyUSB0", O_RDWR);

    if (fd < 0) {
      RCLCPP_INFO(this->get_logger(), "success %d", fd);
    }
  }
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SerialP>());
  rclcpp::shutdown();
  return 0;
}