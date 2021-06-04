#include "signalpower.hpp"

namespace th {

std::vector<double>& SignalPower::operator[](int channel) &
{
  return m_channel_powers[channel];
}

}
