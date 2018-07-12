import QtQuick 2.0
import Material 0.3
import Material.ListItems 0.1

Item {
    id: root
    width: 450;
    height: 220;
    anchors.centerIn: parent

    property bool loading : false
    property int timeout: 5
    property alias username: username.text


    Rectangle{
        id: container
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        color: "white"

        TextField{
            id: username
            anchors {
                top: parent.top
                topMargin: parent.height / 4
                horizontalCenter: parent.horizontalCenter
            }

            placeholderText: "Username"
            floatingLabel: true
            enabled: !loading

            onTextChanged: {
                hasError = false;
                helperText = ""
            }

        }

        TextField{
            id: password
            anchors {
                top: username.bottom
                topMargin: 30
                horizontalCenter: parent.horizontalCenter
            }

            placeholderText: "Password"
            floatingLabel: true
            enabled: !loading
            echoMode: TextInput.Password

            onTextChanged: {
                hasError = false;
                helperText = ""
            }

        }

        Button {
            id: login
            text: "Login"
            // https://material.io/design/environment/elevation.html#elevation-in-material-design
            elevation: 2
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: password.bottom
                topMargin: 25
            }
            enabled: !loading

            backgroundColor: "dodgerblue"
            onClicked: {
                timeoutlabel.visible = false
                if(username.text.length == 0){
                    username.hasError = true;
                    username.helperText = "please enter username"
                }else if(password.text.length == 0){
                    password.hasError = true;
                    password.helperText = "please enter password"
                }else{
                    loading = true;
                    timeout = 5;
                    countDowm.start();
                    login_backend.login(username.text, password.text);
                }
            }
        }

        Subheader{
            text: "Welcome"
        }

        ProgressCircle {
            anchors.centerIn: parent
            visible: loading
            color: "blue"
        }

        ActionButton {
            anchors {
                top: parent.top
                topMargin: 20
                horizontalCenter: parent.right
            }

            action: Action {
                id: addUser
                shortcut: "Ctrl+C"
                onTriggered: {
                    console.debug("addUser triggered")
                }
            }
            iconName: "add"
        }

        Timer {
            id:countDowm;
            repeat: true;
            interval: 1000;
            onTriggered: {
                root.timeout--;
                if (root.timeout < 0) {
                    loading = false
                    timeoutlabel.visible = true
                    root.timeout = 5
                    login_backend.timeout();
                }
            }
        }
        Label {
            id: timeoutlabel
            text : "login timeout, please retry"
            visible: false
            color: "red"
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: login.bottom
                topMargin: 5
            }
        }

    }


}
