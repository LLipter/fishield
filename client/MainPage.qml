import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem

TabbedPage {
    id: mainpage
    title: qsTr("Fishield")
    actionBar.maxActionCount: navDrawer.enabled ? 3 : 4

    property var sections: ["Files", "Transferring", "History"]
    property var sectionTitles: [ qsTr("Files"), qsTr("Downloading/Uploading"), qsTr("History")]

    property string selectedComponent: sections[0]

    actions: [
        Action {
            iconName: "warning"
            name: "Dummy error"
            onTriggered: demo.showError("Something went wrong", "Do you want to retry?", "Close", true)
            enabled: false
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
            enabled: false
        },

        Action {
            iconName: "language"
            name: "Language"
            enabled: false
        },

        Action {
            iconName: "account"
            name: "Accounts"
            enabled: false
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
                            selected: modelData === mainpage.selectedComponent
                            onClicked: {
                                mainpage.selectedComponent = modelData
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


        positiveButtonText: "Done"

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

        onRejected: {
            // TODO set default colors again but we currently don't know what that is
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
                            return Qt.resolvedUrl("%Page.qml").arg(mainpage.selectedComponent.replace(" ", ""))
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

    Connections{
        target: backend
        onRelogin: {
            mainpage.forcePop();
        }
    }



}

