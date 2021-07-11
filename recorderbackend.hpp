#ifndef RECORDERBACKEND_HPP
#define RECORDERBACKEND_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>

#include <QObject>
#include <qqml.h>
#include <QString>
#include <QAbstractListModel>
#include <QFutureWatcher>

#include "streamreader.hpp"
#include "bodypart.hpp"
#include "classifier.hpp"

class RecorderBackend : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString lslChannel MEMBER m_lslChannel NOTIFY lslChannelChanged)
  Q_PROPERTY(QAbstractListModel* channelsModel MEMBER m_channelsModel NOTIFY channelsModelChanged)
  Q_PROPERTY(QAbstractListModel* bandsModel MEMBER m_bandsModel NOTIFY bandsModelChanged)
  Q_PROPERTY(int currentBodyPart MEMBER m_currentBodyPart NOTIFY currentBodyPartChanged)
  QML_ELEMENT
public:
  RecorderBackend();
  explicit RecorderBackend(QObject *parent);

  Q_INVOKABLE void resolveStream();

  Q_INVOKABLE void calibrateRecord(int seconds, int bodyPart);
  Q_INVOKABLE void classifyRecord();

  Q_INVOKABLE void trainClassifier();

signals:
  void lslChannelChanged(QString);
  void channelsModelChanged(QAbstractListModel*);
  void bandsModelChanged(QAbstractListModel*);
  void channelPowerChanged(int channel, double power);
  void currentBodyPartChanged(int bodyPart);

private:
  std::vector<int> getChannels();
  std::vector<std::pair<int, int>> getBands();
  void calibrateRecordAsync(int seconds, int bodyPart);
  void classifyRecordAsync();

private:
  th::StreamReader m_streamReader;
  QString m_lslChannel;

  std::unique_ptr<th::Classifier> m_classifier;

  QAbstractListModel* m_channelsModel;
  QAbstractListModel* m_bandsModel;
  QFutureWatcher<void> m_futureWatcher;

  int m_currentBodyPart{};
};

#endif // RECORDERBACKEND_HPP
