#include "stream.hpp"

namespace th {

  Stream::Stream() : Stream(2) {
  }

  Stream::Stream(std::size_t channels_count) {
    m_channels.resize(channels_count);
  }

  void Stream::resize(std::size_t channels_count) {
    m_channels.resize(channels_count);
  }

  void Stream::clear() {
    for (auto& channel : m_channels) {
      channel.data().clear();
    }
  }

  Buffer& Stream::channel(std::size_t channel_number) & {
    return m_channels[channel_number];
  }

  const Buffer& Stream::channel(std::size_t channel_number) const& {
    return m_channels[channel_number];
  }

  Spectrum Stream::discrete_fourier_transform(std::size_t channel_number, double sampling_frequency) {
    return m_channels[channel_number].discrete_fourier_transform(sampling_frequency);
  }

}  // namespace th
