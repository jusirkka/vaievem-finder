import QtQuick 2.0
import QtLocation 5.0
import Sailfish.Silica 1.0

import "./utils.js" as Util

Map {
    id: map
    anchors.fill: parent
    minimumZoomLevel: 12
    zoomLevel: 14

    plugin: Plugin {
        name: "here"
        parameters: [
            PluginParameter {name: "app_id"; value: "AiDkv1NTXqtZ1bjWP7ft"},
            PluginParameter {name: "app_code"; value: "gUBu0JxovaxEVZC3oNagcQ"},
            PluginParameter {name: "mapping.cache.directory"; value: "/dev/null"},
            PluginParameter {name: "mapping.host"; value: "127.0.0.1:65536"}
        ]

    }

    property var selection: Selection {}
    property var first: Alpha {}
    property var second: Omega {}

    property bool halfZoom: false
    property var  tiles: []
    property var  stops: []
    property bool changed: true

    Component.onCompleted: {
        map.center = Util.home()
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (app.state !== 2) {
                map.selection.visible = true
                map.selection.coordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
            }
        }
        onDoubleClicked: {
            app.reset()
        }
    }

    Timer {
        interval: 500
        repeat: true
        running: app.running
        onTriggered: {
            if (map.changed) {
                map.updateTiles();
            }
        }
    }

    onCenterChanged: {
        // Ensure that tiles are updated after panning.
        // This gets fired ridiculously often, so keep simple.
        map.changed = true;
    }

    function addStops() {
        var component, stop;
        var lines = timetableModel.lines();
        for (var i = 0; i < lines.length; i++) {
            var stops = timetableModel.stops(lines[i]);
            for (var j = 0; j < stops.length; j++) {
                component = Qt.createComponent("BusStop.qml");
                stop = component.createObject(map);
                stop.coordinate = timetableModel.location(stops[j])
                stop.bg = timetableModel.color(lines[i])
                map.stops.push(stop);
                map.addMapItem(stop);
            }
        }
    }

    function clearStops() {
        for (var i = 0; i < map.stops.length; i++) {
            map.removeMapItem(map.stops[i]);
            map.stops[i].destroy();
        }
        map.stops = []
    }

    function getBoundingBox() {
        // Return currently visible [xmin, xmax, ymin, ymax].
        var nw = map.toCoordinate(Qt.point(0, 0));
        var se = map.toCoordinate(Qt.point(map.width, map.height));
        return [nw.longitude, se.longitude, se.latitude, nw.latitude];
    }

    function queueUpdate() {
        // Mark map as changed to trigger an update.
        map.changed = true;
    }

    function renderTile(props) {
        // Render tile from local image file.
        if (props.half_zoom !== map.halfZoom) {
            map.halfZoom = props.half_zoom;
            map.setZoomLevel(map.zoomLevel);
        }
        for (var i = 0; i < map.tiles.length; i++) {
            if (map.tiles[i].uid !== props.uid) continue;
            map.tiles[i].coordinate.latitude = props.nwy;
            map.tiles[i].coordinate.longitude = props.nwx;
            map.tiles[i].smooth = props.smooth;
            map.tiles[i].type = props.type;
            map.tiles[i].zOffset = props.z;
            map.tiles[i].zoomLevel = props.display_zoom +
                (props.half_zoom ? Util.constants.halfZoom : 0);
            map.tiles[i].uri = props.uri;
            map.tiles[i].setWidth(props);
            map.tiles[i].setHeight(props);
            map.tiles[i].setZ(map.zoomLevel);
            return;
        }
        // Add missing tile to collection.
        var component = Qt.createComponent("Tile.qml");
        var tile = component.createObject(map);
        tile.uid = props.uid;
        map.tiles.push(tile);
        map.addMapItem(tile);
        map.renderTile(props);
    }


    function showTile(uid) {
        // Show tile with given uid.
        for (var i = 0; i < map.tiles.length; i++) {
            if (map.tiles[i].uid !== uid) continue;
            map.tiles[i].setZ(map.zoomLevel);
            break;
        }
    }

    function updateTiles() {
        // Ask the Python backend to download missing tiles.
        if (!py.ready) return;
        if (map.width <= 0 || map.height <= 0) return;
        if (map.gesture.isPinchActive) return;
        var bbox = map.getBoundingBox();
        py.call("poor.app.update_tiles",
                [bbox[0], bbox[1], bbox[2], bbox[3],
                 Math.floor(map.zoomLevel),
                 Util.constants.canvasScaleFactor],
                null)

        map.changed = false
    }


}
