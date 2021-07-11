#ifndef CALIBRATIONBACKEND_HPP
#define CALIBRATIONBACKEND_HPP

#include <QObject>
#include <QTimer>
#include <qqml.h>

#include "bodypart.hpp"

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
  void preparationPhaseStarted();
  void preparationPhaseFinished();
  void imageryPhaseStarted();
  void imageryPhaseFinished();
  void cooldownPhaseStarted();
  void cooldownPhaseFinished();
  void runFinished();
  void calibrationFinished();

private:
  void startRun();

private:
  th::BodyPart m_bodyPart = th::BodyPart::None;

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
