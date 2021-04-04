#ifndef HQR_LIB_STREAMREADER_HPP
#define HQR_LIB_STREAMREADER_HPP

#include <lsl_cpp.h>

#include <chrono>
#include <cstddef>
#include <string>

#include "stream.hpp"

namespace th {

  class StreamReader {
   public:
    void resolve_stream(const std::string& name);
    void read(std::chrono::milliseconds ms);
    Spectrum spectrum(std::size_t channel_number, TransformType transform_type);

   private:
    lsl::stream_info m_stream_info;
    Stream m_stream;
  };

}  // namespace th

#endif  // HQR_LIB_STREAMREADER_HPP
