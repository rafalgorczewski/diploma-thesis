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
    m_spectrum_chart->createDefaultAxes();

    QHBoxLayout* layout = new QHBoxLayout(central_widget);
    layout->addWidget(m_resolve_button);
    layout->addWidget(m_plot_button);
    layout->addWidget(m_chartview);

    connect(
      m_resolve_button, &QPushButton::clicked, this, &Window::resolve_stream);
    connect(m_plot_button, &QPushButton::clicked, this, &Window::plot_spectrum);
  }

  void Window::resolve_stream() {
    m_stream_reader.resolve_stream("MyAudioStream");
    qDebug() << "resolved";
  }

  void Window::plot_spectrum() {
    QLineSeries* series = new QLineSeries();

    qDebug() << "read";
    m_stream_reader.read(100ms);
    qDebug() << "spectrum";
    const Spectrum spectrum = m_stream_reader.spectrum(0, TransformType::Dft);

    qDebug() << "plot";
    for (int i = 0; i < spectrum.frequencies.size(); ++i) {
      series->append(spectrum.frequencies[i], spectrum.magnitudes[i]);
    }

    m_spectrum_chart->addSeries(series);
  }
}  // namespace th
