#include "recorderbackend.hpp"

#include <chrono>

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

void RecorderBackend::createClassifier()
{
  std::vector<int> channels = getChannels();
  std::vector<std::pair<int, int>> bands = getBands();
  m_classifier = std::make_unique<th::Classifier>(channels.size() * bands.size());
}

void RecorderBackend::trainClassifier()
{
  if (m_classifier) {
    m_classifier->train();
  }
}

void RecorderBackend::saveClassifierData()
{
  m_classifier->save_data();
}

void RecorderBackend::calibrateRecordAsync(int seconds, int bodyPart)
{
  using namespace std::chrono_literals;

  std::vector<int> channels = getChannels();
  std::vector<std::pair<int, int>> bands = getBands();

  for (int second = 1; second <= seconds; ++second) {
    m_streamReader.read(1000ms);

    eig::VectorXd input(channels.size() * bands.size());
    for (int i = 0; i < channels.size(); ++i) {
      for (int j = 0; j < bands.size(); ++j) {
        const double power = m_streamReader.spectrum(channels[i]).band_power(bands[j].first, bands[j].second);
        input((i * bands.size()) + j) = power;
      }
    }
    m_classifier->feed_data(th::BodyPart(bodyPart), input);

    m_streamReader.clear();
  }
}

void RecorderBackend::classifyRecordAsync()
{
  using namespace std::chrono_literals;

  std::vector<int> channels = getChannels();
  std::vector<std::pair<int, int>> bands = getBands();

  while (true) {
    m_streamReader.read(1000ms);

    eig::VectorXd input(channels.size() * bands.size());
    for (int i = 0; i < channels.size(); ++i) {
      for (int j = 0; j < bands.size(); ++j) {
        const double power = m_streamReader.spectrum(channels[i]).band_power(bands[j].first, bands[j].second);
        input((i * bands.size()) + j) = power;
      }
    }
    m_currentBodyPart = static_cast<int>((*m_classifier)(input));
    emit currentBodyPartChanged(m_currentBodyPart);

    m_streamReader.clear();
  }
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
