#include "hypercine.h"

#include <opencv2/opencv.hpp>

#include <iostream>

using namespace hypercine;

//
//  NOTE: for the example 16bpp .cine files, the values are actually 12bit values (0-4095)
//        they need to be scaled in order to be visible in a 16bit image which should have values (0-65536)
//

int main(int argc, char *argv[]) {

  int error_count = 0;
  try {
    HyperCine hc("./images/phantom_v7_raw_16bpp.cine",hypercine::HyperCine::TO_8_BIT);
    ASSERT_EXPR(hc.frame_rate()==80,error_count);
    HyperCine::HyperFrame hf(238292,6);
    // don't add any regions of interest so the whole image is read on the first iteration
    hc.read_buffer(hf);
    cv::Mat img(hc.height(),hc.width(),hc.opencv_data_type(),hc.data(238292));
    // truncate the values in the Mat so they are integer numbers
    for(int y=0;y<img.rows;++y){
      for(int x=0;x<img.cols;++x){
        img.at<storage_t>(y,x) =  std::floor(img.at<storage_t>(y,x));
      }
    } // then convert to 8 bit values
    img.convertTo(img,CV_8UC1);//,255.0/hc.max_possible_intensity());
    //cv::imwrite("phantom_v7_raw_16bpp_frame_238292.tiff",img);

    // read the gold file
    cv::Mat gold_img = cv::imread("./images/phantom_v7_raw_16bpp_frame_238292.tiff",cv::IMREAD_GRAYSCALE);
    //cv::imwrite("phantom_v7_raw_16bpp_frame_238292.tiff",gold_img);
    cv::Mat img_diff;
    cv::absdiff(gold_img,img,img_diff);
    double min_diff, max_diff;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "frame 238292 max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);

    // now try reading a small region of interest from the same file
    hf.add_window(38,25,20,17);
    hc.read_buffer(hf);
    std::cout << hc << std::endl;
    if(!hc.buffer_has_frame(238297)){
      std::cout << "invalid frame or window" << std::endl;
      error_count ++;
    }
    if(hc.buffer_has_frame(238298)){
      std::cout << "invalid frame or window should have been caught" << std::endl;
      error_count ++;
    }
    cv::Mat img_roi_0(hc.height(0),hc.width(0),hc.opencv_data_type(),hc.data(238292,0));
    // truncate the values in the Mat so they are integer numbers
    for(int y=0;y<img_roi_0.rows;++y){
      for(int x=0;x<img_roi_0.cols;++x){
        img_roi_0.at<storage_t>(y,x) =  std::floor(img_roi_0.at<storage_t>(y,x));
      }
    } // then convert to 8 bit values
    img_roi_0.convertTo(img_roi_0,CV_8UC1,255.0/hc.max_possible_intensity());
    //cv::imwrite("phantom_v7_raw_16bpp_frame_238292_roi_0.tiff",img_roi_0);

    // read the gold file and compare
    cv::Mat gold_img_roi_0 = cv::imread("./images/phantom_v7_raw_16bpp_frame_238292_roi_0.tiff",cv::IMREAD_GRAYSCALE);
    //cv::imwrite("phantom_v7_raw_16bpp_frame_238292_roi_0.tiff",gold_img_roi_0);
    cv::Mat img_diff_roi_0;
    cv::absdiff(gold_img_roi_0,img_roi_0,img_diff_roi_0);
    cv::minMaxLoc(img_diff_roi_0,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "frame 238292 roi 0 max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);

  } catch(std::exception& e) {
    std::cerr << "test failed due to exception thrown" << std::endl;
    std::cerr << e.what() << std::endl;
    error_count++;
  }
  if (error_count != 0)
    std::cout << "TEST FAILED\n";
  else
    std::cout << "TEST PASSED\n";

  return 0;

}

