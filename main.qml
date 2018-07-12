import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    id: rootwindow
    visible: true
    width: 640
    height: 320
    minimumWidth: 640
    minimumHeight: 320
    maximumWidth: 640
    maximumHeight: 320
    title: qsTr("Fishield")
    color: "lightgray"


    LoginWindow{
        id: loginwindow


        Connections
        {
            target: login_backend
            onLogined:
            {
                loginwindow.loading = false;
                console.debug(loginwindow.username ,"logined");
                var component = Qt.createComponent("qrc:/MainWindow.qml");
                if(component.status === Component.Ready){
                    var mainWindow = component.createObject(rootwindow);
                    if(mainWindow === null){
                        console.error("cannot create MainWindow Object");
                        return;
                    }
                    rootwindow.visible = false;
                }else
                    console.error("cannot create MainWindow Conponent");
            }
        }
    }

}
