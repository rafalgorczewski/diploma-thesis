import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Backend.Calibration
import Backend.Recorder

ApplicationWindow {
  id: root

  width: 1920
  height: 1080
  visible: true
  title: "MI BCI"
  flags: Qt.FramelessWindowHint | Qt.Window

  Material.theme: Material.Dark

  font.pointSize: 16

  Calibration {
    id: backendCalibration
    preparationTime: 2
    imageryTime: 7
    cooldownTime: 2

    onCalibrationStarted: {
      calibrateButton.enabled = false
    }
    onRunStarted: {
      calibrationLayout.currentIndex = arrowIndex
    }
    onPreparationPhaseStarted: {
      arrow.opacity = 0.25
      backendRecorder.record(preparationTime, arrowIndex)
    }
    onImageryPhaseStarted: {
      arrow.opacity = 1.0
      backendRecorder.record(imageryTime, arrowIndex)
    }
    onCooldownPhaseStarted: {
      arrow.opacity = 0.05
      backendRecorder.record(cooldownTime, arrowIndex)
    }
    onCalibrationFinished: {
      calibrateButton.enabled = true
    }
  }

  Recorder {
    id: backendRecorder

    lslChannel: lslChannelField.text
    channelsModel: electrodesModel

    onChannelPowerChanged: {
      var element = electrodesModel.find(function (item) {
        return item.channel === channel
      })
      element.alpha = power
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

    width: rootCalibrationColumn.width + (rootCalibrationColumn.x * 2)
    anchors.top: toolbar.bottom
    anchors.bottom: parent.bottom

    color: "black"

    StackLayout {
      currentIndex: tabBar.currentIndex

      Item {
        ColumnLayout {
          x: 32
          y: 64
          spacing: 16

          GroupBox {
            id: addElectrodeBox
            width: 512

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
              Button {
                text: "Dodaj"
                Layout.fillWidth: true

                onClicked: {
                  var newElectrode = {
                    "channel": newElectrodeChannel.value,
                    "name": newElectrodeName.text,
                    "alpha": 0.0
                  }
                  electrodesModel.append(newElectrode)
                }
              }
            }
          }
        }
      }

      Item {
        ColumnLayout {
          id: rootCalibrationColumn

          x: 32
          y: 64
          spacing: 16

          GroupBox {
            id: lslBox
            width: 512

            title: "LSL"

            Row {
              spacing: 10

              Button {
                id: lslConnectButton

                text: "Podłącz LSL"

                onClicked: backendRecorder.resolveStream()
              }

              TextField {
                id: lslChannelField
                width: 350

                text: "MyAudioStream"
              }
            }
          }

          Button {
            id: calibrateButton
            Layout.fillWidth: true

            text: "Kalibruj"

            onClicked: backendCalibration.calibrate(10)
          }

          GroupBox {
            Layout.fillWidth: true
            label: CheckBox {
              id: alphaCheckBox
              checked: true
              text: "Alfa (mu) [Hz]"
            }

            RowLayout {
              Label {
                text: "Min"
              }
              SpinBox {
                from: 7
                to: 9

                value: 8
              }

              Label {
                text: "Max"
              }
              SpinBox {
                from: 10
                to: 13

                value: 12
              }
            }
          }

          GroupBox {
            Layout.fillWidth: true
            label: CheckBox {
              id: betaCheckBox
              checked: true
              text: "Beta [Hz]"
            }

            RowLayout {
              Label {
                text: "Min"
              }
              SpinBox {
                from: 12
                to: 15

                value: 13
              }

              Label {
                text: "Max"
              }
              SpinBox {
                from: 26
                to: 30

                value: 28
              }
            }
          }

          GroupBox {
            Layout.fillWidth: true
            label: CheckBox {
              id: bandCheckBox
              checked: false
              text: "Podpasma [Hz]"
            }

            RowLayout {
              Label {
                text: "Szerokość"
              }

              SpinBox {
                from: 1
                to: 3
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
            Text {
              text: "Alfa: " + alpha
              anchors.top: parent.bottom
            }
          }
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
    }
    Item {
      id: calibrationTab
      Item {
        id: arrow
        StackLayout {
          id: calibrationLayout
          currentIndex: backendCalibration.arrowIndex
          Item {}
          Image {
            x: calibrationTab.x + (calibrationTab.width / 2) - (width / 2)
            y: calibrationTab.y + (calibrationTab.height / 2) - (height / 2)
            source: "gfx/arrow_left.png"
          }
          Image {
            x: calibrationTab.x + (calibrationTab.width / 2) - (width / 2)
            y: calibrationTab.y + (calibrationTab.height / 2) - (height / 2)
            source: "gfx/arrow_right.png"
          }
        }
      }
    }
  }
}
