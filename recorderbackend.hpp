#ifndef RECORDERBACKEND_HPP
#define RECORDERBACKEND_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>

#include <opencv2/opencv.hpp>

#include <QObject>
#include <qqml.h>
#include <QString>
#include <QAbstractListModel>
#include <QFutureWatcher>
#include <QVector>

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
  Q_INVOKABLE void classifyRecord(int runTime);

  Q_INVOKABLE void trainClassifier();
  Q_INVOKABLE QVector<QVector<double>> getClassifierProjectedData() const;
  Q_INVOKABLE QVector<int> getClassifierLabels() const;

  Q_INVOKABLE void saveClassifierData(QString configName);
  Q_INVOKABLE void loadClassifierData(QString file);
  Q_INVOKABLE void clearClassifierData();

  Q_INVOKABLE void stopRecording();

signals:
  void lslChannelChanged(QString);
  void channelsModelChanged(QAbstractListModel*);
  void bandsModelChanged(QAbstractListModel*);
  void currentBodyPartChanged(int bodyPart);

  void channelPowersChanged(int channel, QVector<double> powers);
  void classifierTrained();

  void newInput(QVector<double> input);

private:
  std::vector<int> getChannels();
  std::vector<std::pair<int, int>> getBands();
  void calibrateRecordAsync(int seconds, int bodyPart);
  void classifyRecordAsync(int runTime);

  cv::Mat transformBuffer(QVector<QVector<double>> input, int currentSecond, int runTime);

private:
  th::StreamReader m_streamReader;
  QString m_lslChannel;

  th::Classifier m_classifier;

  QAbstractListModel* m_channelsModel;
  QAbstractListModel* m_bandsModel;
  QFutureWatcher<void> m_futureWatcher;

  int m_currentBodyPart{};
  bool m_stopRequested = false;
};

#endif // RECORDERBACKEND_HPP
