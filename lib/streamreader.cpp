#include "streamreader.hpp"

#include <vector>

namespace th {

  void StreamReader::resolve_stream(const std::string& name) {
    m_stream_info = lsl::resolve_stream("name", name)[0];
    m_stream = Stream(static_cast<std::size_t>(m_stream_info.channel_count()));
  }

  void StreamReader::read(std::chrono::milliseconds ms) {
    using std::chrono::steady_clock, std::chrono::duration_cast;

    m_stream.clear();

    lsl::stream_inlet inlet(m_stream_info);
    std::vector<double> samples;
    const auto start_time = steady_clock::now();

    while (
      duration_cast<std::chrono::milliseconds>(steady_clock::now() - start_time)
        .count() < ms.count()) {
      inlet.pull_sample(samples);
      for (int i = 0; i < m_stream_info.channel_count(); ++i) {
        m_stream.channel(i).add_sample(samples[i]);
      }
    }
  }

  Spectrum StreamReader::spectrum(std::size_t channel_number) {
    return m_stream.discrete_fourier_transform(channel_number);
  }

  void StreamReader::clear()
  {
    m_stream = Stream(static_cast<std::size_t>(m_stream_info.channel_count()));
  }

}  // namespace th
