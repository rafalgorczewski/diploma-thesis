#include "configurator.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

Configurator::Configurator() : Configurator(nullptr)
{

}

Configurator::Configurator(QObject *parent) : QObject(parent)
{

}

void Configurator::saveConfiguration(QString name)
{
  nlohmann::json configuration;

  for (int i = 0; i < m_channelsModel->rowCount(); ++i) {
    const QModelIndex index = m_channelsModel->index(i, 0);
    const int channel = m_channelsModel->data(index, 0).toInt();
    const std::string name = m_channelsModel->data(index, 1).toString().toStdString();
    configuration["channels"][i] = { channel, name };
  }

  configuration["runsCount"] = m_runsCount;
  configuration["preparationTime"] = m_preparationTime;
  configuration["imageryTime"] = m_imageryTime;
  configuration["cooldownTime"] = m_cooldownTime;

  for (int i = 0; i < m_bandsModel->rowCount(); ++i) {
    const QModelIndex index = m_bandsModel->index(i, 0);
    const int min = m_bandsModel->data(index, 0).toInt();
    const int max = m_bandsModel->data(index, 1).toInt();
    configuration["bands"][i] = { min, max };
  }

  std::ofstream output(name.toStdString());
  output << configuration;

  m_fileName = name;
}

void Configurator::loadConfiguration(QString name)
{
  nlohmann::json configuration;
  std::ifstream input(name.toStdString());
  input >> configuration;

  emit clearElectrodes();
  auto channelsObject = configuration["channels"];
  for (auto channelObject : channelsObject) {
    const int number = channelObject[0];
    const std::string name = channelObject[1];

    emit newChannelLoaded(number, QString::fromStdString(name));
  }

  m_runsCount = configuration["runsCount"];
  m_preparationTime = configuration["preparationTime"];
  m_imageryTime = configuration["imageryTime"];
  m_cooldownTime = configuration["cooldownTime"];

  emit clearBands();
  auto bandsObject = configuration["bands"];
  for (auto bandObject : bandsObject) {
    const int min = bandObject[0];
    const int max = bandObject[1];

    emit newBandLoaded(min, max);
  }

  m_fileName = name;

  emit configurationLoaded();
}
