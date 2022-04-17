#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>

#include <aruco/aruco.h>

class image_process
{
public:
    image_process();
    ros::NodeHandle nh;
    ros::Publisher output_pub;
    ros::Subscriber sub;
    std::string image_topic;
    double aa;
    double bb;
    //  cv::Mat cameraMatrix = cv::Mat(3, 3, CV_64F);
    //  cv::Mat disCoeffs = cv::Mat(4,1,CV_64F);
    // image_process(){
    //     // std::shared_ptr<image_process> ps(new image_process());
    //     sub = nh.subscribe<sensor_msgs::Image>("image_topic", 1000, &(image_process::image_processCallback));
    //     output_pub = nh.advertise<sensor_msgs::Image>("/grayimg", 10);
    // };
    void init_ros();
    void image_processCallback(const sensor_msgs::ImageConstPtr &img);
};



