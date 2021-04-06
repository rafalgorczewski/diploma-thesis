#ifndef HQR_LIB_STREAM_HPP
#define HQR_LIB_STREAM_HPP

#include <cstddef>
#include <vector>

#include "buffer.hpp"
#include "spectrum.hpp"

namespace th {

  class Stream {
   public:
    Stream();
    Stream(std::size_t channels_count);

    void resize(std::size_t channels_count);
    void clear();

    Buffer& channel(std::size_t channel_number) &;
    const Buffer& channel(std::size_t channel_number) const&;
    const Buffer& channel(std::size_t channel_number) const&& = delete;

    Spectrum discrete_fourier_transform(std::size_t channel_number);
    Spectrum short_time_fourier_transform(std::size_t channel_number) const;
    Spectrum wavelet_transform(std::size_t channel_number) const;

   private:
    std::vector<Buffer> m_channels;
  };

}  // namespace th

#endif  // HQR_LIB_STREAM_HPP
