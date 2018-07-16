import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Dialogs 1.2 as QDialogs

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
        root.loading = true;
        root.timeout = 5;
        countDowm.start();
        backend.getFileList(currentpath);
    }

    Timer {
        id:countDowm;
        repeat: true;
        interval: 1000;
        onTriggered: {
            root.timeout--;
            if (root.timeout < 0) {
                root.loading = false
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
            root.loading = false;
            countDowm.stop();


            file_names = _file_names;
            is_directory = _is_dir;
            file_sizes = _file_sizes;
            m_times = _m_times;

            console.debug(currentpath, "onFileLoaded");
            console.debug(file_names)
        }

        onNewdir_created:{
            loadfilelist();
        }

        onFile_removed:{
            loadfilelist();
        }
    }

    Flickable {
        id:flickable
        anchors {
            fill: parent
            margins: dp(32)
        }
        visible: !root.loading
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
        visible: root.loading
    }

    ActionButton {
        id: uploadbutton

        visible: loginpage.can_upload

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: dp(16)
        }

        action: Action {
            shortcut: "Ctrl+U"
            onTriggered:{
                fileDialog.selectFolder = false;
                fileDialog.open();
            }
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

    ActionButton {
        id: newdir

        anchors {
            right: parent.right
            bottom: refreshbutton.top
            margins: dp(16)
        }

        action: Action {
            shortcut: "Ctrl+N"
            onTriggered: {
                console.debug("NEWDIR");
                textFieldDialog.open();
            }
        }
        iconName: "newdir"
    }

    ActionButton {
        id: deletedir

        anchors {
            right: parent.right
            bottom: newdir.top
            margins: dp(16)
        }

        action: Action {
            onTriggered: {
                if(currentpath == "/")
                    return;
                console.debug("REMOVE DIR");
                delete_confirm.open();
            }
        }
        iconName: "delete"
    }

    Dialog {
        id: delete_confirm
        width: dp(300)
        title: "Delete this directory"
        text: "Do you want to remove the current working directory? All files within this directory and the directory itself will be removed."
        positiveButtonText: "confirm"
        negativeButtonText: "cancel"
        onAccepted: {
            backend.remove_file(currentpath);
            gobackbutton.action.trigger();
        }
    }



    Dialog {
        id: textFieldDialog
        title: "Name of new directory"
        hasActions: true

        TextField {
            id: optionText
            width: parent.width
            placeholderText: "Directory name"
        }

        onAccepted: {
            console.debug("new dirname", optionText.text);
            backend.newdir(currentpath + optionText.text);
        }
    }



    QDialogs.FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls);
            console.debug(fileDialog.fileUrls, currentpath);
            if(selectFolder == false)
                backend.upload(fileDialog.fileUrls, currentpath);
            else{
                backend.download(fileDialog.fileUrls,
                                 currentpath,
                                 file_names[clickedindex])
            }
        }
    }

    BottomActionSheet {
        id: actionSheet
        actions: [

            Action {
                iconName: "download"
                name: "Download"

                onTriggered: {
                    fileDialog.selectFolder = true;
                    fileDialog.open();
                }

                visible: loginpage.can_download
            },

            Action {
                iconName: "settings"
                name: "Details"
//                hasDividerAfter: true
                visible: false
            },

            Action {
                iconName: "forward"
                name: "Move"
                visible: false
            },

            Action {
                iconName: "delete"
                name: "Delete"

                onTriggered: {
                    backend.remove_file(currentpath + file_names[clickedindex])
                }
            },

            Action {
                iconName: "create"
                name: "Rename"
                visible: false
            }
        ]
    }


}

