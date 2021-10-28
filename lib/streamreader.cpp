#include "streamreader.hpp"

#include <vector>

namespace th {

  void StreamReader::resolve_stream(const std::string& name) {
    m_stream_info = lsl::resolve_stream("name", name)[0];
    m_inlet = {m_stream_info};
    m_stream = Stream(static_cast<std::size_t>(m_stream_info.channel_count()));
  }

  void StreamReader::read(std::chrono::milliseconds ms) {
    using std::chrono::steady_clock, std::chrono::duration_cast;

    clear();

    std::vector<double> samples;

    const auto start_time = steady_clock::now();
    while (steady_clock::now() - start_time < ms) {
      m_inlet.pull_sample(samples);
      for (int i = 0; i < m_stream_info.channel_count(); ++i) {
        m_stream.channel(i).add_sample(samples[i]);
      }
    }
  }

  void StreamReader::flush()
  {
    m_inlet.flush();
  }

  Spectrum StreamReader::spectrum(std::size_t channel_number) {
    return m_stream.discrete_fourier_transform(channel_number, m_stream_info.nominal_srate());
  }

  void StreamReader::clear()
  {
    m_stream.clear();
  }

}  // namespace th
