#include "buffer.hpp"

#include <utility>
#include <complex>
#include <cmath>

namespace th {

  void Buffer::add_sample(sample_t sample) {
    m_samples.push_back(sample);
  }

  sample_t Buffer::get_sample(std::size_t index) const {
    return m_samples[index];
  }

  sample_t Buffer::operator[](std::size_t index) const {
    return get_sample(index);
  }

  Buffer::buffer_t &Buffer::get_buffer() &{
    return m_samples;
  }

  const Buffer::buffer_t &Buffer::get_buffer() const &{
    return m_samples;
  }

  Spectrum Buffer::discrete_fourier_transform() const {
    using namespace std::complex_literals;

    const std::size_t samples_count = m_samples.size() - 1;
    const auto spectral_resolution =
            SAMPLING_FREQUENCY / static_cast<const double>(samples_count);

    std::vector<double> x(samples_count), y(samples_count);

    for (std::size_t i = 0; i < samples_count; ++i) {
      const auto harmonic_number = static_cast<double>(i);
      const auto frequency = static_cast<double>(i) * spectral_resolution;
      std::complex<double> magnitude{};

      for (std::size_t n = 0; n < samples_count; ++n) {
        const double sample_value = m_samples[n];
        const double cosine =
                std::cos(
                        -2.0 * M_PI * static_cast<double>(n) * harmonic_number /
                        static_cast<const double>(samples_count));
        const std::complex<double> sine =
                1i * std::sin(-2.0 * M_PI * static_cast<double>(n) *
                              harmonic_number /
                              static_cast<const double>(samples_count));

        magnitude += sample_value * (cosine + sine);
      }

      const double module = std::abs(magnitude);

      x[i] = frequency;
      y[i] = module;
    }

    return {std::move(x), std::move(y)};
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

}
