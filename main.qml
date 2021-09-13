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
  flags: Qt.FramelessWindowHint | Qt.Window

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
      classifyButton.enabled = true
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
    preparationTime: preparationTimeSpinBox.value
    imageryTime: imageryTimeSpinBox.value
    cooldownTime: cooldownTimeSpinBox.value

    onCalibrationStarted: {
      calibrateButton.enabled = false
      classifyButton.enabled = false
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
      calibrateButton.enabled = true
      classifyButton.enabled = true

      var str = configurator.fileName
      backendRecorder.saveClassifierData((String(str).slice(
                                            String(str).lastIndexOf(
                                              "/") + 1)).replace(".json", ""))
      backendRecorder.trainClassifier()
    }
  }

  Recorder {
    id: backendRecorder

    lslChannel: lslChannelField.text
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
    runsCount: runsSpinBox.value
    preparationTime: preparationTimeSpinBox.value
    imageryTime: imageryTimeSpinBox.value
    cooldownTime: cooldownTimeSpinBox.value
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

  Rectangle {
    id: toolbar

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: 32
    color: Material.accentColor

    MouseArea {
      anchors.fill: parent

      property variant clickPos: Qt.point(0, 0)

      onPressed: {
        clickPos = Qt.point(mouseX, mouseY)
      }
      onPositionChanged: {
        root.x = root.x + mouseX - clickPos.x
        root.y = root.y + mouseY - clickPos.y
      }
    }

    Rectangle {
      id: closeRect

      anchors.right: toolbar.right
      height: toolbar.height
      width: 32

      color: Material.accentColor

      Label {
        text: "✕"
      }

      MouseArea {
        anchors.fill: parent
        onClicked: root.close()
      }
    }

    Rectangle {
      anchors.right: closeRect.left
      height: toolbar.height
      width: 32

      color: Material.accentColor

      Label {
        text: "―"
      }

      MouseArea {
        anchors.fill: parent
        onClicked: root.showMinimized()
      }
    }
  }

  Rectangle {
    id: panelRect

    width: 512 + (panelColumn.x * 2)
    anchors.top: toolbar.bottom
    anchors.bottom: parent.bottom

    color: "black"

    ScrollView {
      width: 512 + (panelColumn.x * 2)
      height: root.height - 32
      clip: true

      ColumnLayout {
        id: panelColumn

        x: 32
        y: 32
        spacing: 16

        Row {
          Button {
            text: "Zapisz konfigurację"
            width: panelRect.width / 2.25
            onClicked: configSaveFileDialog.open()
          }
          Button {
            text: "Wczytaj konfigurację"
            width: panelRect.width / 2.25
            onClicked: {
              configLoadFileDialog.open()

              runsSpinBox.value = configurator.runsCount
              preparationTimeSpinBox.value = configurator.preparationTime
              imageryTimeSpinBox.value = configurator.imageryTime
              cooldownTimeSpinBox.value = configurator.cooldownTime
            }
          }
        }

        GroupBox {
          id: addElectrodeBox
          Layout.fillWidth: true

          title: "Nowa elektroda"

          ColumnLayout {

            Row {
              spacing: 10

              Label {
                text: "Kanał:"
              }

              SpinBox {
                id: newElectrodeChannel
                from: 0
                to: 40

                value: 0
              }

              Label {
                text: "Nazwa:"
              }

              TextField {
                id: newElectrodeName
                text: "Nienazwana"
              }
            }
            Row {
              Button {
                text: "Dodaj"
                width: panelRect.width / 2.35

                onClicked: addNewChannel(newElectrodeChannel.value,
                                         newElectrodeName.text)
              }
              Button {
                text: "Usuń"
                width: panelRect.width / 2.35

                onClicked: removeChannel(newElectrodeChannel.value)
              }
            }
          }
        }
        GroupBox {
          id: lslBox
          Layout.fillWidth: true

          title: "LSL"

          Row {
            spacing: 10

            TextField {
              id: lslChannelField
              width: 350

              text: "MyAudioStream"
            }

            Button {
              id: lslConnectButton

              text: "Podłącz LSL"

              onClicked: {
                backendRecorder.resolveStream()
                enabled = false
              }
            }
          }
        }
        GroupBox {
          id: calibrationBox
          Layout.fillWidth: true

          title: "Kalibracja"

          Row {
            spacing: 10
            Label {
              text: "Przebiegi:"
            }
            SpinBox {
              id: runsSpinBox
              width: panelRect.width / 3.25
              from: 1
              to: 100

              value: 1
            }

            Button {
              id: loadCalibrationButton
              width: panelRect.width / 6.5
              enabled: !lslConnectButton.enabled

              text: "Wczytaj"

              onClicked: {
                calibrationFileDialog.open()
              }
            }

            Button {
              id: calibrateButton
              width: panelRect.width / 6.5
              enabled: !lslConnectButton.enabled

              text: "Kalibruj"

              onClicked: {
                calibrateButton.enabled = false
                backendCalibration.calibrate(runsSpinBox.value)
                tabBar.currentIndex = 1
              }
            }
          }
        }
        Row {
          Button {
            id: classifyButton
            width: panelRect.width / 2.35
            enabled: !lslConnectButton.enabled && !calibrateButton.enabled
            text: "Rejestruj"

            onClicked: {
              enabled = false
              stopButton.enabled = true
              backendRecorder.classifyRecord()
              tabBar.currentIndex = 2
            }
          }
          Button {
            id: stopButton
            width: panelRect.width / 2.35
            enabled: false
            text: "Zatrzymaj"

            onClicked: {
              backendRecorder.stopRecording()
              enabled = false
              classifyButton.enabled = true
            }
          }
        }
        GroupBox {
          Layout.fillWidth: true

          title: "Czasy trwania sekcji [s]"
          Row {

            SpinBox {
              id: preparationTimeSpinBox
              width: 160

              from: 1
              to: 20

              value: 2
            }
            SpinBox {
              id: imageryTimeSpinBox
              width: 160

              from: 1
              to: 20

              value: 7
            }
            SpinBox {
              id: cooldownTimeSpinBox
              width: 160

              from: 1
              to: 20

              value: 2
            }
          }
        }
        GroupBox {
          id: bandsBox
          Layout.fillWidth: true

          title: "Pasma [Hz]"

          Column {
            Row {
              Button {
                text: "Dodaj"
                width: bandsBox.width / 3
                onClicked: addNewBand(
                             Number.fromLocaleString(bandBeginSpinBox.text),
                             Number.fromLocaleString(bandEndSpinBox.text))
              }
              TextField {
                id: bandBeginSpinBox
                text: "8"
                horizontalAlignment: TextInput.AlignHCenter
                width: panelRect.width / 3.9
                validator: IntValidator {
                  bottom: 1
                  top: 44000
                }
              }
              Label {
                text: "_"
              }

              TextField {
                id: bandEndSpinBox
                text: "12"
                horizontalAlignment: TextInput.AlignHCenter
                width: panelRect.width / 3.9
                validator: IntValidator {
                  bottom: 1
                  top: 44000
                }
              }
            }
            ListView {
              id: bandsView

              anchors.left: parent.left
              anchors.right: parent.right
              height: 256

              model: bandsModel
              delegate: Rectangle {
                color: "white"
                width: bandsView.width + 14
                height: 32
                Text {
                  text: bandBegin + " — " + bandEnd
                  anchors.centerIn: parent

                  font.pixelSize: 26
                }
              }
            }
          }
        }
      }
    }
  }

  TabBar {
    id: tabBar

    anchors.left: panelRect.right
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
