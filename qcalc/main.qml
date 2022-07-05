import QtQuick 2.12
import QtQuick.Window 2.12
import Qt.labs.settings 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr ("QML Calculator")

    function updateResultArea (msg) {
        resultText.text = msg.toString ();
    }

    function updateRequestQueueSize (msg) {
        requestQueueSizeField.text = "Requests: " + msg.toString ();
    }

    function updateResponceQueueSize (msg) {
        responceQueueSizeField.text = "Responces: " + msg.toString ();
    }

    function appendErrorMessage (msg) {
        consoleTextEdit.text += "<font color=\"red\">" + msg + "</fond><br>";
    }
    function appendRequestMessage (msg) {
        consoleTextEdit.text += "<font color=\"green\">" + msg + "</fond><br>";
    }
    function appendResponceMessage (msg) {
        consoleTextEdit.text += "<font color=\"blue\">" + msg + "</fond><br>";
    }

    Settings {
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    Rectangle {
        id: resultArea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height * 2 / 10
        border.color: "white"
        border.width: 1
        color: "#DED9D4"
        Text {
            id: resultText
            anchors.fill: parent
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.pixelSize: parent.height * 2 / 3

            fontSizeMode: Text.Fit
            minimumPixelSize: 10
        }
    }

    Rectangle {
        id: timeoutArea
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width / 3
        height: 46

        border.color: "white"
        border.width: 1

        TextField {
            id: timeoutTextField
            placeholderText: "Timeout in seconds (0 by default)"
            cursorVisible: false
            font.pixelSize: parent.height * 2 / 3

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            validator: IntValidator {bottom: 11}
            background: Rectangle {color: "lightGray"}
        }
    }

    Rectangle {
        id: queueSizeArea
        anchors.left: parent.left
        anchors.top: timeoutArea.bottom
        width: parent.width / 3
        height: 46

        border.color: "white"
        border.width: 1

        TextField {
            id: requestQueueSizeField
            text: "Requests: 0"
            readOnly: true
            cursorVisible: false
            font.pixelSize: parent.height * 2 / 3

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width / 2

            validator: IntValidator {bottom: 11}
            background: Rectangle {color: "lightGray"}
        }

        TextField {
            id: responceQueueSizeField
            text: "Responces: 0"
            readOnly: true
            cursorVisible: false
            font.pixelSize: parent.height * 2 / 3

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width / 2

            validator: IntValidator {bottom: 11}
            background: Rectangle {color: "lightGray"}
        }
    }

    Rectangle {
        id: consoleArea
        width: parent.width / 3
        anchors.left: parent.left
        anchors.top: queueSizeArea.bottom
        height: parent.height - timeoutArea.height - queueSizeArea.height
        border.color: "white"
        border.width: 1
        color: "#ECE5C7"

        Text {
            id: consoleTextEdit
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            textFormat: TextEdit.RichText
            font.pixelSize: parent.height / 50
            wrapMode: Text.WordWrap
        }
    }

    Rectangle {
        id: buttonsArea
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height * 8 / 10
        width: parent.width * 2 / 3
        border.color: "white"
        border.width: 1

        Button {
            id: addButton
            x: 0
            y: 0
            width: parent.width / 4
            height: parent.height / 5
            color: "#FDCF73"
            text: "+"
            onClicked: qtcalculator.slotAddSymbol ('+')
        }

        Button {
            id: subButton
            x: parent.width / 4
            y: 0
            width: parent.width / 4
            height: parent.height / 5
            color: "#FDCF73"
            text: "-"
            onClicked: qtcalculator.slotAddSymbol ('-')
        }

        Button {
            id: multButton
            x: parent.width * 2 / 4
            y: 0
            width: parent.width / 4
            height: parent.height / 5
            color: "#FDCF73"
            text: "*"
            onClicked: qtcalculator.slotAddSymbol ('*')
        }

        Button {
            id: divButton
            x: parent.width * 3 / 4
            y: 0
            width: parent.width / 4
            height: parent.height / 5
            color: "#FDCF73"
            text: "/"
            onClicked: qtcalculator.slotAddSymbol ('/')
        }

        Button {
            id: n7Button
            x: 0
            y: parent.height / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "7"
            onClicked: qtcalculator.slotAddSymbol ('7')
        }

        Button {
            id: n8Button
            x: parent.width / 4
            y: parent.height / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "8"
            onClicked: qtcalculator.slotAddSymbol ('8')
        }

        Button {
            id: n9Button
            x: parent.width * 2 / 4
            y: parent.height / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "9"
            onClicked: qtcalculator.slotAddSymbol ('9')
        }

        Button {
            id: n4Button
            x: 0
            y: parent.height * 2 / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "4"
            onClicked: qtcalculator.slotAddSymbol ('4')
        }

        Button {
            id: n5Button
            x: parent.width / 4
            y: parent.height * 2 / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "5"
            onClicked: qtcalculator.slotAddSymbol ('5')
        }

        Button {
            id: n6Button
            x: parent.width * 2 / 4
            y: parent.height * 2 / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "6"
            onClicked: qtcalculator.slotAddSymbol ('6')
        }

        Button {
            id: n1Button
            x: 0
            y: parent.height * 3 / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "1"
            onClicked: qtcalculator.slotAddSymbol ('1')
        }

        Button {
            id: n2Button
            x: parent.width / 4
            y: parent.height * 3 / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "2"
            onClicked: qtcalculator.slotAddSymbol ('2')
        }

        Button {
            id: n3Button
            x: parent.width * 2 / 4
            y: parent.height * 3 / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "3"
            onClicked: qtcalculator.slotAddSymbol ('3')
        }

        Button {
            id: n0Button
            x: 0
            y: parent.height * 4 / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "0"
            onClicked: qtcalculator.slotAddSymbol ('0')
        }

        Button {
            id: npButton
            x: parent.width / 4
            y: parent.height * 4 / 5
            width: parent.width / 4
            height: parent.height / 5
            //color: pressed ? "#e0b91c" : "#face20"
            color: "#9B878B"
            text: "."
            onClicked: qtcalculator.slotAddSymbol ('.')
        }

        Button {
            id: ncButton
            x: parent.width * 2 / 4
            y: parent.height * 4 / 5
            width: parent.width / 4
            height: parent.height / 5
            color: "#9B878B"
            text: "C"
            onClicked: qtcalculator.slotClear ()
        }

        Button {
            id: resButton
            x: parent.width * 3 / 4
            y: parent.height / 5
            width: parent.width / 4
            height: parent.height * 4 / 5
            color: "#E88846"
            text: "="
            onClicked: qtcalculator.slotExecute (timeoutTextField.text)
        }
    }
}
