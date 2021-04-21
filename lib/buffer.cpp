#include "buffer.hpp"

#include <fftw3.h>

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

  Spectrum Buffer::discrete_fourier_transform() {
    const std::size_t samples_count = m_samples.size();
    const std::size_t samples_count_half = samples_count / 2;

    fftw_complex* out =
      (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * samples_count);

    fftw_plan p =
      fftw_plan_dft_r2c_1d(samples_count, m_samples.data(), out, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);

    Spectrum result{ std::vector<double>(samples_count_half),
                     std::vector<double>(samples_count_half) };
    for (std::size_t n = 0; n < samples_count_half; ++n) {
      result.frequencies[n] = (n * SAMPLING_FREQUENCY) / samples_count;
      result.magnitudes[n] =
        std::abs(std::complex<double>(out[n][0], out[n][1]));
    }

    fftw_free(out);

    return result;
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
