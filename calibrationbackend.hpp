#ifndef CALIBRATIONBACKEND_HPP
#define CALIBRATIONBACKEND_HPP

#include <QObject>
#include <QTimer>
#include <qqml.h>

class CalibrationBackend : public QObject
{
  Q_OBJECT

  Q_PROPERTY(int arrowIndex READ arrowIndex NOTIFY arrowIndexChanged)
  Q_PROPERTY(int preparationTime MEMBER m_preparationTime NOTIFY preparationTimeChanged)
  Q_PROPERTY(int imageryTime MEMBER m_imageryTime NOTIFY imageryTimeChanged)
  Q_PROPERTY(int cooldownTime MEMBER m_cooldownTime NOTIFY cooldownTimeChanged)

  QML_ELEMENT
public:
  CalibrationBackend();
  explicit CalibrationBackend(QObject* parent);

  int arrowIndex() const;
  Q_INVOKABLE void calibrate(int runsMaxCount);

signals:
  void arrowIndexChanged(int);

  void preparationTimeChanged(int);
  void imageryTimeChanged(int);
  void cooldownTimeChanged(int);

  void calibrationStarted();
  void runStarted();
  void imageryPhaseStarted();
  void imageryPhaseFinished();
  void runFinished(int arrowIndex);
  void calibrationFinished();

private:
  enum class ArrowIndex { None, Left, Right };

private:
  void startRun();

private:
  ArrowIndex m_arrowIndex = ArrowIndex::None;

  QTimer m_preparationTimer;
  QTimer m_imageryTimer;
  QTimer m_cooldownTimer;

  int m_preparationTime = 2;
  int m_imageryTime = 7;
  int m_cooldownTime = 2;

  int m_runsMaxCount = 0;
  int m_currentRun = 0;
};

#endif // CALIBRATIONBACKEND_HPP
