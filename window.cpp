#include "window.hpp"

#include <vector>
#include <lsl_cpp.h>
#include <iostream>

#include "buffer.hpp"

using namespace QtCharts;
using namespace lsl;

namespace th {
  Window::Window() : QMainWindow(nullptr) {
    resize(800, 600);

    QLineSeries *series = new QLineSeries();

    std::vector<stream_info> results =
            resolve_stream("name", "MyAudioStream");
    stream_inlet inlet(results[0]);

    th::Buffer buffer;
    std::vector<double> samples;

    for (int i = 0; i < 1000; ++i) {
      const int time = inlet.pull_sample(samples);
      if (samples.size()) {
        buffer.add_sample(samples[0]);
      }
    }
    for (const auto sample : buffer.discrete_fourier_transform().magnitudes) {
      std::cout << sample << std::endl;
    }
  }
}
