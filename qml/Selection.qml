import QtQuick 2.0
import QtLocation 5.0
import QtPositioning 5.3
import Sailfish.Silica 1.0

import "./utils.js" as Util

MapQuickItem {

    anchorPoint.x: sourceItem.width / 2
    anchorPoint.y: sourceItem.height / 2
    height: sourceItem.height
    width: sourceItem.width
    visible: false
    coordinate: Util.home()

    sourceItem: Item {
        height: image.height
        width: image.width


        Image {
            id: image
            smooth: false
            source: app.getIcon("crosshairs")
        }
    }

    z: 600

}
