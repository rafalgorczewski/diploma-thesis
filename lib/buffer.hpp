#ifndef HQR_LIB_BUFFER_HPP
#define HQR_LIB_BUFFER_HPP

#include <chrono>
#include <cstddef>
#include <vector>

#include "spectrum.hpp"

namespace th {

  class Buffer {
   public:
    using buffer_t = std::vector<double>;

   public:
    void add_sample(double sample);
    double sample(std::size_t index) const;
    double operator[](std::size_t index) const;

    buffer_t& data() &;
    const buffer_t& data() const&;
    const buffer_t& data() const&& = delete;

    Spectrum discrete_fourier_transform(double sampling_frequency);

    buffer_t::iterator begin();
    buffer_t::iterator end();
    buffer_t::const_iterator cbegin() const;
    buffer_t::const_iterator cend() const;

   private:
    buffer_t m_samples;
  };

}  // namespace th

#endif  // HQR_LIB_BUFFER_HPP
