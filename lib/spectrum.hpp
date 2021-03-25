#ifndef THESIS_SPECTRUM_HPP
#define THESIS_SPECTRUM_HPP

#include <vector>

namespace th {

  struct Spectrum {
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
  };

}

#endif //THESIS_SPECTRUM_HPP
