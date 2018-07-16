import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Item {
    property var file_names: ["File one", "File two", "File Three", "File Four"]
    property var states: ["uploaded", "uploaded", "uploaded", "uploaded"]
    property bool loading: false


    // TODO : GET HISTORY INFOMATION FROM SERVER
    //    Component.onCompleted: backend.file_history()


    Flickable {
        id:flickable
        anchors {
            fill: parent
            margins: dp(32)
        }
        contentHeight: Math.max(content.implicitHeight, height)

        Connections {
            target: backend
            //            onHistoryLoaded: {
            //                console.log("onFileLoaded");
            //                file_names = _file_names;
            //                file_authors = _file_authors;
            //                file_sizes = _file_sizes;
            //                loading = false;
            //            }
        }

        ProgressCircle {
            anchors.centerIn: parent
            visible: loading
        }

        Column {
            id: content
            anchors.fill: parent
            visible: !loading
            Repeater {
                model: file_names
                delegate: ListItem.Subtitled {
                    iconName:"file"
                    text: file_names[index]
                    subText: states[index]
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
