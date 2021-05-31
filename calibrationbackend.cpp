#include "calibrationbackend.hpp"

CalibrationBackend::CalibrationBackend(QObject *parent) : QObject(parent) {

}

int CalibrationBackend::arrowIndex() const {
  return static_cast<int>(m_arrowIndex);
}
