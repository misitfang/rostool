#include "image_process.h"

using namespace cv;
image_process::image_process()
{
    init_ros();
};

void image_process::init_ros()
{   //nh.param()入口参数分别对应：参数服务器上的名称  参数变量名  初始值
    nh.param<std::string>("image_topic", image_topic, "/usb_cam2/image_raw");
    ROS_INFO("iamage_topic: %s", image_topic.c_str());
    sub = nh.subscribe<sensor_msgs::Image>(image_topic, 1000, &image_process::image_processCallback, this);

    output_pub = nh.advertise<sensor_msgs::Image>("/arucoimage", 10);
}
void image_process::image_processCallback(const sensor_msgs::ImageConstPtr &img)
{
    Mat image = cv_bridge::toCvShare(img)->image;
    nh.param<std::string>("image_topic", image_topic, "/usb_cam2/image_raw");
    nh.param<double>("camerMatrix", aa);
    nh.param<double>("disCoeffs", bb);

    Mat cameraMatrix = cv::Mat_<double>(3, 3) <<aa;
    Mat disCoeffs = Mat_<double>(4, 1) <<bb;

    // Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 283.63134, 0., 308.13219,
    //                     0., 283.09696, 255.16537,
    //                     0., 0., 1.);                                                  //相机内参
    // Mat disCoeffs = (Mat_<double>(4, 1) << -0.196668, 0.022176, 0.003651, -0.001057); //畸变
    aruco::CameraParameters camera_para;
    camera_para.setParams(cameraMatrix, disCoeffs, image.size());
    double aruco_size = 0.7;
    aruco::MarkerDetector Detector;
    Detector.setDictionary("ARUCO");
    Mat image_out = image;

    std::vector<aruco::Marker> markers;
    markers.clear();

    Detector.detect(image_out, markers, camera_para, aruco_size);
    for (size_t i = 0; i < markers.size(); i++)
    {
        markers[i].draw(image_out, cv::Scalar(0, 0, 255), 2);
        aruco::CvDrawingUtils::draw3dAxis(image_out, markers[i], camera_para);
    }

    sensor_msgs::ImagePtr output_msg = cv_bridge::CvImage(std_msgs::Header(), "rgb8", image_out).toImageMsg();
    output_pub.publish(output_msg);
}