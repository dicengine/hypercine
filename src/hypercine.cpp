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
  cine_file.read(reinterpret_cast<char*>(&header_.Type), sizeof(header_.Type));
  DEBUG_MSG("file size:            " << file_size);
  DEBUG_MSG("header type:          " << header_.Type);
  cine_file.read(reinterpret_cast<char*>(&header_.Headersize), sizeof(header_.Headersize));
  DEBUG_MSG("header size:          " << header_.Headersize);
  int test_size = 0;
  test_size += sizeof(header_.Type);
  test_size += sizeof(header_.Headersize);
  test_size += sizeof(header_.Compression);
  test_size += sizeof(header_.Version);
  test_size += sizeof(header_.FirstMovieImage);
  test_size += sizeof(header_.TotalImageCount);
  test_size += sizeof(header_.FirstImageNo);
  test_size += sizeof(header_.ImageCount);
  test_size += sizeof(header_.OffImageHeader);
  test_size += sizeof(header_.OffSetup);
  test_size += sizeof(header_.OffImageOffsets);
  test_size += sizeof(header_.TriggerTime);
  DEBUG_MSG("test size:            " << test_size);
  assert(test_size==header_.Headersize);
  cine_file.read(reinterpret_cast<char*>(&header_.Compression), sizeof(header_.Compression));
  DEBUG_MSG("header compression:   " << header_.Compression);
  assert(header_.Compression==0);
  cine_file.read(reinterpret_cast<char*>(&header_.Version), sizeof(header_.Version));
  DEBUG_MSG("header version:       " << header_.Version);
  assert(header_.Version==1);
  cine_file.read(reinterpret_cast<char*>(&header_.FirstMovieImage), sizeof(header_.FirstMovieImage));
  DEBUG_MSG("header first mov img: " << header_.FirstMovieImage);
  cine_file.read(reinterpret_cast<char*>(&header_.TotalImageCount), sizeof(header_.TotalImageCount));
  DEBUG_MSG("total image count:    " << header_.TotalImageCount);
  cine_file.read(reinterpret_cast<char*>(&header_.FirstImageNo), sizeof(header_.FirstImageNo));
  DEBUG_MSG("first image no:       " << header_.FirstImageNo);
  cine_file.read(reinterpret_cast<char*>(&header_.ImageCount), sizeof(header_.ImageCount));
  DEBUG_MSG("header image count:   " << header_.ImageCount);
  cine_file.read(reinterpret_cast<char*>(&header_.OffImageHeader), sizeof(header_.OffImageHeader));
  assert((int)header_.OffImageHeader==test_size);
  DEBUG_MSG("offset image header:  " << header_.OffImageHeader);
  cine_file.read(reinterpret_cast<char*>(&header_.OffSetup), sizeof(header_.OffSetup));
  DEBUG_MSG("offset setup:         " << header_.OffSetup);
  cine_file.read(reinterpret_cast<char*>(&header_.OffImageOffsets), sizeof(header_.OffImageOffsets));
  DEBUG_MSG("offset image offsets: " << header_.OffImageOffsets);
  cine_file.read(reinterpret_cast<char*>(&header_.TriggerTime), sizeof(header_.TriggerTime));
  image_offsets_.resize(header_.ImageCount);

  // BITMAP HEADER

  DEBUG_MSG("reading the cine bitmap header:");
  // seek the file posision of the image header:
  cine_file.seekg(header_.OffImageHeader);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biSize), sizeof(bitmap_header_.biSize));
  DEBUG_MSG("bitmap header size:      " << bitmap_header_.biSize);
  int header_test_size = 0;
  header_test_size += sizeof(bitmap_header_.biSize);
  header_test_size += sizeof(bitmap_header_.biWidth);
  header_test_size += sizeof(bitmap_header_.biHeight);
  header_test_size += sizeof(bitmap_header_.biPlanes);
  header_test_size += sizeof(bitmap_header_.biBitCount);
  header_test_size += sizeof(bitmap_header_.biCompression);
  header_test_size += sizeof(bitmap_header_.biSizeImage);
  header_test_size += sizeof(bitmap_header_.biXPelsPerMeter);
  header_test_size += sizeof(bitmap_header_.biYPelsPerMeter);
  header_test_size += sizeof(bitmap_header_.biClrUsed);
  header_test_size += sizeof(bitmap_header_.biClrImportant);
  assert(header_test_size==(int)bitmap_header_.biSize);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biWidth), sizeof(bitmap_header_.biWidth));
  DEBUG_MSG("bitmap width:            " << bitmap_header_.biWidth);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biHeight), sizeof(bitmap_header_.biHeight));
  DEBUG_MSG("bitmap height:           " << bitmap_header_.biHeight);
  if(bitmap_header_.biHeight < 0){
    std::cout <<"** Warning: the cine file has recorded the pixel array upside down" << std::endl;
    bitmap_header_.biHeight *= -1;
  }
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biPlanes), sizeof(bitmap_header_.biPlanes));
  DEBUG_MSG("bitmap num planes:       " << bitmap_header_.biPlanes);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biBitCount), sizeof(bitmap_header_.biBitCount));
  DEBUG_MSG("bitmap bit count:        " << bitmap_header_.biBitCount);
  assert(bitmap_header_.biBitCount==8||bitmap_header_.biBitCount==16);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biCompression), sizeof(bitmap_header_.biCompression));
  DEBUG_MSG("bitmap compression:      " << bitmap_header_.biCompression);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biSizeImage), sizeof(bitmap_header_.biSizeImage));
  DEBUG_MSG("bitmap image size:       " << bitmap_header_.biSizeImage);
  assert(bitmap_header_.biSizeImage*header_.ImageCount <= file_size);
  int bit_depth = (bitmap_header_.biSizeImage * 8) / (bitmap_header_.biWidth * bitmap_header_.biHeight);
  DEBUG_MSG("bitmap actual bit count: " << bit_depth);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biXPelsPerMeter), sizeof(bitmap_header_.biXPelsPerMeter));
  DEBUG_MSG("bitmap x pels/meter:     " << bitmap_header_.biXPelsPerMeter);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biYPelsPerMeter), sizeof(bitmap_header_.biYPelsPerMeter));
  DEBUG_MSG("bitmap y pels/meter:     " << bitmap_header_.biYPelsPerMeter);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biClrUsed), sizeof(bitmap_header_.biClrUsed));
  DEBUG_MSG("bitmap colors used:      " << bitmap_header_.biClrUsed);
  assert(bitmap_header_.biClrUsed==0);
  cine_file.read(reinterpret_cast<char*>(&bitmap_header_.biClrImportant), sizeof(bitmap_header_.biClrImportant));
  DEBUG_MSG("important colors:        " << bitmap_header_.biClrImportant);

  if(bit_depth==8) bit_depth_=BIT_DEPTH_8;
  else if (bit_depth==16) bit_depth_=BIT_DEPTH_16;
  else if (bit_depth==10) bit_depth_=BIT_DEPTH_10_PACKED;
  else{
    std::cerr << "invalid bit depth " << bit_depth << std::endl;
    throw std::invalid_argument("invalid bit depth");
  }

  // read the image offsets:
  cine_file.seekg(header_.OffImageOffsets);
  assert(image_offsets_.size()==header_.ImageCount);
  for (size_t i=0;i<header_.ImageCount;++i){
    int64_t offset;
    cine_file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
    image_offsets_[i] = offset;
  }
  // close the file:
  cine_file.close();

}



} // end namespace hypercine
