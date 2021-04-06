#ifndef HQR_LIB_BUFFER_HPP
#define HQR_LIB_BUFFER_HPP

#include <chrono>
#include <cstddef>
#include <vector>

#include "sample.hpp"
#include "spectrum.hpp"

namespace th {

  class Buffer {
   public:
    using buffer_t = std::vector<sample_t>;

   public:
    void add_sample(sample_t sample);
    sample_t sample(std::size_t index) const;
    sample_t operator[](std::size_t index) const;

    buffer_t& data() &;
    const buffer_t& data() const&;
    const buffer_t& data() const&& = delete;

    Spectrum discrete_fourier_transform();
    Spectrum short_time_fourier_transform() const;
    Spectrum wavelet_transform() const;

    buffer_t::iterator begin();
    buffer_t::iterator end();
    buffer_t::const_iterator cbegin() const;
    buffer_t::const_iterator cend() const;

   private:
    buffer_t m_samples;
  };

}  // namespace th

#endif // HQR_LIB_BUFFER_HPP
