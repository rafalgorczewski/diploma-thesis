#ifndef HQR_WINDOW_HPP
#define HQR_WINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QtCharts>

#include "streamreader.hpp"

namespace th {

  class Window : public QMainWindow {
    Q_OBJECT

   public:
    Window();

   private:
    void resolve_stream();
    void plot_spectrum();

   private:
    static constexpr int SAMPLING_DURATION = 250;
    static constexpr int SPECTRUM_REDUCTION_FACTOR = 5;

   private:
    StreamReader m_stream_reader;

    QtCharts::QChart* m_spectrum_chart = new QtCharts::QChart;
    QtCharts::QChartView* m_chartview =
      new QtCharts::QChartView(m_spectrum_chart);
    QTimer* plot_timer = new QTimer(this);

    QPushButton* m_resolve_button = new QPushButton;
    QPushButton* m_plot_button = new QPushButton;
  };

}  // namespace th

#endif  // HQR_WINDOW_HPP
