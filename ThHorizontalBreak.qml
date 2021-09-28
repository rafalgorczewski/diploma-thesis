import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: separator

  property int lineSize: 2

  height: line.height + (line.y * 2)

  Row {
    y: 8

    Rectangle {
      id: line

      y: 16
      height: lineSize
      width: separator.width
      color: "white"
    }
  }
}
