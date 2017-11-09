import QtQuick 2.0
import Sailfish.Silica 1.0

IconButton {

    z: 600
    anchors.bottom: parent.bottom
    anchors.bottomMargin: Theme.paddingSmall
    anchors.horizontalCenter: parent.horizontalCenter
    height: icon.sourceSize.height
    icon.smooth: false
    icon.source: app.getIcon("first")

}
