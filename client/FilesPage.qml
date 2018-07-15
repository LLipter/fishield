import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Dialogs 1.2

Item {
    id: root

    property var file_names: []
    property var is_directory: []
    property var m_times: []
    property var file_sizes: []

    property bool loading: true
    property string currentpath: "/"
    property int clickedindex: 0
    property int timeout: 5

    Component.onCompleted: {
        loadfilelist();
    }

    function loadfilelist(){
        loginpage.loading = true;
        loginpage.timeout = 5;
        countDowm.start();
        backend.getFileList(currentpath);
    }

    Timer {
        id:countDowm;
        repeat: true;
        interval: 1000;
        onTriggered: {
            loginpage.timeout--;
            if (loginpage.timeout < 0) {
                loginpage.loading = false
                errorlabel.visible = true
                backend.timeout();
            }
        }
    }

    Label {
        id: errorlabel
        text : "timeout, please retry"
        visible: false
        color: "red"
        anchors.centerIn: parent
    }

    Connections{
        target: backend
        onFileLoaded: {
            loading = false;
            countDowm.stop();


            file_names = _file_names;
            is_directory = _is_dir;
            file_sizes = _file_sizes;
            m_times = _m_times;

            console.debug(currentpath, "onFileLoaded");
            console.debug(file_names)
        }
    }

    Flickable {
        id:flickable
        anchors {
            fill: parent
            margins: dp(32)
        }
        visible: !loading
        contentHeight: Math.max(content.implicitHeight, height)

        Column {
            id: content
            anchors.fill: parent
            width: parent.width

            ListItem.Subheader {
                text: "Current path: " + currentpath
                backgroundColor: "lightskyblue"
            }

            Repeater {
                model: file_names
                delegate: ListItem.Subtitled {
                    iconName: is_directory[index] ? "folder" : "file"
                    text: file_names[index]
                    subText: file_sizes[index]
                    valueText: m_times[index]
                    backgroundColor: "white"
                    property int file_index: index
                    onClicked: {
                        if(!is_directory[index]){
                            clickedindex = file_index;
                            actionSheet.open();
                        }else{
                            currentpath += file_names[index];
                            currentpath += "/";
                            loadfilelist();
                        }

                    }

                }
            }
        }
    }

    Scrollbar {
        flickableItem: flickable
    }

    ProgressCircle {
        anchors.centerIn: parent
        visible: loading
    }

    ActionButton {
        id: uploadbutton

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: dp(16)
        }

        action: Action {
            text: "&UPLOAD"
            shortcut: "Ctrl+U"
            onTriggered: filePicker.visible = true
        }
        iconName: "upload"
    }

    ActionButton {
        id: refreshbutton

        anchors {
            right: parent.right
            bottom: uploadbutton.top
            margins: dp(16)
        }

        action: Action {
            text: "&REFRESH"
            shortcut: "F5"
            onTriggered: {
                console.debug("REFRESH");
                loadfilelist();
            }
        }
        iconName: "refresh"

    }

    ActionButton {
        id: gobackbutton

        anchors {
            right: parent.right
            top: parent.top
            margins: dp(16)
        }

        action: Action {
            text: "&GOBACK"
            shortcut: "Backspace"
            onTriggered: {
                console.debug("GOBACK");
                if(currentpath == "/")
                    return;
                else{
                    var idx = currentpath.lastIndexOf('/', currentpath.length-2);
                    currentpath = currentpath.substring(0,idx+1);
                    loadfilelist();
                }
            }
        }
        iconName: "return"

    }





    // TODO : GET FILEPICKER
    FilePicker {
        id:filePicker
        anchors.fill: parent
        showDotAndDotDot: true
        nameFilters: "*.cpp"
        onFileSelected: {
            console.log("User selects file: " + currentFolder() + "/" +fileName)
            visible = false
        }
        visible: false
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

