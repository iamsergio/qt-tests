import QtQuick 2.5

/** Shows that TextInput "text" binding doesn't break
 * 1) Focus the text input and change the value
 * 2) Click the yellow rectangle and the text will change
  */
Item {
    id: root
    width: 1000
    height: 1000

    property int count: 0
    property string someString: "foo"

    TextInput {
        id: textInput
        text: someString
    }


    Rectangle {
        width: 50
        height: 50
        color: "yellow"
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.count++;
                root.someString = "bar" + count;
            }
        }
    }
}
