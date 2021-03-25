#ifndef THESIS_BUFFER_HPP
#define THESIS_BUFFER_HPP

#include "sample.hpp"
#include "spectrum.hpp"

#include <deque>
#include <cstddef>
#include <chrono>

namespace th {

  class Buffer {
   public:
    using buffer_t = std::deque<sample_t>;

   public:
    void add_sample(sample_t sample);
    sample_t get_sample(std::size_t index) const;
    sample_t operator[](std::size_t index) const;

    buffer_t &get_buffer() &;
    const buffer_t &get_buffer() const &;
    const buffer_t &get_buffer() const && = delete;

    Spectrum discrete_fourier_transform() const;
    Spectrum short_time_fourier_transform() const;
    Spectrum wavelet_transform() const;

    buffer_t::iterator begin();
    buffer_t::iterator end();
    buffer_t::const_iterator cbegin() const;
    buffer_t::const_iterator cend() const;

   private:
    buffer_t m_samples;
  };

}

#endif //THESIS_BUFFER_HPP
