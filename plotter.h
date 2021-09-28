#ifndef PLOTTER_H
#define PLOTTER_H

#include <QObject>
#include <QString>
#include <qqml.h>
#include <QVector>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>

class Plotter : public QObject
{
  Q_OBJECT
  QML_ELEMENT
public:
  explicit Plotter(QObject *parent = nullptr);

Q_INVOKABLE void plot(QBarSeries* series, QVector<double> powers, QValueAxis* yAxis);

};

#endif // PLOTTER_H
