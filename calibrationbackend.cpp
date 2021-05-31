#include "calibrationbackend.hpp"

#include "QRandomGenerator"

CalibrationBackend::CalibrationBackend() : CalibrationBackend(nullptr)
{

}

CalibrationBackend::CalibrationBackend(QObject* parent) : QObject(parent) {
  m_preparationTimer.setSingleShot(true);
  m_imageryTimer.setSingleShot(true);
  m_cooldownTimer.setSingleShot(true);

  m_preparationTimer.setInterval(m_preparationTime * 1000);
  m_imageryTimer.setInterval(m_imageryTime * 1000);
  m_cooldownTimer.setInterval(m_cooldownTime * 1000);

  connect(&m_preparationTimer, &QTimer::timeout, [this]{
    emit imageryPhaseStarted();
    m_imageryTimer.start();
  });
  connect(&m_imageryTimer, &QTimer::timeout, [this]{
    emit imageryPhaseFinished();
    m_cooldownTimer.start();
  });
  connect(&m_cooldownTimer, &QTimer::timeout, [this]{
    ++m_currentRun;
    emit runFinished(static_cast<int>(m_arrowIndex));

    if (m_currentRun < m_runsMaxCount) {
      startRun();
    } else {
      emit calibrationFinished();
    }
  });
}

int CalibrationBackend::arrowIndex() const {
  return static_cast<int>(m_arrowIndex);
}

void CalibrationBackend::calibrate(int runsMaxCount) {
  m_runsMaxCount = runsMaxCount;
  startRun();
}

void CalibrationBackend::startRun() {
  emit calibrationStarted();
  m_arrowIndex = ArrowIndex(QRandomGenerator::global()->bounded(1, 3));
  emit runStarted();
  m_preparationTimer.start();
}
