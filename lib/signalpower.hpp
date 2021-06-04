#ifndef HQR_LIB_SIGNALPOWER_HPP
#define HQR_LIB_SIGNALPOWER_HPP

#include <unordered_map>
#include <vector>

namespace th {
  class SignalPower
  {
  public:
    std::vector<double>& operator[](int channel) &;

  private:
    std::unordered_map<int, std::vector<double>> m_channel_powers;
  };
}
#endif // HQR_LIB_SIGNALPOWER_HPP
