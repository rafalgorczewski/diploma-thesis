#ifndef RECORDERBACKEND_HPP
#define RECORDERBACKEND_HPP

#include <unordered_map>
#include <vector>

#include <QObject>
#include <qqml.h>
#include <QString>
#include <QAbstractListModel>
#include <QFutureWatcher>

#include "streamreader.hpp"
#include "bodypart.hpp"
#include "signalpower.hpp"

class RecorderBackend : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString lslChannel MEMBER m_lslChannel NOTIFY lslChannelChanged)
  Q_PROPERTY(QAbstractListModel* channelsModel MEMBER m_channelsModel NOTIFY channelsModelChanged)
  QML_ELEMENT
public:
  RecorderBackend();
  explicit RecorderBackend(QObject *parent);

  Q_INVOKABLE void resolveStream();

  Q_INVOKABLE void record(int seconds, int bodyPart);

signals:
  void lslChannelChanged(QString);
  void channelsModelChanged(QAbstractListModel*);
  void channelPowerChanged(int channel, double power);

private:
  std::vector<int> getChannels();
  void asyncRecord(int seconds, int bodyPart);

private:
  th::StreamReader m_streamReader;
  QString m_lslChannel;

  std::unordered_map<th::BodyPart, th::SignalPower> m_powersMap;

  QAbstractListModel* m_channelsModel;
  QFutureWatcher<void> m_futureWatcher;
};

#endif // RECORDERBACKEND_HPP
