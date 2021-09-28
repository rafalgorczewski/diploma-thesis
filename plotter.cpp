#include "plotter.h"

#include <algorithm>

#include <QBarSet>

Plotter::Plotter(QObject *parent) : QObject(parent)
{

}

void Plotter::plot(QBarSeries* series, QVector<double> powers, QValueAxis* yAxis)
{
  series->clear();

  const auto it = std::max_element(powers.cbegin(), powers.cend());
  if (it != powers.cend()) {
    yAxis->setMax(*it);
  }

  QBarSet* set = new QBarSet("Power");
  set->setColor(QColor("#F48FB1"));
  for (auto power : powers) {
    set->append(power);
  }
  series->append(set);
}
