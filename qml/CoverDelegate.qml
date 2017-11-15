import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {

    id: coverItem
    anchors.fill: parent

    Column {

        spacing: Theme.paddingMedium

        anchors.fill: parent

        Rectangle {
            color: model.color
            height: lineLabel.height + 2 * Theme.paddingSmall
            width: cover.width
            Label {
                id: lineLabel
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: Theme.highlightColor
                text: model.line
                font.pixelSize: Theme.fontSizeMedium
            }
        }


        Label {
            color: Theme.highlightColor
            text: model.origin
            elide: Text.ElideLeft
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            text: new Date(model.leaving).toLocaleString(Qt.locale(), "ddd HH:mm")
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            color: Theme.highlightColor
            text: model.destination
            elide: Text.ElideLeft
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            text: new Date(model.arriving).toLocaleString(Qt.locale(), "ddd HH:mm")
            font.pixelSize: Theme.fontSizeSmall
        }
    }
}
