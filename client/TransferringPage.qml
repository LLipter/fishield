import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1


Item {
    property var file_names:[]

    property var file_state: []

    property var transferring_processes: []

    property var taskids: []

    property var selectid: 0

    Connections{
        target: backend
        onProcess_report:{
            console.debug("process report")
            file_names = _file_names;
            file_state = _file_states;
            transferring_processes = _file_processes;
            taskids = _task_ids;
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
                        actionSheet.open();
                        selectid = taskids[index];
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
                iconName: "pause"
                name: "Pause"
                onTriggered: {
                    backend.pause_task(selectid);
                }
            },

            Action {
                iconName: "refresh"
                name: "Resume"
                onTriggered: {
                    backend.resume_task(selectid);
                }
            },

            Action {
                iconName: "cancel"
                name: "Cancel"
                enabled: false
            }
        ]
    }
}
