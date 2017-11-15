import QtQuick 2.0
import Sailfish.Silica 1.0
import QtLocation 5.0


Page {

    allowedOrientations: Orientation.Landscape | Orientation.Portrait

    z: 100

    onOrientationChanged: map.changed = true

    Component.onCompleted: {
        app.cycleButton = cycleButton
        app.map = map
        app.scaleBar = scaleBar
    }

    StateButton {id: cycleButton}
    MyMap {id: map}
    ScaleBar {id: scaleBar}

}

