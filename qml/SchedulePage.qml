import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    SilicaFlickable {

        anchors.fill: parent

        PageHeader {
            id: header
            title: "Bus Schedules"
        }

        SilicaListView {

            id: scheduleView
            model: timetableModel


            anchors {
                top: header.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                topMargin: 2 * Theme.paddingLarge
            }


            delegate: Component {ScheduleDelegate {}}

            ViewPlaceholder {
                enabled: scheduleView.count === 0
                text: "No schedules"
                hintText: "Double tap in the Map for a new search"
            }
        }

        VerticalScrollDecorator {}

        PullDownMenu {
            MenuItem {
                text: "About"
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                }
            }
            MenuItem {
                text: "Settings"
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
                }
            }
        }
    }

}
