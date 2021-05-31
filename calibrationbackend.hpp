#ifndef CALIBRATIONBACKEND_HPP
#define CALIBRATIONBACKEND_HPP

#include <QObject>
#include <qqml.h>

class CalibrationBackend : public QObject
{
  Q_OBJECT

  Q_PROPERTY(int arrowIndex READ arrowIndex NOTIFY arrowIndexChanged)

  QML_ELEMENT
public:
  explicit CalibrationBackend(QObject *parent = nullptr);

  int arrowIndex() const;

signals:
  void arrowIndexChanged(int);

private:
  enum class ArrowIndex { None, Left, Right };

private:
  ArrowIndex m_arrowIndex = ArrowIndex::None;
};

#endif // CALIBRATIONBACKEND_HPP
