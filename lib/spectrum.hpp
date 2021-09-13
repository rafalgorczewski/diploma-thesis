#ifndef HQR_LIB_SPECTRUM_HPP
#define HQR_LIB_SPECTRUM_HPP

#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>
#include <cmath>

namespace th {

  struct Spectrum {
    std::vector<double> frequencies;
    std::vector<double> magnitudes;

    double band_power(double start_freq, double end_freq) const {
      const auto lower =
        std::lower_bound(frequencies.cbegin(), frequencies.cend(), start_freq);
      const auto upper =
        std::upper_bound(frequencies.cbegin(), frequencies.cend(), end_freq);
      if (lower == frequencies.cend() || upper == frequencies.cend()) {
        return {};
      }

      const auto lower_index = std::distance(frequencies.cbegin(), lower);
      const auto upper_index = std::distance(frequencies.cbegin(), upper);
      const auto lower_magnitude = magnitudes.cbegin() + lower_index;
      const auto upper_magnitude = magnitudes.cbegin() + upper_index;

      const auto sum = std::accumulate(lower_magnitude, upper_magnitude, 0.0, [](double lhs, double rhs) {
        return lhs + (rhs * rhs);
      });
      const auto num = upper_index - lower_index;

      const auto mean = sum / (num > 0 ? num : 1);
      return std::sqrt(mean);
    }
  };

}  // namespace th

#endif  // HQR_LIB_SPECTRUM_HPP
