#ifndef HQR_WINDOW_HPP
#define HQR_WINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
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
    StreamReader m_stream_reader;

    QtCharts::QChart* m_spectrum_chart = new QtCharts::QChart;
    QtCharts::QChartView* m_chartview =
      new QtCharts::QChartView(m_spectrum_chart);

    QPushButton* m_resolve_button = new QPushButton;
    QPushButton* m_plot_button = new QPushButton;
  };

}  // namespace th

#endif // HQR_WINDOW_HPP
