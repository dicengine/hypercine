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
#include <set>
#include <map>
#include <vector>
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
    HyperFrame(){};
    // copy constructor
    HyperFrame(const HyperFrame & hf){
      frame_ids = hf.get_frame_ids();
      for(size_t i=0;i<hf.num_windows();++i){
        x_begin.push_back(hf.window_x_begin(i));
        y_begin.push_back(hf.window_y_begin(i));
        x_count.push_back(hf.window_width(i));
        y_count.push_back(hf.window_height(i));
      }
    }
    // reset the hyperframe parameters
    void clear(){
      frame_ids.clear();
      x_begin.clear();
      y_begin.clear();
      x_count.clear();
      y_count.clear();
    }
    // add a range of frames to the hyperframe
    void add_frames(const int frame_begin, const int count){
      for(int i=frame_begin;i<frame_begin+count;++i)
        frame_ids.insert(i);
    }
    // add a single frame to the hyperframe
    void add_frame(const int frame_id){
        frame_ids.insert(frame_id);
    }
    // return a deep copy of the frame id set
    std::set<int> get_frame_ids()const{
      return frame_ids;
    }
    // return a pointer to the frame ids
    std::set<int> const * frame_ids_view()const{
      return & frame_ids;
    }
    // return the number of frames
    size_t num_frames()const{
      return frame_ids.size();
    }
    // add a region of interest to the hyperframe
    void add_window(const size_t x_b, const size_t x_c, const size_t y_b, const size_t y_c){
      x_begin.push_back(x_b);
      y_begin.push_back(y_b);
      x_count.push_back(x_c);
      y_count.push_back(y_c);
    }
    // returns the number of regions of interst
    size_t num_windows()const{return x_begin.size();}
    // return the total number of pixels per frame (including all windows in the frame)
    size_t num_pixels_per_frame()const{
      size_t num_pixels_pf = 0;
      for(size_t i=0;i<num_windows();++i)
        num_pixels_pf += x_count[i]*y_count[i];
      return num_pixels_pf;
    }
    // return the number of pixels in the selected window
    size_t num_pixels_per_window(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return x_count[window_id]*y_count[window_id];
    }
    // return the width of the selected window
    size_t window_width(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return x_count[window_id];
    }
    // return the height of the selected window
    size_t window_height(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return y_count[window_id];
    }
    // return the x begin for the selected window
    size_t window_x_begin(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return x_begin[window_id];
    }
    // return the x begin for the selected window
    size_t window_y_begin(const size_t window_id)const{
      if(window_id>=num_windows()) return 0;
      return y_begin[window_id];
    }
    // returns the size of a buffer large enough to store an entire row of pixel values for the widest window
    size_t buffer_row_size()const{
      size_t max_num_pixels = 0;
      for(size_t i=0;i<num_windows();++i)
        if(window_width(i)>max_num_pixels)
          max_num_pixels = window_width(i);
      return max_num_pixels;
    }
  private:
    // storage of frame ids
    std::set<int> frame_ids;
    // vector x coordinates of upper left corners for the frame windows or regions of interest
    std::vector<size_t> x_begin;
    // vector of widths of the frame windows or regions of interest
    std::vector<size_t> x_count;
    // vector of y coordinats of upper left corners for the frame windows or regions of interest
    std::vector<size_t> y_begin;
    // vector of heights of the frame windows or regions of interest
    std::vector<size_t> y_count;
  };

  /// constructor
  /// \param file_name name of the file to read
  HyperCine(const char * file_name);
  /// destructor
  ~HyperCine(){};

  /// method to read the cine file into the storage buffer
  /// \param hf the HyperFrame that defines what regions of interest to read and for what frame range
  void read_buffer(HyperFrame & hf);

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

  /// return the image width, or the width of a window if specified
  int width(const int window_id=-1)const{
     if(window_id==-1) return bitmap_header_.width;
     else return hf_.window_width(window_id);
  };

  /// return the image height, or the height of a window if specified
  int height(const int window_id=-1)const{
    if(window_id==-1) return bitmap_header_.height;
    else return hf_.window_height(window_id);
  };

  /// return a pointer to the raw data
  std::vector<char> * data(){return & data_;}

  /// returns true if the frame and window id are valid
  bool valid_frame_window(const int frame, const size_t window_id);

  /// return a pointer to the raw data for a given frame, and window
  char * data(const int frame, const size_t window_id);

  /// send hc to ostreams like cout
  friend std::ostream& operator<<(std::ostream& os, const HyperCine & hc);

private:
  /// method to read the cine header
  void read_header(const char * file_name);

  /// method to read a hyperframe into the buffer for a 10bit packed cine file
  void read_hyperframe_10_bit_packed();

  /// struct to hold information such as the indexing into each frame of the video
  CineFileHeader header_;
  /// struct to hold information about each frame itself
  BitmapHeader bitmap_header_;
  /// pointer to the array with the offsets to each image
  std::vector<int64_t> image_offsets_;
  /// the name of the cine file
  std::string file_name_;
  /// buffer for storing date read from cine file in contiguous block (all frames, all windows together)
  std::vector<char> data_;
  /// storage for the index of the first pixel of a frame window for each frame, one vector of indices per frame
  /// (access is [frame][window] to get the index of the first pixel's value in the data_ storage)
  std::map<int,std::vector<size_t> > data_indices_;
  /// store the HyperFrame that defines the windows and frames
  HyperFrame hf_;
};


} // end namespace hypercine

#endif
