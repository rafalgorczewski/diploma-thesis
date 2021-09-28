import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Qt.labs.platform
import Backend.Calibration
import Backend.Recorder
import Backend.Configurator

ApplicationWindow {
  id: root

  width: 1920
  height: 1080
  visible: true
  title: "MI BCI"
  flags: Qt.CustomizeWindowHint | Qt.Window

  Material.theme: Material.Dark

  font.pointSize: 16

  function calibrationArrowDisable() {
    calibrationArrowLeft.opacity = 0
    calibrationArrowRight.opacity = 0
  }

  function calibrationSelectArrow(index) {
    if (index === 1) {
      calibrationArrowLeft.opacity = 1
    } else if (index === 2) {
      calibrationArrowRight.opacity = 1
    }
  }

  function addNewChannel(channel, name) {
    var element = electrodesModel.find(function (item) {
      return item.channel === channel
    })
    if (element === null) {
      var newElectrode = {
        "channel": channel,
        "name": name
      }
      electrodesModel.append(newElectrode)
    }
  }

  function removeChannel(channel) {
    for (var i = 0; i < electrodesModel.count; ++i) {
      if (electrodesModel.get(i).channel === channel) {
        electrodesModel.remove(i)
        break
      }
    }
  }

  function addNewBand(min, max) {
    var element = bandsModel.find(function (item) {
      return item.bandBegin === min && item.bandEnd === max
    })
    if (element === null && max >= min) {
      var newBand = {
        "bandBegin": min,
        "bandEnd": max
      }
      bandsModel.append(newBand)
    }
  }

  FileDialog {
    id: calibrationFileDialog
    defaultSuffix: "csv"
    fileMode: FileDialog.OpenFile
    nameFilters: ["CSV (*.csv)"]
    onAccepted: {
      var path = file.toString()
      path = path.replace(/^(file:\/{3})/, "")
      var cleanPath = decodeURIComponent(path)
      backendRecorder.loadClassifierData(cleanPath)
      panel.classifyButton.enabled = true
    }
  }

  FileDialog {
    id: configSaveFileDialog
    defaultSuffix: "json"
    fileMode: FileDialog.SaveFile
    nameFilters: ["JSON (*.json)"]
    onAccepted: {
      var path = file.toString()
      path = path.replace(/^(file:\/{3})/, "")
      var cleanPath = decodeURIComponent(path)
      configurator.saveConfiguration(cleanPath)
    }
  }

  FileDialog {
    id: configLoadFileDialog
    defaultSuffix: "json"
    fileMode: FileDialog.OpenFile
    nameFilters: ["JSON (*.json)"]
    onAccepted: {
      var path = file.toString()
      path = path.replace(/^(file:\/{3})/, "")
      var cleanPath = decodeURIComponent(path)
      configurator.loadConfiguration(cleanPath)
    }
  }

  Calibration {
    id: backendCalibration
    preparationTime: panel.preparationTime
    imageryTime: panel.imageryTime
    cooldownTime: panel.cooldownTime

    onCalibrationStarted: {
      panel.calibrateButton.enabled = false
      panel.classifyButton.enabled = false
    }
    onRunStarted: {
      calibrationArrowDisable()
    }
    onPreparationPhaseStarted: {
      backendRecorder.calibrateRecord(preparationTime, 0)
    }
    onImageryPhaseStarted: {
      calibrationSelectArrow(arrowIndex)
      backendRecorder.calibrateRecord(imageryTime, arrowIndex)
    }
    onCooldownPhaseStarted: {
      calibrationArrowDisable()
      backendRecorder.calibrateRecord(cooldownTime, 0)
    }
    onCalibrationFinished: {
      panel.calibrateButton.enabled = true
      panel.classifyButton.enabled = true

      var str = configurator.fileName
      backendRecorder.saveClassifierData((String(str).slice(
                                            String(str).lastIndexOf(
                                              "/") + 1)).replace(".json", ""))
      backendRecorder.trainClassifier()
    }
  }

  Recorder {
    id: backendRecorder

    lslChannel: panel.lslChannel
    channelsModel: electrodesModel
    bandsModel: bandsModel

    onChannelPowerChanged: {
      var element = electrodesModel.find(function (item) {
        return item.channel === channel
      })
      element.alpha = power
    }

    onCurrentBodyPartChanged: {
      classificationLayout.currentIndex = bodyPart
    }
  }

  Configurator {
    id: configurator

    channelsModel: electrodesModel
    runsCount: panel.runsCount
    preparationTime: panel.preparationTime
    imageryTime: panel.imageryTime
    cooldownTime: panel.cooldownTime
    bandsModel: bandsModel

    onNewChannelLoaded: addNewChannel(number, name)
    onNewBandLoaded: addNewBand(min, max)
  }

  ListModel {
    id: bandsModel
    function find(criteria) {
      for (var i = 0; i < bandsModel.count; ++i)
        if (criteria(bandsModel.get(i)))
          return bandsModel.get(i)
      return null
    }
  }

  ListModel {
    id: electrodesModel
    function find(criteria) {
      for (var i = 0; i < electrodesModel.count; ++i)
        if (criteria(electrodesModel.get(i)))
          return electrodesModel.get(i)
      return null
    }
  }

  ThWindowBar {
    id: toolbar

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: 32
  }

  ThPanel {
    id: panel

    anchors.top: toolbar.bottom
    anchors.bottom: parent.bottom

    width: root.width / 3
  }

  TabBar {
    id: tabBar

    anchors.left: panel.right
    anchors.top: toolbar.bottom
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    TabButton {
      text: "Elektrody"
    }
    TabButton {
      text: "Kalibracja"
    }
    TabButton {
      text: "Klasyfikacja"
    }
  }

  StackLayout {
    width: tabBar.width
    height: tabBar.height - tabBar.contentHeight
    anchors.bottom: tabBar.bottom
    anchors.right: tabBar.right
    currentIndex: tabBar.currentIndex

    Item {
      id: electrodesTab
      GridView {
        id: electrodesView
        cellHeight: 128
        cellWidth: 128
        anchors.fill: parent
        model: electrodesModel
        delegate: Rectangle {
          width: electrodesView.cellWidth
          height: electrodesView.cellHeight

          Text {
            text: channel + " | " + name
            anchors.centerIn: parent
          }
        }
      }
    }
    Item {
      id: calibrationTab
      Image {
        id: calibrationArrowLeft
        anchors.left: parent.left
        width: root.width / 3.5

        x: calibrationTab.x + (calibrationTab.width / 2) - (width / 2)
        y: calibrationTab.y + (calibrationTab.height / 2) - (height / 2)
        source: "gfx/arrow_left.png"
      }

      Label {
        anchors.centerIn: parent

        text: "✕"
        font.pointSize: 40
      }

      Image {
        id: calibrationArrowRight
        anchors.right: parent.right
        width: root.width / 3.5

        x: calibrationTab.x + (calibrationTab.width / 2) - (width / 2)
        y: calibrationTab.y + (calibrationTab.height / 2) - (height / 2)
        source: "gfx/arrow_right.png"
      }
    }
    Item {
      id: classificationTab
      Item {
        id: classificationArrow
        StackLayout {
          id: classificationLayout
          currentIndex: backendRecorder.currentBodyPart
          Item {}
          Image {
            x: classificationTab.x + (classificationTab.width / 2) - (width / 2)
            y: classificationTab.y + (classificationTab.height / 2) - (height / 2)
            source: "gfx/arrow_left.png"
          }
          Image {
            x: classificationTab.x + (classificationTab.width / 2) - (width / 2)
            y: classificationTab.y + (classificationTab.height / 2) - (height / 2)
            source: "gfx/arrow_right.png"
          }
        }
      }
    }
  }
}
