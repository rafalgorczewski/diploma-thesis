#ifndef HQR_LIB_SPECTRUM_HPP
#define HQR_LIB_SPECTRUM_HPP

#include <vector>

namespace th {

  enum class TransformType { Dft, Stft, Wt };

  struct Spectrum {
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
  };

}  // namespace th

#endif // HQR_LIB_SPECTRUM_HPP
