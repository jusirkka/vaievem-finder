import QtQuick 2.2
import Sailfish.Silica 1.0

Page {

    SilicaFlickable {

        anchors.fill: parent

        PageHeader {
            id: header
            title: "Settings"
        }


        ComboBox {
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            anchors {
                left: parent.left
                right: parent.right
                top: header.bottom
                leftMargin: Theme.horizontalPageMargin
            }

            label: "Maximum walking distance "
            currentIndex: Math.ceil(timetableModel.walkingDistance - 1)

            menu: ContextMenu {
                MenuItem {text: "one kilometer"}
                MenuItem {text: "two kilometers"}
                MenuItem {text: "three kilometers"}
            }

            onCurrentIndexChanged: {
                timetableModel.walkingDistance = currentIndex + 1
            }

        }

    }
}
