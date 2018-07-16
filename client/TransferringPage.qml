import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1


Item {
    property var file_names:[]

    property var file_state: []

    property var transferring_processes: []


    Connections{
        target: backend
        onProcess_report:{
            console.debug("process report")
            file_names = _file_names;
            file_state = _file_states;
            transferring_processes = _file_processes;
        }

    }


    Flickable {
        id:flickable
        anchors {
            fill: parent
            margins: dp(32)
        }
        contentHeight: Math.max(content.implicitHeight, height)

        Column {
            id: content
            anchors.fill: parent
            Repeater {
                model: file_names
                delegate: ListItem.Subtitled {
                    iconName:"file"
                    text: file_names[index]
                    subText: file_state[index]
                    valueText: transferring_processes[index]
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
    BottomActionSheet {
        id: actionSheet
        actions: [

            Action {
                iconName: "download"
                name: "Pause"
                enabled: false
            },

            Action {
                iconName: "settings"
                name: "Cancel"
                enabled: false
            }
        ]
    }
}
