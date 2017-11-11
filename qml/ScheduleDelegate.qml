import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {

    id: scheduleItem
    contentHeight: lineLabel.height + 2 * Theme.paddingMedium

    Row {

        y: Theme.paddingMedium
        x: Theme.horizontalPageMargin
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin

        spacing: Theme.paddingMedium

        Rectangle {
            id: lineLabel
            color: model.color
            height: Theme.itemSizeMedium
            width: Theme.itemSizeMedium
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: Theme.highlightColor
                text: model.line
                font.pixelSize: Theme.fontSizeHuge
            }
        }

        Column {

            anchors.verticalCenter: lineLabel.verticalCenter
            anchors.rightMargin: Theme.horizontalPageMargin
            spacing: Theme.paddingSmall

            Label {
                width: lineLabel.width * 2.75
                color: Theme.highlightColor
                text: model.origin
                elide: Text.ElideLeft
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                width: lineLabel.width * 2.75
                color: Theme.highlightColor
                text: model.destination
                elide: Text.ElideLeft
                font.pixelSize: Theme.fontSizeMedium
            }

        }

        Column {

            anchors.verticalCenter: lineLabel.verticalCenter
            spacing: Theme.paddingSmall


            Label {
                text: new Date(model.leaving).toLocaleString(Qt.locale(), "ddd HH:mm")
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                text: new Date(model.arriving).toLocaleString(Qt.locale(), "ddd HH:mm")
                font.pixelSize: Theme.fontSizeMedium
            }
        }

    }
}
