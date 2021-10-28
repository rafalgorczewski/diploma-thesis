#include "recorderbackend.hpp"

#include <chrono>
#include <cstddef>
#include <utility>

#include <QtConcurrent/QtConcurrent>
#include <QVector>

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

void RecorderBackend::classifyRecord(int runTime)
{
  m_futureWatcher.waitForFinished();
  auto future = QtConcurrent::run([this, runTime] {
    classifyRecordAsync(runTime);
  });
  m_futureWatcher.setFuture(future);
}

void RecorderBackend::trainClassifier()
{
  m_futureWatcher.waitForFinished();
  m_classifier.train();
  emit classifierTrained();
}

QVector<QVector<double>> RecorderBackend::getClassifierProjectedData() const
{
  const auto& projectedData = m_classifier.getProjectedData();
  QVector<QVector<double>> projectedDataVec;
  for (int i = 0; i < projectedData.rows; ++i) {
    projectedDataVec.push_back(QVector<double>{projectedData.at<double>(i, 0), projectedData.at<double>(i, 1)});
  }
  return projectedDataVec;
}

QVector<int> RecorderBackend::getClassifierLabels() const
{
  const auto& labels = m_classifier.getLabels();
  QVector<int> labelsVec;
  for (int i = 0; i < labels.rows; ++i) {
    labelsVec.push_back(labels.at<int>(i, 0));
  }
  return labelsVec;
}

void RecorderBackend::saveClassifierData(QString configName)
{
  m_futureWatcher.waitForFinished();
  m_classifier.save_data(configName.toStdString());
}

void RecorderBackend::loadClassifierData(QString file)
{
  m_futureWatcher.waitForFinished();
  clearClassifierData();
  m_classifier.load_data(file.toStdString());
  emit classifierTrained();
}

void RecorderBackend::clearClassifierData()
{
  m_futureWatcher.waitForFinished();
  m_classifier = {};
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

  cv::Mat input{};
  for (int second = 1; second <= seconds; ++second) {
    m_streamReader.read(1000ms);

    for (std::size_t i = 0; i < channels.size(); ++i) {
      QVector<double> channelPowers;
      channelPowers.reserve(bands.size());
      for (std::size_t j = 0; j < bands.size(); ++j) {
        const double power = m_streamReader.spectrum(channels[i]).band_power(bands[j].first, bands[j].second);
        input.push_back(power);
        channelPowers.push_back(power);
      }

      emit channelPowersChanged(channels[i], channelPowers);
    }

    m_streamReader.clear();
  }
  m_classifier.feed_data(bodyPart, input.t());
}

void RecorderBackend::classifyRecordAsync(int runTime)
{
  using namespace std::chrono_literals;

  m_streamReader.flush();

  std::vector<int> channels = getChannels();
  std::vector<std::pair<int, int>> bands = getBands();

  QVector<QVector<double>> secondPowers(runTime, QVector<double>(channels.size() * bands.size(), 0));
  int runSec = 0;
  while (!m_stopRequested) {
    m_streamReader.read(1000ms);

    QVector<double> input;
    for (std::size_t i = 0; i < channels.size(); ++i) {
      QVector<double> channelPowers;
      for (std::size_t j = 0; j < bands.size(); ++j) {
        const double power = m_streamReader.spectrum(channels[i]).band_power(bands[j].first, bands[j].second);
        input.push_back(power);
        channelPowers.push_back(power);
      }

      emit channelPowersChanged(channels[i], channelPowers);
    }

    secondPowers[runSec] = input;

    const auto transformedBuffer = transformBuffer(secondPowers, runSec, runTime);

    m_currentBodyPart = m_classifier(transformedBuffer);

    const auto projectedInput = m_classifier.projectInput(transformedBuffer);
    QVector<double> labelsVec;
    for (int i = 0; i < projectedInput.cols; ++i) {
      labelsVec.push_back(projectedInput.at<double>(0, i));
    }

    emit newInput(labelsVec);
    emit currentBodyPartChanged(m_currentBodyPart);

    m_streamReader.clear();
    ++runSec;
    runSec %= runTime;
  }
  m_stopRequested = false;
}

cv::Mat RecorderBackend::transformBuffer(QVector<QVector<double>> input, int currentSecond, int runTime)
{
  cv::Mat transformed{};

  for (int sec = currentSecond + 1; sec < runTime; ++sec) {
    for (int i = 0; i < input[sec].size(); ++i) {
      transformed.push_back(input[sec][i]);
    }
  }
  for (int sec = 0; sec <= currentSecond; ++sec) {
    for (int i = 0; i < input[sec].size(); ++i) {
      transformed.push_back(input[sec][i]);
    }
  }

  return transformed.t();
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
