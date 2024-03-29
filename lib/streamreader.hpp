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
    void flush();

    Spectrum spectrum(std::size_t channel_number);

    void clear();

   private:
    lsl::stream_info m_stream_info;
    lsl::stream_inlet m_inlet{{}};
    Stream m_stream;
  };

}  // namespace th

#endif  // HQR_LIB_STREAMREADER_HPP
