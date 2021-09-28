import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: separator

  property alias text: label.text
  property int lineSize: 2

  height: label.height + 16

  Row {
    y: 8

    Rectangle {
      y: label.y + (label.height / 2)
      height: lineSize
      width: (separator.width - label.width) / 2
      color: "white"
    }
    Label {
      id: label
      color: "white"
    }
    Rectangle {
      y: label.y + (label.height / 2)
      height: lineSize
      width: (separator.width - label.width) / 2
      color: "white"
    }
  }
}
