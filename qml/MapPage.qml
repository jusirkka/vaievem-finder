import QtQuick 2.0
import Sailfish.Silica 1.0
import QtLocation 5.0


Page {

    anchors.fill: parent
    height: Screen.height
    width: Screen.width
    z: 100

    Component.onCompleted: {
        app.cycleButton = cycleButton
        app.map = map
        console.log("completed")
    }

    StateButton {id: cycleButton}
    MyMap {id: map}
}

