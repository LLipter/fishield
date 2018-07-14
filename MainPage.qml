import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem

TabbedPage {
    id: mainpage
    title: qsTr("Fishield")
    property var sections: [ "History", "Files", "Transferring" ]
    property var sectionTitles: [ qsTr("History"), qsTr("Files"), qsTr("Downloading/Uploading") ]
    property string selectedComponent: sections[0]

    actionBar.maxActionCount: navDrawer.enabled ? 3 : 4

    actions: [
        Action {
            iconName: "warning"
            name: "Dummy error"
            onTriggered: demo.showError("Something went wrong", "Do you want to retry?", "Close", true)
        },

        Action {
            iconName: "color"
            name: "Colors"
            onTriggered: colorPicker.show()
        },

        Action {
            iconName: "settings"
            name: "Settings"
            hoverAnimation: true
        },

        Action {
            iconName: "language"
            name: "Language"
            enabled: false
        },

        Action {
            iconName: "account"
            name: "Accounts"
        }
    ]

    backAction: navDrawer.action

    NavigationDrawer {
        id: navDrawer
        enabled: mainpage.width < dp(500)
        onEnabledChanged: smallLoader.active = enabled

        Flickable {
            anchors.fill: parent

            contentHeight: Math.max(content.implicitHeight, height)

            Column {
                id: content
                anchors.fill: parent

                Repeater {
                    model: sections

                    delegate: Column {
                        width: parent.width

                        ListItem.Standard {
                            text: sectionTitles[index]
                            selected: modelData === mainWindow.selectedComponent
                            onClicked: {
                                mainWindow.selectedComponent = modelData
                                navDrawer.close()
                            }
                        }
                    }
                }
            }
        }
    }

    Repeater {
        id:tabRepeater
        model: !navDrawer.enabled ? sections : 0

        delegate: Tab {
            title: sectionTitles[index]

            property string selectedComponent: modelData
            property var section: modelData

            sourceComponent: tabDelegate
        }
    }

    Loader {
        id: smallLoader
        anchors.fill: parent
        sourceComponent: tabDelegate

        property var section: []
        visible: active
        active: false
    }

    Dialog {
        id: colorPicker
        title: "Pick color"
        negativeButton.visible: false


        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: dp(160)
        }

        Grid {
            columns: 7
            spacing: dp(8)

            Repeater {
                model: [
                    "red", "pink", "purple", "deepPurple", "indigo",
                    "blue", "lightBlue", "cyan", "teal", "green",
                    "lightGreen", "lime", "yellow", "amber", "orange",
                    "deepOrange", "grey", "blueGrey", "brown", "black",
                    "white"
                ]

                Rectangle {
                    width: dp(30)
                    height: dp(30)
                    radius: dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent

                        onPressed: {
                            switch(selection.selectedIndex) {
                            case 0:
                                theme.primaryColor = parent.color
                                break;
                            case 1:
                                theme.accentColor = parent.color
                                break;
                            case 2:
                                theme.backgroundColor = parent.color
                                break;
                            }
                        }
                    }
                }
            }
        }

    }


    Component {
        id: tabDelegate

        Item {

            Flickable {
                id: flickable
                anchors.fill:parent
                clip: true
                contentHeight: Math.max(example.implicitHeight + 40, height)
                Loader {
                    id: example
                    anchors.fill: parent
                    asynchronous: true
                    visible: status == Loader.Ready
                    // selectedComponent will always be valid, as it defaults to the first component
                    source: {
                        if (navDrawer.enabled) {
                            return Qt.resolvedUrl("%Page.qml").arg(mainWindow.selectedComponent.replace(" ", ""))
                        } else {
                            return Qt.resolvedUrl("%Page.qml").arg(selectedComponent.replace(" ", ""))
                        }
                    }
                }

                ProgressCircle {
                    anchors.centerIn: parent
                    visible: example.status == Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }


}


