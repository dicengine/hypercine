#include "hypercine.h"

#include <opencv2/opencv.hpp>

#include <iostream>

using namespace hypercine;

int main(int argc, char *argv[]) {

  int error_count = 0;
  try {
    // create an opencv image by reading in an existing tiff
    cv::Mat base_img = cv::imread("./images/packed_12bpp_frame_60.tiff",cv::IMREAD_GRAYSCALE);
    ASSERT_EXPR(base_img.type()==CV_8UC1,error_count);

    // write one image to clear out the output cine file if it exists
    const std::string file_name = "test_write8bpp.cine";
    HyperCine::write_frame(file_name.c_str(),base_img.cols,base_img.rows,(uint8_t*)base_img.data, true);
    std::vector<cv::Mat> gold_imgs;
    gold_imgs.push_back(base_img);
    for(size_t i=0;i<10;++i){
      cv::Mat img = base_img.clone();
      std::stringstream img_text;
      img_text << "image_" << i << ".png";
      cv::putText(img,img_text.str(),cv::Point(10, img.rows / 2),cv::FONT_HERSHEY_DUPLEX,1.0,cv::Scalar(0),2);
      gold_imgs.push_back(img);
//      cv::imwrite(img_text.str(),img);
      HyperCine::write_frame(file_name.c_str(),img.cols,img.rows,(uint8_t*)img.data);
    }
    // test file size
    const int header_size = 36 + sizeof(HyperCine::TIME64);
    const int bitmap_header_size = 40;
    const int offset_storage_size = MAX_WRITE_FRAMES * 8;
    const int per_image_storage = base_img.rows * base_img.cols; // FIXME factor in bit depth, this assumes 8 bit
    const int total_file_size = header_size + bitmap_header_size + offset_storage_size + per_image_storage*11;
    std::ifstream cine_file(file_name.c_str(), std::ifstream::ate | std::ifstream::binary);
    const int file_size = cine_file.tellg();
    ASSERT_EXPR(file_size==total_file_size,error_count);
    cine_file.close();

    // read in the cine file and compare images to gold images
    HyperCine hc(file_name.c_str());
    HyperCine::HyperFrame hf;
    hf.add_frames(0,gold_imgs.size());
    hc.read_buffer(hf);
    std::cout << hc << std::endl;

    // test images compared to gold images
    for(int i=0;i<(int)gold_imgs.size();++i){
      cv::Mat re_read_image(hc.height(0),hc.width(0),CV_8UC1,hc.data_8(i,0));
      cv::Mat img_diff;
      double min_diff, max_diff;
      cv::Point min_loc, max_loc;
      cv::absdiff(gold_imgs[i],re_read_image,img_diff);
      cv::minMaxLoc(img_diff,&min_diff,&max_diff,&min_loc,&max_loc);
      std::cout << "frame " << i << " max diff: " << max_diff << std::endl;
      ASSERT_EXPR(max_diff==0,error_count);
//      std::stringstream img_text;
//      img_text << "re_read_image_" << i-1 << ".png";
//      cv::imwrite(img_text.str(),re_read_image);
    }
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
