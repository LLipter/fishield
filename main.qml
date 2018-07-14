import QtQuick 2.9
import QtQuick.Window 2.2
import Material 0.3
import Material.ListItems 0.1

ApplicationWindow {
    id: rootwindow
    visible: true
    width: 800
    height: 480
    minimumHeight: 320
    minimumWidth: 260
    title: qsTr("Fishield")

    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "white"
    }


    initialPage: Page {
        title: "Welcode"
        id: loginpage

        property bool loading : false
        property int timeout: 5


        View{
            elevation: 2
            id: container
            width: 320
            height: 240
            anchors.centerIn: parent

            TextField{
                id: username
                anchors {
                    top: parent.top
                    topMargin: parent.height / 4
                    horizontalCenter: parent.horizontalCenter
                }

                placeholderText: "Username"
                floatingLabel: true
                enabled: !loginpage.loading

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
                enabled: !loginpage.loading
                echoMode: TextInput.Password

                onTextChanged: {
                    hasError = false;
                    helperText = ""
                }

            }

            Button {
                id: login
                text: "Login"
                elevation: 2
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: password.bottom
                    topMargin: 25
                }
                enabled: !loginpage.loading

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
                        loginpage.loading = true;
                        loginpage.timeout = 5;
                        countDowm.start();
                        login_backend.login(username.text, password.text);
                    }
                }
            }

            ProgressCircle {
                anchors.centerIn: parent
                visible: loginpage.loading
                color: "blue"
            }

            Timer {
                id:countDowm;
                repeat: true;
                interval: 1000;
                onTriggered: {
                    loginpage.timeout--;
                    if (loginpage.timeout < 0) {
                        loginpage.loading = false
                        timeoutlabel.visible = true
                        loginpage.timeout = 5
                        login_backend.timeout();
                    }
                }
            }

            Label {
                id: timeoutlabel
                text : "login timeout, please try again"
                visible: false
                color: "red"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: login.bottom
                    topMargin: 15
                }
            }

            Connections
            {
                target: login_backend
                onLogined:
                {
                    loginpage.loading = false;
                    // TODO : ILLEGAL PASSWORD OR NO SUCH USER
                    console.debug(username.text ,"logined");
                    pageStack.push(Qt.resolvedUrl("MainPage.qml"))
                }
            }

        }


    }


}
