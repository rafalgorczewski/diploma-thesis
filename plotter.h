#ifndef PLOTTER_H
#define PLOTTER_H

#include <QObject>
#include <QString>
#include <qqml.h>
#include <QVector>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QScatterSeries>

class Plotter : public QObject
{
  Q_OBJECT
  QML_ELEMENT
public:
  explicit Plotter(QObject *parent = nullptr);

Q_INVOKABLE void plotPowers(QBarSeries* series, QVector<double> powers, QValueAxis* yAxis);
Q_INVOKABLE void plotClassificationPoints(QScatterSeries* inputSeries, QVector<double> projectedInput);
Q_INVOKABLE void plotClassificationPoints(QScatterSeries* leftSeries,
                                          QScatterSeries* rightSeries,
                                          QScatterSeries* noneSeries,
                                          QVector<QVector<double>> projectedData,
                                          QVector<int> labels,
                                          QValueAxis* xAxis,
                                          QValueAxis* yAxis);
}; 

#endif // PLOTTER_H
