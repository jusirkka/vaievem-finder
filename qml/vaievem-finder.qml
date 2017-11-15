import QtQuick 2.0
import Sailfish.Silica 1.0
import QtPositioning 5.3

ApplicationWindow {
    id: app

    initialPage: Component {MapPage {}}
    cover: Component {CoverPage {}}

    property var map: null
    property var cycleButton: null
    property var scaleBar: null
    property int pixelRatio: 100
    property var states: ["first", "second", "go"]
    property int state: 0
    property bool running: applicationActive

    Python {id: py}
    PositionSource {
        id: gps
        // nmeaSource: "nmea.log"
    }

    Component.onCompleted: {
        py.setHandler("queue-update", map.queueUpdate)
        py.setHandler("render-tile", map.renderTile)
        py.setHandler("show-tile", map.showTile)
        setPixelRatio()
        cycleButton.clicked.connect(app.cycleState)
        gps.start()
    }

    Keys.onPressed: {
        // Allow zooming with H/J keys on the emulator.
        if (event.key === Qt.Key_J)  {
            map.setZoomLevel(map.zoomLevel+1)
        } else if (event.key === Qt.Key_H) {
            map.setZoomLevel(map.zoomLevel-1)
        }
    }

    function setPixelRatio() {
        // Return path to icon suitable for user's screen,
        // finding the closest match to Theme.pixelRatio.
        var ratios = [1.00, 1.25, 1.50, 1.75, 2.00]
        var minIndex = -1, minDiff = 1000, diff
        for (var i = 0; i < ratios.length; i++) {
            diff = Math.abs(Theme.pixelRatio - ratios[i]);
            minIndex = diff < minDiff ? i : minIndex;
            minDiff = Math.min(minDiff, diff);
        }
        app.pixelRatio = Math.floor(100 * ratios[minIndex])
    }

    function getIcon(name) {
        return "icons/%1-%2.png".arg(name).arg(app.pixelRatio);
    }

    function cycleState() {
        if (state === 2) {
            map.selection.visible = false
            timetableModel.find(map.first.coordinate, map.second.coordinate)
            pageStack.push(Qt.resolvedUrl("SchedulePage.qml"))
            map.setupStops()
        } else if (map.selection.visible) {
            map[states[state]].visible = true
            map[states[state]].coordinate = map.selection.coordinate
            state = (state + 1) % 3
            map.selection.visible = false
            cycleButton.icon.source = getIcon(states[state])
        }
    }

    function reset() {
        state = 0
        map.first.visible = false
        map.second.visible = false
        map.selection.visible = false
        cycleButton.icon.source = getIcon(states[state])
        map.clearStops()
        timetableModel.reset()
    }
}

