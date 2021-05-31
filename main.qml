import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Backend.Calibration

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
        text: "＿"
      }

      MouseArea {
        anchors.fill: parent
        onClicked: root.showMinimized()
      }
    }
  }

  Rectangle {
    id: panelRect

    width: rootColumn.width + (rootColumn.x * 2)
    anchors.top: toolbar.bottom
    anchors.bottom: parent.bottom

    color: "black"

    StackLayout {
      currentIndex: tabBar.currentIndex

      Item {}

      Item {
        ColumnLayout {
          id: rootColumn

          x: 32
          y: 64
          spacing: 16

          GroupBox {
            id: rootBox
            width: 512

            title: "LSL"

            Row {
              spacing: 10

              Button {
                id: lslConnectButton

                text: "Podłącz LSL"
              }

              TextField {
                id: lslChannelField
                width: 350

                text: "MyAudioStream"
              }
            }
          }

          Button {
            Layout.fillWidth: true

            text: "Kalibruj"
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
    anchors.fill: tabBar
    currentIndex: tabBar.currentIndex

    Item {}
    Item {
      id: calibrationTab
      StackLayout {
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
          source: "gfx/arrow_left.png"
        }
      }
    }
  }
}
