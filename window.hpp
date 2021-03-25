#ifndef THESIS_WINDOW_HPP
#define THESIS_WINDOW_HPP

#include <QMainWindow>
#include <QtCharts>

namespace th {

  class Window : public QMainWindow {
    Q_OBJECT

   public:
    Window();

   private:
    QtCharts::QChart m_spectrum;
    QtCharts::QChartView m_chartview{&m_spectrum};
  };

}

#endif //THESIS_WINDOW_HPP
