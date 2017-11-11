import QtQuick 2.0
import QtLocation 5.0
import QtPositioning 5.3
import Sailfish.Silica 1.0

import "./utils.js" as Util

MapQuickItem {

    id: mark

    anchorPoint.x: i.x
    anchorPoint.y: i.y + i.height
    width: sourceItem.width
    height: sourceItem.height
    visible: false
    coordinate: Util.home()

    property int accuracy: 60

    sourceItem: Item {
        width: (r.width + Math.max(r.width, 2 * i.width)) / 2
        height: (r.height + Math.max(r.height, 2 * i.height)) / 2

        Rectangle {
            id: r
            width: mark.accuracy
            height: mark.accuracy
            color: "blue"
            opacity: 0.4
            x: 0
            y: Math.max(0, i.height - r.height / 2)
        }

        Image {
            id: i
            smooth: false
            source: app.getIcon("runner")
            x: r.width / 2
            y: Math.max(0, r.height / 2 - i.height)
        }
    }


    z: 600

}
