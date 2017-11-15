import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {

    id: cover

    SilicaListView {
        id: coverView
        anchors.fill: parent
        model: coverModel
        delegate: Component {
            CoverDelegate {
                width: cover.width
            }
        }
    }

    CoverPlaceholder {
        visible: coverView.count === 0

        Rectangle {
            anchors.fill: parent
            color: "#666666"
            width: cover.width
            height: cover.height

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                id: busImage
                source: "icons/busstop-200.png"
            }

            Label {
                anchors.top: busImage.bottom
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: Theme.highlightColor
                text: "No Schedules"
                font.pixelSize: Theme.fontSizeMedium
            }
        }
    }

    CoverActionList {

        CoverAction {
            iconSource: "image://theme/icon-cover-previous"
            onTriggered: coverModel.prev()
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: coverModel.next()
        }

    }
}

