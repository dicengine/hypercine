#include "hypercine.h"

#include <iostream>
#include <fstream>
#include <cassert>

namespace hypercine {

HyperCine::HyperCine(const char * file_name):
  file_name_(file_name),
  bit_depth_(NO_SUCH_BIT_DEPTH){
  read_header(file_name);
}

void
HyperCine::read_header(const char * file_name){

  std::ifstream cine_file(file_name, std::ios::in | std::ios::binary);
  if (cine_file.fail()){
    throw std::invalid_argument("invalid cine file " + (std::string)file_name);
  }
  cine_file.seekg(0, std::ios::end);
  long long int file_size = cine_file.tellg();
  cine_file.seekg(0, std::ios::beg);

  // CINE HEADER

  DEBUG_MSG("reading the cine header info: " << file_name);
  cine_file.read(reinterpret_cast<char*>(&header_.type), sizeof(header_.type));
  DEBUG_MSG("file size:            " << file_size);
  DEBUG_MSG("header type:          " << header_.type);
  cine_file.read(reinterpret_cast<char*>(&header_.header_size), sizeof(header_.header_size));
  DEBUG_MSG("header size:          " << header_.header_size);
  int test_size = 0;
  test_size += sizeof(header_.type);
  test_size += sizeof(header_.header_size);
  test_size += sizeof(header_.compression);
  test_size += sizeof(header_.version);
  test_size += sizeof(header_.first_movie_image);
  test_size += sizeof(header_.total_image_count);
  test_size += sizeof(header_.first_image_no);
  test_size += sizeof(header_.image_count);
  test_size += sizeof(header_.off_image_header);
  test_size += sizeof(header_.off_setup);
  test_size += sizeof(header_.off_image_offsets);
  test_size += sizeof(header_.trigger_time);
  DEBUG_MSG("test size:            " << test_size);
  assert(test_size==header_.header_size);
  cine_file.read(reinterpret_cast<char*>(&header_.compression), sizeof(header_.compression));
  DEBUG_MSG("header compression:   " << header_.compression);
  assert(header_.compression==0);
  cine_file.read(reinterpret_cast<char*>(&header_.version), sizeof(header_.version));
  DEBUG_MSG("header version:       " << header_.version);
  assert(header_.version==1);
  cine_file.read(reinterpret_cast<char*>(&header_.first_movie_image), sizeof(header_.first_movie_image));
  DEBUG_MSG("header first mov img: " << header_.first_movie_image);
  cine_file.read(reinterpret_cast<char*>(&header_.total_image_count), sizeof(header_.total_image_count));
  DEBUG_MSG("total image count:    " << header_.total_image_count);
  cine_file.read(reinterpret_cast<char*>(&header_.first_image_no), sizeof(header_.first_image_no));
  DEBUG_MSG("first image no:       " << header_.first_image_no);
  cine_file.read(reinterpret_cast<char*>(&header_.image_count), sizeof(header_.image_count));
  DEBUG_MSG("header image count:   " << header_.image_count);
  cine_file.read(reinterpret_cast<char*>(&header_.off_image_header), sizeof(header_.off_image_header));
  assert((int)header_.off_image_header==test_size);
  DEBUG_MSG("offset image header:  " << header_.off_image_header);
  cine_file.read(reinterpret_cast<char*>(&header_.off_setup), sizeof(header_.off_setup));
  DEBUG_MSG("offset setup:         " << header_.off_setup);
  cine_file.read(reinterpret_cast<char*>(&header_.off_image_offsets), sizeof(header_.off_image_offsets));
  DEBUG_MSG("offset image offsets: " << header_.off_image_offsets);
  cine_file.read(reinterpret_cast<char*>(&header_.trigger_time), sizeof(header_.trigger_time));
  image_offsets_.resize(header_.image_count);

  // BITMAP HEADER

  DEBUG_MSG("reading the cine bitmap header:");
  // seek the file posision of the image header:
  cine_file.seekg(header_.off_image_header);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.size), sizeof(bitmap_header_.size));
  DEBUG_MSG("bitmap header size:      " << bitmap_header_.size);
  int header_test_size = 0;
  header_test_size += sizeof(bitmap_header_.size);
  header_test_size += sizeof(bitmap_header_.width);
  header_test_size += sizeof(bitmap_header_.height);
  header_test_size += sizeof(bitmap_header_.planes);
  header_test_size += sizeof(bitmap_header_.bit_count);
  header_test_size += sizeof(bitmap_header_.compression);
  header_test_size += sizeof(bitmap_header_.size_image);
  header_test_size += sizeof(bitmap_header_.x_pixels_per_meter);
  header_test_size += sizeof(bitmap_header_.y_pixels_per_meter);
  header_test_size += sizeof(bitmap_header_.clr_used);
  header_test_size += sizeof(bitmap_header_.clr_important);
  assert(header_test_size==(int)bitmap_header_.size);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.width), sizeof(bitmap_header_.width));
  DEBUG_MSG("bitmap width:            " << bitmap_header_.width);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.height), sizeof(bitmap_header_.height));
  DEBUG_MSG("bitmap height:           " << bitmap_header_.height);
  if(bitmap_header_.height < 0){
    std::cout <<"** Warning: the cine file has recorded the pixel array upside down" << std::endl;
    bitmap_header_.height *= -1;
  }
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.planes), sizeof(bitmap_header_.planes));
  DEBUG_MSG("bitmap num planes:       " << bitmap_header_.planes);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.bit_count), sizeof(bitmap_header_.bit_count));
  DEBUG_MSG("bitmap bit count:        " << bitmap_header_.bit_count);
  assert(bitmap_header_.bit_count==8||bitmap_header_.bit_count==16);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.compression), sizeof(bitmap_header_.compression));
  DEBUG_MSG("bitmap compression:      " << bitmap_header_.compression);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.size_image), sizeof(bitmap_header_.size_image));
  DEBUG_MSG("bitmap image size:       " << bitmap_header_.size_image);
  assert(bitmap_header_.size_image*header_.image_count <= file_size);
  int bit_depth = (bitmap_header_.size_image * 8) / (bitmap_header_.width * bitmap_header_.height);
  DEBUG_MSG("bitmap actual bit count: " << bit_depth);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.x_pixels_per_meter), sizeof(bitmap_header_.x_pixels_per_meter));
  DEBUG_MSG("bitmap x pels/meter:     " << bitmap_header_.x_pixels_per_meter);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.y_pixels_per_meter), sizeof(bitmap_header_.y_pixels_per_meter));
  DEBUG_MSG("bitmap y pels/meter:     " << bitmap_header_.y_pixels_per_meter);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.clr_used), sizeof(bitmap_header_.clr_used));
  DEBUG_MSG("bitmap colors used:      " << bitmap_header_.clr_used);
  assert(bitmap_header_.clr_used==0);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.clr_important), sizeof(bitmap_header_.clr_important));
  DEBUG_MSG("important colors:        " << bitmap_header_.clr_important);

  if(bit_depth==8) bit_depth_=BIT_DEPTH_8;
  else if (bit_depth==16) bit_depth_=BIT_DEPTH_16;
  else if (bit_depth==10) bit_depth_=BIT_DEPTH_10_PACKED;
  else{
    std::cerr << "invalid bit depth " << bit_depth << std::endl;
    throw std::invalid_argument("invalid bit depth");
  }

  // read the image offsets:
  cine_file.seekg(header_.off_image_offsets);
  assert(image_offsets_.size()==header_.image_count);
  for (size_t i=0;i<header_.image_count;++i){
    int64_t offset;
    cine_file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
    image_offsets_[i] = offset;
  }
  // close the file:
  cine_file.close();

}



} // end namespace hypercine
