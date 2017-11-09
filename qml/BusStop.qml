import QtQuick 2.0
import QtLocation 5.0
import QtPositioning 5.3
import Sailfish.Silica 1.0

import "./utils.js" as Util

MapQuickItem {

    id: stop

    anchorPoint.x: 0
    anchorPoint.y: sourceItem.height
    height: sourceItem.height
    width: sourceItem.width
    visible: true
    coordinate: Util.home()

    property color bg: "#000000"

    sourceItem: Item {
        height: image.height
        width: image.width

        Rectangle {
            anchors.fill: parent
            opacity: 0.8
            color: stop.bg
        }

        Image {
            id: image
            smooth: false
            source: app.getIcon("busstop")
        }
    }

    z: 600

}
