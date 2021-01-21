#include "hypercine.h"

#include <opencv2/opencv.hpp>

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_count = 0;
  try {
    HyperCine hc("./images/example_8bpp.cine");
    HyperCine::HyperFrame hf(0,11);
    // don't add any regions of interest so the whole image is read on the first iteration
    hc.read_buffer(hf);
    cv::Mat img(hc.height(),hc.width(),CV_16UC1,hc.data(10));
    // cv::imwrite("example_8bpp_frame_11.tiff",img);

    // read the gold file
    cv::Mat gold_img = cv::imread("./images/example_8bpp_frame_11.tiff",cv::IMREAD_GRAYSCALE);
    gold_img.convertTo(gold_img, CV_16UC1);
    cv::Mat img_diff;
    cv::absdiff(gold_img,img,img_diff);
    double min_diff, max_diff;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "frame 11 max diff: " << max_diff << std::endl;
    ASSERT_EXPR(max_diff==0,error_count);

    // now try reading a small region of interest from the same file
    hf.add_window(70,100,30,60);
    hc.read_buffer(hf);
    std::cout << hc << std::endl;
    if(!hc.valid_frame_window(10,0)){
      std::cout << "invalid frame or window" << std::endl;
      error_count ++;
    }
    cv::Mat img_roi_0(hc.height(0),hc.width(0),CV_16UC1,hc.data(10,0));
    // cv::imwrite("example_8bpp_frame_10_roi_0.tiff",img_roi_0);

    // read the gold file and compare
    cv::Mat gold_img_roi_0 = cv::imread("./images/example_8bpp_frame_10_roi_0.tiff",cv::IMREAD_GRAYSCALE);
    gold_img_roi_0.convertTo(gold_img_roi_0, CV_16UC1);
    cv::Mat img_diff_roi_0;
    cv::absdiff(gold_img_roi_0,img_roi_0,img_diff_roi_0);
    cv::minMaxLoc(img_diff_roi_0,&min_diff,&max_diff,&min_loc,&max_loc);
    std::cout << "frame 10 roi 0 max diff: " << max_diff << std::endl;
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

