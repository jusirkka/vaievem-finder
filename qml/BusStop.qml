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
    property string name: ""
    property int seq_num: 0
    property bool bubbleVisible: false

    Timer {
        interval: 5000
        repeat: false
        running: stop.bubbleVisible
        onTriggered: stop.bubbleVisible = false
    }

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

            MouseArea {
                anchors.fill: parent
                onClicked: stop.bubbleVisible = stop.seq_num > 0 && !stop.bubbleVisible
            }

        }

        Rectangle {
            id: bubble

            anchors.bottom: image.top
            anchors.left: image.left
            width: infoLabel.width + 2 * Theme.paddingLarge
            height: infoLabel.height + 2 * Theme.paddingLarge
            visible: stop.bubbleVisible
            color: "black"
            Label {
                id: infoLabel
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: "" + stop.seq_num + ". " + stop.name
                color: "white"
                font.pixelSize: Theme.fontSizeMedium
            }
        }
    }

    z: 600

    onBubbleVisibleChanged: {
        z = bubbleVisible ? 601 : 600
    }

}
