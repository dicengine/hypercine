#ifndef HYPERCINE_H
#define HYPERCINE_H
#if defined(WIN32)
#  if defined(HYPERCINE_LIB_EXPORTS_MODE)
#    define HYPERCINE_LIB_DLL_EXPORT __declspec(dllexport)
#  else
#    define HYPERCINE_LIB_DLL_EXPORT __declspec(dllimport)
#  endif
#else
#  define HYPERCINE_LIB_DLL_EXPORT
#endif
// debugging macros:
#ifdef HYPERCINE_DEBUG_MSG
#  define DEBUG_MSG(x) do { std::cout << "[DEBUG]: " << x << std::endl; } while (0)
#else
#  define DEBUG_MSG(x) do {} while (0)
#endif

# define ASSERT_EXPR(test,count){if(test){}else{std::cout << "[-- failed assertion --](" << __FILE__ <<  " line "<< __LINE__ <<"): " <<  #test << std::endl;count++;}}

#if defined(WIN32)
  #include <cstdint>
#else
  #include <stdint.h>
#endif
#include <string>
#include <vector>
//#include <ostream>
#include <iostream>

namespace hypercine {


class HYPERCINE_LIB_DLL_EXPORT
HyperCine{
public:

  /// \brief function to swap the endianness of a 16bit data type
  /// \param x the variable to endian swap
  static void endian_swap(uint16_t& x){
    x = (x>>8) | (x<<8);
  }

  /// Fractions
  typedef uint32_t FRACTIONS;
  /// Pointer to fractions
  typedef uint32_t *PFRACTIONS;

  /// Struct to hold the trigger time
  typedef struct tagTIME64
  {
    /// fractions
    FRACTIONS fractions;
    /// seconds
    uint32_t seconds;
  } TIME64;

  /// Struct to hold the cine file header information
  struct
  CineFileHeader{
    /// Type
    uint16_t type;
    /// Header size
    uint16_t header_size;
    /// Header offset
    uint64_t header_offset;
    /// Compression
    uint16_t compression;
    /// Version
    uint16_t version;
    /// First movie image
    int32_t first_movie_image;
    /// Total image count
    uint32_t total_image_count;
    /// First image index
    int32_t first_image_no;
    /// Image count
    uint32_t image_count;
    /// Offset to header
    uint32_t off_image_header;
    /// Offset to setup
    uint32_t off_setup;
    /// Offset to image offsets
    uint32_t off_image_offsets;
    /// trigger time
    TIME64 trigger_time;
  };

  /// Bit depth enumeration
  enum Bit_Depth{
    BIT_DEPTH_8=0,
    BIT_DEPTH_16,
    BIT_DEPTH_10_PACKED,
    NO_SUCH_BIT_DEPTH
  };

  /// Struct to hold the image information
  struct BitmapHeader{
    /// bitmap size
    uint32_t size;
    /// bitmap width
    int32_t width;
    /// bitmap height
    int32_t height;
    /// bitmap planes
    uint16_t planes;
    /// bitmap bit count
    uint16_t bit_count;
    /// bitmap compression
    uint32_t compression;
    /// bitmap image size
    uint32_t size_image;
    /// bitmap x pixels per meter
    int32_t x_pixels_per_meter;
    /// bitmap y pixels per meter
    int32_t y_pixels_per_meter;
    /// bitmap color used
    uint32_t clr_used;
    /// bitmap color important
    uint32_t clr_important;
    /// bit depth of the file
    Bit_Depth bit_depth;
  };

  /// struct to hold the image bounds requested during a file read
  struct HYPERCINE_LIB_DLL_EXPORT
  HyperFrame{
    // constructor
    HyperFrame():
      frame_begin(0),
      frame_count(0){};
    // reset the hyperframe parameters
    void clear(){
      frame_begin=0;
      frame_count=0;
      x_begin.clear();
      y_begin.clear();
      x_count.clear();
      y_count.clear();
    }
    // returns true if the HyperFrame is valid
    bool is_valid()const{return frame_count>0
        &&x_begin.size()==x_count.size()
        &&x_begin.size()==y_begin.size()
        &&x_begin.size()==y_count.size();}
    // returns the number of regions of interst
    size_t num_rois()const{if(is_valid()) return x_begin.size(); else return 0;}
    // return the number of pixels per frame
    size_t num_pixels_per_frame()const{
      size_t num_pixels_pf = 0;
      for(size_t i=0;i<num_rois();++i)
        num_pixels_pf += x_count[i]*y_count[i];
      return num_pixels_pf;
    }
    // returns the largest number of pixels in any one hyperframe region of interest (one frame, one subwindow of the image)
    size_t max_pixels_per_roi()const{
      size_t max_num_pixels = 0;
      for(size_t i=0;i<num_rois();++i)
        if(x_count[i]*y_count[i]>max_num_pixels)
          max_num_pixels = x_count[i]*y_count[i];
      return max_num_pixels;
    }
    size_t max_pixels_per_roi_row()const{
      size_t max_num_pixels = 0;
      for(size_t i=0;i<num_rois();++i)
        if(x_count[i]>max_num_pixels)
          max_num_pixels = x_count[i];
      return max_num_pixels;
    }
    // first frame (indexed by the global frame id's, which can be negative if pre-frames are used, not necessarily 0-based)
    int frame_begin;
    // number of frames to read
    int frame_count;
    // vector x coordinates of upper left corners for the frame windows or regions of interest
    std::vector<size_t> x_begin;
    // vector of widths of the frame windows or regions of interest
    std::vector<size_t> x_count;
    // vector of y coordinats of upper left corners for the frame windows or regions of interest
    std::vector<size_t> y_begin;
    // vector of heights of the frame windows or regions of interest
    std::vector<size_t> y_count;
  };

  // TODO if the read request is out of frame, go to file to get it and send a warning

  /// constructor
  /// \param file_name name of the file to read
  HyperCine(const char * file_name);
  /// destructor
  ~HyperCine(){};

  /// method to read the cine header
  void read_header(const char * file_name);

  /// method to read the cine file into the storage buffer
  /// \param hf the HyperFrame that defines what regions of interest to read and for what frame range
  void read_buffer(HyperFrame & hf);

  /// method to read a hyperframe into the buffer for a 10bit packed cine file
  void read_hyperframe_10_bit_packed(const HyperFrame & hf);

  /// return pointer to the cine file header
  CineFileHeader * header(){return &header_;}

  /// return pointer to the bitmap header
  BitmapHeader * bitmap_header(){return &bitmap_header_;}

  /// return a pointer to the image offsets
  std::vector<int64_t> * image_offsets(){return &image_offsets_;}

  /// return the filename
  std::string file_name()const{return file_name_;}

  /// return the bit depth
  Bit_Depth bit_depth() const{return bitmap_header_.bit_depth;}

  /// return the image width, or the width of an roi if specified
  size_t width(const int roi=-1)const;

  /// return the image height, or the height of an roi if specified
  size_t height(const int roi=-1)const;

  /// return a pointer to the raw data
  std::vector<char> * data(){return & data_;}

  /// return a pointer to the raw data for a given frame, roi
  char * data(const int & frame, const size_t roi);

  /// send hc to ostreams like cout
  friend std::ostream& operator<<(std::ostream& os, const HyperCine & hc);

  /// struct to hold information such as the indexing into each frame of the video
  CineFileHeader header_;
  /// struct to hold information about each frame itself
  BitmapHeader bitmap_header_;
  /// pointer to the array with the offsets to each image
  std::vector<int64_t> image_offsets_;
  /// the name of the cine file
  std::string file_name_;
  /// buffer for storing date read from cine file in contiguous block (all frames, all regions of interst together)
  std::vector<char> data_;
  /// storage for the index of the first pixel of an roi for each roi, one vector of indices per frame
  /// (access is (frame,roi_id) to get the index of the first pixel's value in the data_ storage)
  std::vector<std::vector<size_t> > roi_data_indices_;
  /// store the width of each region of interest
  std::vector<size_t> roi_widths_;
  /// store the height of each region of interest
  std::vector<size_t> roi_heights_;
private:


};


} // end namespace hypercine

#endif
