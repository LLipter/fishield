import QtQuick 2.9
import QtQuick.Window 2.2
import Material 0.3
import Material.ListItems 0.1

ApplicationWindow {
    id: rootwindow
    visible: true
    width: 640
    height: 320
    minimumWidth: 640
    minimumHeight: 320
    maximumWidth: 640
    maximumHeight: 320
    title: qsTr("Fishield")

    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "white"
    }


    initialPage: Page {
        title: "Welcode"
        id: loginwindow
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
                enabled: !loginwindow.loading

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
                enabled: !loginwindow.loading
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
                enabled: !loginwindow.loading

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
                        loginwindow.loading = true;
                        loginwindow.timeout = 5;
                        countDowm.start();
                        login_backend.login(username.text, password.text);
                    }
                }
            }

            ProgressCircle {
                anchors.centerIn: parent
                visible: loginwindow.loading
                color: "blue"
            }

            Timer {
                id:countDowm;
                repeat: true;
                interval: 1000;
                onTriggered: {
                    loginwindow.timeout--;
                    if (loginwindow.timeout < 0) {
                        loginwindow.loading = false
                        timeoutlabel.visible = true
                        loginwindow.timeout = 5
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

            Connections
            {
                target: login_backend
                onLogined:
                {
                    loginwindow.loading = false;
                    console.debug(loginwindow.username ,"logined");
                    pageStack.push(Qt.resolvedUrl("MainWindow.qml"))
                }
            }

        }


    }


}
