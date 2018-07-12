import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem


TabbedPage {

    property var sections: [ "History", "Files", "Transferring" ]
    property string selectedComponent: sections[0]
    id: page
    title: qsTr("Fishield")
    actionBar.maxActionCount: navDrawer.enabled ? 2 : 5


    onGoBack: {
        confirmationDialog.show()
        event.accepted = true
    }

    actionBar.actions: [
        Action {
            iconName: "warning"
            name: "Warnings"
        },

        Action {
            iconName: "color"
            name: "Colors"
            hoverAnimation: true
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
            hoverAnimation: true
        },

        Action {
            iconName: "account"
            name: "Accounts"
        }
    ]

    backAction: navDrawer.action

    Repeater {
        id:tabRepeater
        model: !navDrawer.enabled ? sections : 0

        Tab {
            title: sections[index]
            property string selectedComponent: modelData
        }
    }


    NavigationDrawer {


        id: navDrawer
        enabled: page.width < dp(500)

        Flickable {
            anchors.fill: parent
            clip: true
            Column {
                anchors.fill: parent

                Repeater {
                    model: sections
                    width: parent.width

                    ListItem.Standard {
                        text: sections[index]
                        onClicked: {
                            mainWindow.selectedComponent = modelData
                            navDrawer.close()
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: colorPicker
        title: "Pick color"

        MenuField {
            id: selection
            model: ["Primary color", "TabHighlight Color", "Background color"]
        }

        negativeButton.visible: false

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
                                theme.tabHighlightColor = parent.color
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
                Loader {
                    anchors.fill: parent
                    asynchronous: true
                    visible: status == Loader.Ready
                    // selectedComponent will always be valid, as it defaults to the first component
                    source: {
                        if (navDrawer.enabled) {
                            return Qt.resolvedUrl("%Window.qml").arg(mainWindow.selectedComponent.replace(" ", ""))
                        } else {
                            return Qt.resolvedUrl("%Window.qml").arg(selectedComponent.replace(" ", ""))
                        }
                    }
                }

                ProgressCircle {
                    anchors.centerIn: parent
                    visible: example.status === Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }


}




