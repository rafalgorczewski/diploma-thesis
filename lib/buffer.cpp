#include "buffer.hpp"

#include <rfftw.h>

#include <cmath>
#include <complex>
#include <utility>

namespace th {

  void Buffer::add_sample(sample_t sample) {
    m_samples.push_back(sample);
  }

  sample_t Buffer::sample(std::size_t index) const {
    return m_samples[index];
  }

  sample_t Buffer::operator[](std::size_t index) const {
    return m_samples[index];
  }

  Buffer::buffer_t& Buffer::data() & {
    return m_samples;
  }

  const Buffer::buffer_t& Buffer::data() const& {
    return m_samples;
  }

  Spectrum Buffer::discrete_fourier_transform() const {
    return {};
  }

  Spectrum Buffer::short_time_fourier_transform() const {
    return {};
  }

  Spectrum Buffer::wavelet_transform() const {
    return {};
  }

  Buffer::buffer_t::iterator Buffer::begin() {
    return m_samples.begin();
  }

  Buffer::buffer_t::iterator Buffer::end() {
    return m_samples.end();
  }

  Buffer::buffer_t::const_iterator Buffer::cbegin() const {
    return m_samples.cbegin();
  }

  Buffer::buffer_t::const_iterator Buffer::cend() const {
    return m_samples.cend();
  }

}  // namespace th
