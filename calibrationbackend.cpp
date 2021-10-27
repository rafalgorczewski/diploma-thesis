#include "calibrationbackend.hpp"

#include <algorithm>
#include <random>

CalibrationBackend::CalibrationBackend() : CalibrationBackend(nullptr)
{

}

CalibrationBackend::CalibrationBackend(QObject* parent) : QObject(parent) {
  m_preparationTimer.setSingleShot(true);
  m_imageryTimer.setSingleShot(true);
  m_cooldownTimer.setSingleShot(true);

  connect(&m_preparationTimer, &QTimer::timeout, [this]{
    emit preparationPhaseFinished();
    emit imageryPhaseStarted();
    m_imageryTimer.start();
  });
  connect(&m_imageryTimer, &QTimer::timeout, [this]{
    emit imageryPhaseFinished();
    emit cooldownPhaseStarted();
    m_cooldownTimer.start();
  });
  connect(&m_cooldownTimer, &QTimer::timeout, [this]{
    ++m_currentRun;
    emit cooldownPhaseFinished();
    emit runFinished();

    if (m_currentRun < m_runsMaxCount) {
      startRun();
    } else {
      emit calibrationFinished();
    }
  });
}

int CalibrationBackend::arrowIndex() const {
  return static_cast<int>(m_bodyPart);
}

void CalibrationBackend::calibrate(int runsMaxCount) {
  updateTimerIntervals();

  m_runsMaxCount = runsMaxCount;
  fillPartsQueue();

  emit calibrationStarted();
  startRun();
}

void CalibrationBackend::startRun() {
  m_bodyPart = m_partsQueue.front();
  m_partsQueue.pop_front();

  emit runStarted();
  emit preparationPhaseStarted();
  m_preparationTimer.start();
}

void CalibrationBackend::fillPartsQueue()
{
  m_partsQueue = {};
  for (int i = 0; i < m_runsMaxCount; ++i) {
    m_partsQueue.push_back(th::BodyPart(i % 3));
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(m_partsQueue.begin(), m_partsQueue.end(), g);
}

void CalibrationBackend::updateTimerIntervals()
{
  m_preparationTimer.setInterval(m_preparationTime * 1000);
  m_imageryTimer.setInterval(m_imageryTime * 1000);
  m_cooldownTimer.setInterval(m_cooldownTime * 1000);
}
