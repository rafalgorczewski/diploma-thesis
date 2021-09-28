import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
  id: panelRect

  property alias preparationTime: preparationTimeSpinBox.value
  property alias imageryTime: imageryTimeSpinBox.value
  property alias cooldownTime: cooldownTimeSpinBox.value
  property alias lslChannel: lslChannelField.text
  property alias runsCount: runsSpinBox.value

  property alias classifyButton: classifyButton
  property alias calibrateButton: calibrateButton
  color: "black"

  ScrollView {
    anchors.fill: parent
    clip: true
    contentHeight: 64 + configurationSeparator.height + lslBox.height
                   + addElectrodeBox.height + bandsBox.height
                   + calibrationCategory.height + sectionTimesBox.height
                   + calibrationBox.height + settingsBreak.height
                   + configurationSaveLoad.height + recordingSeparator.height
                   + lslConnectButton.height + arrowFromLsl.height
                   + calibrationStartLoad.height + arrowFromCalibration.height
                   + recordingStartStop.height

    Item {
      id: scrollingItem

      anchors.fill: parent
      anchors.rightMargin: 32
      anchors.leftMargin: 32

      ThHorizontalSeparator {
        id: configurationSeparator
        text: "Stream settings"

        anchors.left: parent.left
        anchors.right: parent.right
      }

      GroupBox {
        id: lslBox
        title: "LSL stream"

        anchors.top: configurationSeparator.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        RowLayout {
          anchors.fill: parent
          Label {
            text: "Stream name:"
          }

          TextField {
            id: lslChannelField
            Layout.fillWidth: true

            text: "MyAudioStream"
          }
        }
      }

      GroupBox {
        id: addElectrodeBox
        title: "Electrodes"

        anchors.top: lslBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ColumnLayout {
          anchors.fill: parent
          RowLayout {
            spacing: 10

            Label {
              text: "Channel:"
              Layout.fillWidth: true
            }

            SpinBox {
              id: newElectrodeChannel
              Layout.fillWidth: true

              from: 0
              to: 40
              value: 0
            }

            Label {
              text: "Name:"
              Layout.fillWidth: true
            }

            TextField {
              id: newElectrodeName
              text: "Unnamed"
              Layout.fillWidth: true
            }
          }
          Button {
            text: "Add"
            Layout.fillWidth: true

            onClicked: addNewChannel(newElectrodeChannel.value,
                                     newElectrodeName.text)
          }
        }
      }

      GroupBox {
        id: bandsBox
        title: "Bands"

        anchors.top: addElectrodeBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        height: panelRect.height / 3

        ColumnLayout {
          anchors.fill: parent
          RowLayout {
            TextField {
              id: bandBeginSpinBox
              Layout.fillWidth: true
              text: "8"
              horizontalAlignment: TextInput.AlignHCenter
              validator: IntValidator {
                bottom: 1
                top: 44000
              }
            }

            Label {
              text: "Hz"
            }

            Rectangle {
              height: 1
              width: 16
              color: "white"
            }

            TextField {
              id: bandEndSpinBox
              Layout.fillWidth: true
              text: "12"
              horizontalAlignment: TextInput.AlignHCenter
              validator: IntValidator {
                bottom: 1
                top: 44000
              }
            }

            Label {
              text: "Hz"
            }

            Button {
              text: "Add"
              Layout.fillWidth: true
              onClicked: addNewBand(
                           Number.fromLocaleString(bandBeginSpinBox.text),
                           Number.fromLocaleString(bandEndSpinBox.text))
            }
          }
          ListView {
            id: bandsView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.vertical: ScrollBar {
              id: scroll
              active: true
            }

            spacing: 4

            model: bandsModel
            delegate: Rectangle {
              id: bandRect

              color: "transparent"
              border.width: 2
              border.color: "white"
              width: bandsView.width - scroll.width
              height: 32

              Label {
                text: bandBegin + " Hz — " + bandEnd + " Hz"
                anchors.fill: parent
                horizontalAlignment: TextInput.AlignHCenter

                font.pointSize: parent.height / 2
              }
              Rectangle {
                anchors.right: parent.right
                height: parent.height
                width: height

                color: "white"

                Label {
                  text: "✕"
                  anchors.centerIn: parent
                  font.pointSize: 10
                  color: "black"
                }

                MouseArea {
                  anchors.fill: parent
                  onClicked: removeBand(bandBegin, bandEnd)
                }
              }
            }
          }
        }
      }

      ThHorizontalCategory {
        id: calibrationCategory
        text: "Calibration settings"

        anchors.top: bandsBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right
      }

      GroupBox {
        id: sectionTimesBox
        anchors.top: calibrationCategory.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        title: "Section lengths [s]"
        RowLayout {
          anchors.fill: parent
          SpinBox {
            id: preparationTimeSpinBox
            Layout.fillWidth: true

            from: 1
            to: 20

            value: 2
          }
          SpinBox {
            id: imageryTimeSpinBox
            Layout.fillWidth: true

            from: 1
            to: 20

            value: 7
          }
          SpinBox {
            id: cooldownTimeSpinBox
            Layout.fillWidth: true

            from: 1
            to: 20

            value: 2
          }
        }
      }

      GroupBox {
        id: calibrationBox

        anchors.top: sectionTimesBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        title: "Session"

        RowLayout {
          anchors.fill: parent
          Label {
            Layout.fillWidth: true
            text: "Runs count:"
          }
          SpinBox {
            Layout.fillWidth: true
            id: runsSpinBox
            from: 1
            to: 100

            value: 1
          }
        }
      }

      ThHorizontalBreak {
        id: settingsBreak

        anchors.top: calibrationBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right
      }

      RowLayout {
        id: configurationSaveLoad

        anchors.top: settingsBreak.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
          Layout.fillWidth: true
          text: "Save settings"
          onClicked: configSaveFileDialog.open()
        }

        Button {
          Layout.fillWidth: true
          text: "Load settings"
          onClicked: {
            configLoadFileDialog.open()

            runsSpinBox.value = configurator.runsCount
            preparationTimeSpinBox.value = configurator.preparationTime
            imageryTimeSpinBox.value = configurator.imageryTime
            cooldownTimeSpinBox.value = configurator.cooldownTime
          }
        }
      }

      ThHorizontalSeparator {
        id: recordingSeparator
        text: "Recording"

        anchors.top: configurationSaveLoad.bottom
        anchors.left: parent.left
        anchors.right: parent.right
      }

      Button {
        id: lslConnectButton
        anchors.top: recordingSeparator.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        text: "Connect to LSL"

        onClicked: {
          backendRecorder.resolveStream()
          enabled = false
        }
      }

      Label {
        id: arrowFromLsl

        anchors.top: lslConnectButton.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        horizontalAlignment: TextInput.AlignHCenter

        text: "then"
      }

      RowLayout {
        id: calibrationStartLoad

        anchors.top: arrowFromLsl.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
          id: calibrateButton
          Layout.fillWidth: true
          enabled: !lslConnectButton.enabled

          text: "Start calibration"

          onClicked: {
            calibrateButton.enabled = false
            backendCalibration.calibrate(runsSpinBox.value)
            tabBar.currentIndex = 1
          }
        }

        Label {
          text: " or "
        }

        Button {
          id: loadCalibrationButton
          Layout.fillWidth: true
          enabled: !lslConnectButton.enabled

          text: "Load calibration"

          onClicked: {
            calibrationFileDialog.open()
          }
        }
      }

      Label {
        id: arrowFromCalibration

        anchors.top: calibrationStartLoad.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        horizontalAlignment: TextInput.AlignHCenter

        text: "then"
      }

      RowLayout {
        id: recordingStartStop

        anchors.top: arrowFromCalibration.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
          id: classifyButton
          Layout.fillWidth: true
          enabled: !lslConnectButton.enabled && !calibrateButton.enabled
          text: "Start classification"

          onClicked: {
            enabled = false
            stopButton.enabled = true
            backendRecorder.classifyRecord()
            tabBar.currentIndex = 2
          }
        }
        Button {
          id: stopButton
          enabled: false
          text: "Stop"

          onClicked: {
            backendRecorder.stopRecording()
            enabled = false
            classifyButton.enabled = true
          }
        }
      }
    }
  }
}
