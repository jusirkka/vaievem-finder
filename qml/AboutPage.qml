import QtQuick 2.2
import Sailfish.Silica 1.0

Page {

    SilicaFlickable {

        anchors.fill: parent

        PageHeader {
            id: header
            title: "About"
        }

        Label {
            id: aboutLabel
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            anchors {
                left: parent.left
                right: parent.right
                top: header.bottom
                leftMargin: Theme.horizontalPageMargin
            }
            wrapMode: Text.Wrap
            textFormat: Text.StyledText
            text: "Vai e Vem Finder helps you to find a local bus schedule in Portimão, Portugal."
        }

        SectionHeader {
            id: authorHeader
            anchors {
                left: parent.left
                right: parent.right
                top: aboutLabel.bottom
                rightMargin: Theme.horizontalPageMargin
            }
            text: "Author"
        }

        Label {
            id: authorLabel
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            anchors {
                left: parent.left
                right: parent.right
                top: authorHeader.bottom
                leftMargin: Theme.horizontalPageMargin
            }
            text: "Jukka Sirkka"
        }

        SectionHeader {
            id: sourceHeader
            anchors {
                left: parent.left
                right: parent.right
                top: authorLabel.bottom
                rightMargin: Theme.horizontalPageMargin
            }
            text: "Source"
        }

        Button {
            id: sourceButton
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            anchors {
                left: parent.left
                right: parent.right
                top: sourceHeader.bottom
                leftMargin: Theme.horizontalPageMargin
                rightMargin: Theme.horizontalPageMargin
            }
            text: "github"
            onClicked: Qt.openUrlExternally("https://github.com/jusirkka/veievem-finder")
        }
    }
}
