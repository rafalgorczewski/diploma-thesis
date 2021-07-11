#ifndef CONFIGURATOR_HPP
#define CONFIGURATOR_HPP

#include <QObject>
#include <QString>
#include <qqml.h>
#include <QAbstractListModel>

class Configurator : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QAbstractListModel* channelsModel MEMBER m_channelsModel NOTIFY channelsModelChanged)
  Q_PROPERTY(int runsCount MEMBER m_runsCount NOTIFY runsCountChanged)
  Q_PROPERTY(int preparationTime MEMBER m_preparationTime NOTIFY preparationTimeChanged)
  Q_PROPERTY(int imageryTime MEMBER m_imageryTime NOTIFY imageryTimeChanged)
  Q_PROPERTY(int cooldownTime MEMBER m_cooldownTime NOTIFY cooldownTimeChanged)
  Q_PROPERTY(QAbstractListModel* bandsModel MEMBER m_bandsModel NOTIFY bandsModelChanged)
  QML_ELEMENT
public:
  Configurator();
  explicit Configurator(QObject *parent);

  Q_INVOKABLE void saveConfiguration(QString name);
  Q_INVOKABLE void loadConfiguration(QString name);

signals:
  void channelsModelChanged();
  void runsCountChanged();
  void preparationTimeChanged();
  void imageryTimeChanged();
  void cooldownTimeChanged();
  void bandsModelChanged();

  void newChannelLoaded(int number, QString name);
  void newBandLoaded(int min, int max);

private:
  QAbstractListModel* m_channelsModel;
  int m_runsCount;
  int m_preparationTime;
  int m_imageryTime;
  int m_cooldownTime;
  QAbstractListModel* m_bandsModel;
};

#endif // CONFIGURATOR_HPP
