#include "recorderbackend.hpp"

#include <chrono>
#include <cstddef>
#include <utility>
#include <opencv2/opencv.hpp>

#include <QtConcurrent/QtConcurrent>

RecorderBackend::RecorderBackend() : RecorderBackend(nullptr)
{

}

RecorderBackend::RecorderBackend(QObject *parent) : QObject(parent)
{

}

void RecorderBackend::resolveStream()
{
  m_streamReader.resolve_stream(m_lslChannel.toStdString());
}

void RecorderBackend::calibrateRecord(int seconds, int bodyPart)
{
  m_futureWatcher.waitForFinished();
  auto future = QtConcurrent::run([this, seconds, bodyPart] {
    calibrateRecordAsync(seconds, bodyPart);
  });
  m_futureWatcher.setFuture(future);
}

void RecorderBackend::classifyRecord()
{
  m_futureWatcher.waitForFinished();
  auto future = QtConcurrent::run([this] {
    classifyRecordAsync();
  });
  m_futureWatcher.setFuture(future);
}

void RecorderBackend::trainClassifier()
{
  m_futureWatcher.waitForFinished();
  m_classifier.train();
}

void RecorderBackend::saveClassifierData(QString configName)
{
  m_futureWatcher.waitForFinished();
  m_classifier.save_data(configName.toStdString());
}

void RecorderBackend::loadClassifierData(QString file)
{
  m_futureWatcher.waitForFinished();
  m_classifier = {};
  m_classifier.load_data(file.toStdString());
}

void RecorderBackend::stopRecording()
{
  m_stopRequested = true;
}

void RecorderBackend::calibrateRecordAsync(int seconds, int bodyPart)
{
  using namespace std::chrono_literals;

  std::vector<int> channels = getChannels();
  std::vector<std::pair<int, int>> bands = getBands();

  for (int second = 1; second <= seconds; ++second) {
    m_streamReader.read(1000ms);

    cv::Mat input{};
    for (std::size_t i = 0; i < channels.size(); ++i) {
      for (std::size_t j = 0; j < bands.size(); ++j) {
        const double power = m_streamReader.spectrum(channels[i]).band_power(bands[j].first, bands[j].second);
        input.push_back(power);
      }
    }
    m_classifier.feed_data(bodyPart, input.t());

    m_streamReader.clear();
  }
}

void RecorderBackend::classifyRecordAsync()
{
  using namespace std::chrono_literals;

  std::vector<int> channels = getChannels();
  std::vector<std::pair<int, int>> bands = getBands();

  while (!m_stopRequested) {
    m_streamReader.read(1000ms);

    cv::Mat input{};
    for (std::size_t i = 0; i < channels.size(); ++i) {
      for (std::size_t j = 0; j < bands.size(); ++j) {
        const double power = m_streamReader.spectrum(channels[i]).band_power(bands[j].first, bands[j].second);
        input.push_back(power);
      }
    }
    qDebug() << "Klasyfikacja...";
    m_currentBodyPart = m_classifier(input.t());
    qDebug() << "Wynik: " << m_currentBodyPart;
    emit currentBodyPartChanged(m_currentBodyPart);

    m_streamReader.clear();
  }
  m_stopRequested = false;
}

std::vector<int> RecorderBackend::getChannels() {
  std::vector<int> channels;
  for (int i = 0; i < m_channelsModel->rowCount(); ++i) {
    const QModelIndex index = m_channelsModel->index(i, 0);
    const int channel = m_channelsModel->data(index, 0).toInt();
    channels.push_back(channel);
  }
  return channels;
}

std::vector<std::pair<int, int>> RecorderBackend::getBands() {
  std::vector<std::pair<int, int>> bands;
  for (int i = 0; i < m_bandsModel->rowCount(); ++i) {
    const QModelIndex index = m_bandsModel->index(i, 0);
    const int min = m_bandsModel->data(index, 0).toInt();
    const int max = m_bandsModel->data(index, 1).toInt();
    bands.emplace_back(min, max);
  }
  return bands;
}
