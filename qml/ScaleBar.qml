/* -*- coding: utf-8-unix -*-
 *
 * Copyright (C) 2014 Osmo Salomaa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtPositioning 5.3
import Sailfish.Silica 1.0

import "utils.js" as Util

Item {
    id: scaleBar
    anchors.bottom: parent.bottom
    anchors.bottomMargin: Theme.paddingLarge
    anchors.left: parent.left
    anchors.leftMargin: Theme.paddingSmall
    opacity: 0.9
    visible: scaleWidth > 0
    z: 600

    property real scaleWidth: 0
    property string text: ""

    Rectangle {
        id: base
        color: "black"
        height: Math.floor(Theme.pixelRatio * 3)
        width: scaleBar.scaleWidth
    }

    Rectangle {
        anchors.bottom: base.top
        anchors.left: base.left
        color: "black"
        height: Math.floor(Theme.pixelRatio * 10)
        width: Math.floor(Theme.pixelRatio * 3)
    }

    Rectangle {
        anchors.bottom: base.top
        anchors.right: base.right
        color: "black"
        height: Math.floor(Theme.pixelRatio * 10)
        width: Math.floor(Theme.pixelRatio * 3)
    }

    Text {
        anchors.bottom: base.top
        anchors.bottomMargin: Math.floor(Theme.pixelRatio * 4)
        anchors.horizontalCenter: base.horizontalCenter
        color: "black"
        font.bold: true
        font.family: "sans-serif"
        font.pixelSize: Math.round(Theme.pixelRatio * 18)
        horizontalAlignment: Text.AlignHCenter
        text: scaleBar.text
    }


    function update() {
        // Update scalebar for current zoom level and latitude.
        var upper = map.toCoordinate(Qt.point(map.width / 2, 0));
        var lower = map.toCoordinate(Qt.point(map.width / 2, map.height));
        // 10 000 km = 90 degrees
        var fullLen = Math.abs(upper.latitude - lower.latitude) * 1e6 / 9.
        // target scaleWidth is 0.4 * screen.width
        var scaleParams = Util.scaleDisplay(fullLen / map.height * map.width * 0.4)
        scaleBar.scaleWidth = scaleParams.value * map.height / fullLen
        scaleBar.text = scaleParams.string
    }
}
