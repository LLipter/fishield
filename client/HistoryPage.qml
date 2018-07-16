import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Item {
    property var file_names: ["File one", "File two", "File Three", "File Four"]
    property var file_states: ["uploaded", "uploaded", "uploaded", "uploaded"]

    Component.onCompleted: backend.file_history()


    Flickable {
        id:flickable
        anchors {
            fill: parent
            margins: dp(32)
        }
        contentHeight: Math.max(content.implicitHeight, height)

        Connections {
            target: backend
            onHistory_loaded: {
                console.log("onHistoryLoaded");
                file_names = _file_names;
                file_states = _file_states;

                console.debug(_file_names)
                console.debug(_file_states)
            }
        }

        Column {
            id: content
            anchors.fill: parent
            Repeater {
                model: file_names
                delegate: ListItem.Subtitled {
                    iconName:"file"
                    text: file_names[index]
                    subText: file_states[index]
                    backgroundColor: "white"
                    onClicked: {
                        actionSheet.open()
                    }
                }
            }
        }

    }

    Scrollbar {
        flickableItem: flickable
    }

    ActionButton {
        id: refreshbutton

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: dp(16)
        }

        action: Action {
            shortcut: "F5"
            onTriggered: {
                console.debug("REFRESH HISTORY");
                backend.file_history();
            }
        }
        iconName: "refresh"

    }

    BottomActionSheet {
        id: actionSheet


        actions: [

            Action {
                iconName: "download"
                name: "Download"
                enabled: false
            },

            Action {
                iconName: "settings"
                name: "Details"
                hasDividerAfter: true
                enabled: false
            },

            Action {
                iconName: "forward"
                name: "Move"
                enabled: false
            },

            Action {
                iconName: "delete"
                name: "Delete"
                enabled: false
            },

            Action {
                iconName: "create"
                name: "Rename"
                enabled: false
            }
        ]
    }
}
