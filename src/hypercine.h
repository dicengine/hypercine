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


namespace hypercine {

class HYPERCINE_LIB_DLL_EXPORT
HyperCine{
public:
  /// constructor
  HyperCine(){}; 
  /// destructor
  ~HyperCine(){}; 
};

} // end namespace hypercine

#endif
