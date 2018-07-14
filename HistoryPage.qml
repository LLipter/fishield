import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Item {
    property var file_names: ["File one", "File two", "File Three", "File Four"]
    property var sections: ["Docs"]
    property var file_authors: ["zuolin", "vergil", "john", "unknown"]
    property var m_times: ["2 hours ago", "one day ago", "one mouth ago", "unknown"]
    property var file_sizes: ["100k", "10M", "100M", "80k"]
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
                model: sections
                delegate: Column {
                    width: parent.width

                    ListItem.Subheader {
                        text: sections[index]
                    }
                    Repeater {
                        model: file_names
                        delegate: ListItem.Subtitled {
                            iconName:"file"
                            text: qsTr(modelData)
                            subText: "uploaded by " + file_authors[index]
                            valueText: m_times[index]
                            maximumLineCount: 2
                            backgroundColor: "white"
                            onClicked: {
                                actionSheet.open()
                            }
                        }
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
            },

            Action {
                iconName: "settings"
                name: "Details"
                hasDividerAfter: true
            },

            Action {
                iconName: "forward"
                name: "Move"
            },

            Action {
                iconName: "delete"
                name: "Delete"
            },

            Action {
                iconName: "create"
                name: "Rename"
            }
        ]
    }
}
