#include "plotter.h"

#include <algorithm>

#include <QBarSet>

Plotter::Plotter(QObject *parent) : QObject(parent)
{

}

void Plotter::plotPowers(QBarSeries* series, QVector<double> powers, QValueAxis* yAxis)
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

void Plotter::plotClassificationPoints(QScatterSeries* inputSeries,
                                        QVector<double> projectedInput,
                                        QValueAxis* xAxis,
                                        QValueAxis* yAxis)
{
  const double minX = xAxis->min();
  const double minY = yAxis->min();
  const double maxX = xAxis->max();
  const double maxY = yAxis->max();

  const double inputX = projectedInput[0];
  const double inputY = projectedInput[1];

  const double plottedX = std::clamp(inputX, minX, maxX);
  const double plottedY = std::clamp(inputY, minY, maxY);

  inputSeries->clear();
  inputSeries->append(plottedX, plottedY);
}

void Plotter::plotClassificationPoints(QScatterSeries* leftSeries,
                                       QScatterSeries* rightSeries,
                                       QScatterSeries* noneSeries,
                                       QVector<QVector<double>> projectedData,
                                       QVector<int> labels,
                                       QValueAxis* xAxis,
                                       QValueAxis* yAxis)
{
  double minX{};
  double minY{};
  double maxX{};
  double maxY{};
  for (const auto& row : projectedData) {
    if (row[0] < minX) {
      minX = row[0];
    }
    if (row[1] < minY) {
      minY = row[1];
    }

    if (row[0] > maxX) {
      maxX = row[0];
    }
    if (row[1] > maxY) {
      maxY = row[1];
    }
  }
  xAxis->setMin(minX);
  xAxis->setMax(maxX);
  yAxis->setMin(minY);
  yAxis->setMax(maxY);

  leftSeries->clear();
  rightSeries->clear();
  noneSeries->clear();

  for (int i = 0; i < projectedData.size(); ++i) {
    QScatterSeries* series{};
    switch (labels[i]) {
      case 0: {
        series = noneSeries;
        break;
      }
      case 1: {
        series = leftSeries;
        break;
      }
      case 2: {
        series = rightSeries;
        break;
      }
    }
    series->append(projectedData[i][0], projectedData[i][1]);
  }
}
