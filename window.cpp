#include "window.hpp"

#include <lsl_cpp.h>

#include <QDebug>
#include <QLineSeries>
#include <chrono>
#include <vector>

#include "streamreader.hpp"

using namespace QtCharts;
using namespace lsl;
using namespace std::chrono_literals;

namespace th {
  Window::Window() : QMainWindow(nullptr) {
    resize(800, 600);
    QWidget* central_widget = new QWidget;
    setCentralWidget(central_widget);

    m_resolve_button->setText("Resolve stream");
    m_resolve_button->resize(100, 50);
    m_plot_button->setText("Record");
    m_plot_button->resize(100, 50);

    m_spectrum_chart->legend()->hide();

    QHBoxLayout* layout = new QHBoxLayout(central_widget);
    QVBoxLayout* left_pane = new QVBoxLayout;
    layout->addLayout(left_pane);
    left_pane->addWidget(m_resolve_button);
    left_pane->addWidget(m_plot_button);
    left_pane->addWidget(m_power_label);
    layout->addWidget(m_chartview);

    connect(
      m_resolve_button, &QPushButton::clicked, this, &Window::resolve_stream);
    connect(m_plot_button, &QPushButton::clicked, [this] {
      m_plot_button->setEnabled(false);
      plot_timer->start(SAMPLING_DURATION);
    });
    connect(plot_timer, &QTimer::timeout, this, &Window::plot_spectrum);
  }

  void Window::resolve_stream() {
    m_stream_reader.resolve_stream("MyAudioStream");
    m_resolve_button->setEnabled(false);
  }

  void Window::plot_spectrum() {
    QLineSeries* series = new QLineSeries();

    m_stream_reader.read(std::chrono::milliseconds(SAMPLING_DURATION));
    const Spectrum spectrum = m_stream_reader.spectrum(0);
    m_power_label->setText(QString::number(spectrum.band_power(8, 12)));

    for (std::size_t i = 0;
         i < spectrum.frequencies.size() / SPECTRUM_REDUCTION_FACTOR;
         ++i) {
      series->append(spectrum.frequencies[i], spectrum.magnitudes[i]);
    }
    m_spectrum_chart->removeAllSeries();
    m_spectrum_chart->addSeries(series);
    m_spectrum_chart->createDefaultAxes();
  }
}  // namespace th
