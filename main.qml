import QtQuick
import QtQuick.Window
import QtCharts
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Qt.labs.platform
import Backend.Calibration
import Backend.Recorder
import Backend.Configurator
import Backend.Plotter

ApplicationWindow {
  id: root

  width: 1920
  height: 1080
  visible: true
  title: "MI BCI"
  flags: Qt.CustomizeWindowHint | Qt.Window

  Material.theme: Material.Dark

  font.pointSize: 16

  function findIndexInModel(model, criteria) {
    for (var i = 0; i < model.count; ++i)
      if (criteria(model.get(i)))
        return i
    return null
  }

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

  function removeBand(bandBegin, bandEnd) {
    for (var i = 0; i < bandsModel.count; ++i) {
      var band = bandsModel.get(i)
      if (band.bandBegin === bandBegin && band.bandEnd === bandEnd) {
        bandsModel.remove(i)
        break
      }
    }
  }

  function getBandsAsStrings() {
    var names = []
    for (var i = 0; i < bandsModel.count; ++i) {
      var item = bandsModel.get(i)
      var name = "" + item.bandBegin + "-" + item.bandEnd
      names.push(name)
    }
    return names
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

    onChannelPowersChanged: {
      var electrodeIndex = findIndexInModel(electrodesModel, function (item) {
        return item.channel === channel
      })
      if (electrodeIndex !== null) {
        var electrode = electrodesView.itemAtIndex(electrodeIndex)
        plotter.plotPowers(electrode.powerSeries, powers,
                           electrode.powerSeriesYAxis)
      }
    }

    onCurrentBodyPartChanged: {
      classificationLayout.currentIndex = bodyPart
    }

    onClassifierTrained: {
      plotter.plotClassificationPoints(
            leftPointsSeries, rightPointsSeries, nonePointsSeries,
            backendRecorder.getClassifierProjectedData(),
            backendRecorder.getClassifierLabels(), pointsAxisX, pointsAxisY)
    }

    onNewInput: {
      plotter.plotClassificationPoints(inputPointSeries, input, pointsAxisX,
                                       pointsAxisY)
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

    onClearElectrodes: electrodesModel.clear()
    onClearBands: bandsModel.clear()

    onNewChannelLoaded: addNewChannel(number, name)
    onNewBandLoaded: addNewBand(min, max)
  }

  Plotter {
    id: plotter
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
      text: "Electrodes"
    }
    TabButton {
      text: "Calibration"
    }
    TabButton {
      text: "Classification"
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
      ListView {
        id: electrodesView
        anchors.fill: parent
        model: electrodesModel

        clip: true

        spacing: 4

        delegate: Rectangle {
          property alias powerSeries: powerSeries
          property alias powerSeriesYAxis: axisY
          color: Material.background

          width: electrodesView.width
          height: 256

          ChartView {
            id: chart
            anchors.fill: parent
            antialiasing: true
            legend.visible: false
            backgroundColor: "black"

            theme: ChartView.ChartThemeDark

            BarCategoryAxis {
              id: axisX
              color: "white"
              categories: getBandsAsStrings()
            }

            ValueAxis {
              id: axisY
              color: "white"
              min: 0
              max: 1
            }

            BarSeries {
              id: powerSeries
              axisX: axisX
              axisY: axisY
            }
          }

          Rectangle {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

            radius: 10

            color: "black"

            width: channelText.width * 3
            height: channelText.height * 1.2
            Text {
              id: channelText
              text: channel + " | " + name
              color: "white"
              anchors.centerIn: parent
              font.pointSize: 10
              font.bold: true
            }
            Rectangle {
              anchors.right: parent.right
              height: parent.height
              width: height
              radius: 10

              color: "white"

              Label {
                text: "✕"
                anchors.centerIn: parent
                font.pointSize: 10
                color: "black"
              }

              MouseArea {
                anchors.fill: parent
                onClicked: removeChannel(channel)
              }
            }
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
      ChartView {
        id: pointsChart
        anchors.fill: parent
        antialiasing: true
        backgroundColor: "black"

        theme: ChartView.ChartThemeDark

        ValueAxis {
          id: pointsAxisX
          color: "white"
          min: 0
          max: 1
        }
        ValueAxis {
          id: pointsAxisY
          color: "white"
          min: 0
          max: 1
        }

        ScatterSeries {
          id: leftPointsSeries
          name: "Left"

          color: "magenta"

          axisX: pointsAxisX
          axisY: pointsAxisY

          borderWidth: 1
          markerSize: 10
        }
        ScatterSeries {
          id: rightPointsSeries
          name: "Right"

          color: "cyan"

          axisX: pointsAxisX
          axisY: pointsAxisY

          borderWidth: 1
          markerSize: 10
        }
        ScatterSeries {
          id: nonePointsSeries
          name: "None"

          color: "yellow"

          axisX: pointsAxisX
          axisY: pointsAxisY

          borderWidth: 1
          markerSize: 10
        }
        ScatterSeries {
          id: inputPointSeries
          name: "Input"
          markerShape: ScatterSeries.MarkerShapeRectangle
          borderWidth: 1
          markerSize: 25

          color: "red"

          axisX: pointsAxisX
          axisY: pointsAxisY
        }

        Item {
          id: classificationArrow
          opacity: 0.25
          StackLayout {
            id: classificationLayout
            currentIndex: backendRecorder.currentBodyPart
            Item {}
            Image {
              scale: 0.5

              source: "gfx/arrow_left.png"
              x: classificationTab.x + (classificationTab.width / 2) - (width / 2)
              y: classificationTab.y + (classificationTab.height / 2) - (height / 2)
            }
            Image {
              scale: 0.5

              source: "gfx/arrow_right.png"
              x: classificationTab.x + (classificationTab.width / 2) - (width / 2)
              y: classificationTab.y + (classificationTab.height / 2) - (height / 2)
            }
          }
        }
      }
    }
  }
}
