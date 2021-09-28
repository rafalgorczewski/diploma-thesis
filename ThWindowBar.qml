import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
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

  Label {
    anchors.left: toolbar.left
    height: toolbar.height

    text: "MI BCI"
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
