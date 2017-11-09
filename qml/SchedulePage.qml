import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {

    DialogHeader {
        id: header
        acceptText: "Clear"
        cancelText: "Stops"
    }

    SilicaListView {

        id: scheduleView
        model: timetableModel


        anchors {
            top: header.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        delegate: Component {
            ScheduleDelegate {}
        }

        VerticalScrollDecorator {}
    }
}
