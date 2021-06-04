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

void RecorderBackend::record(int seconds, int bodyPart)
{
  m_futureWatcher.waitForFinished();
  auto future = QtConcurrent::run([this, seconds, bodyPart] {
    asyncRecord(seconds, bodyPart);
  });
  m_futureWatcher.setFuture(future);
}

void RecorderBackend::asyncRecord(int seconds, int bodyPart)
{
  using namespace std::chrono_literals;

  std::vector<int> channels = getChannels();

  for (int second = 1; second <= seconds; ++second) {
    m_streamReader.read(1000ms);

    for (const int channel : channels) {
      double band_power = m_streamReader.spectrum(channel).band_power(8, 12);
      m_powersMap[th::BodyPart(bodyPart)][channel].push_back(band_power);
      emit channelPowerChanged(channel, band_power);
    }

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
