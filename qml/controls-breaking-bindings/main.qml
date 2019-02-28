import QtQuick 2.5
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4 as QQC1
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
    property int checkState: Qt.Checked
    property bool radioChecked: true
    property int sliderValue: 50
    property int comboCurrentIndex: 1

    TextInput {
        id: textInput
        text: someString
    }


    CheckBox { // works
        y: 50
        checkState: root.checkState
    }

    RadioButton { // works
        y: 50
        x: 100
        checked: root.radioChecked
    }


    QQC1.CheckBox { // breaks binding!
        y: 100
        checkedState: root.checkState
    }

    QQC1.RadioButton { // breaks binding!
        x: 100
        y: 100
        checked: root.radioChecked
    }

    Slider { // works
        y: 200
        from: 0
        to: 100
        value: root.sliderValue
    }

    QQC1.Slider { // works
        y: 250
        minimumValue: 0
        maximumValue: 100
        value: root.sliderValue
    }

    ComboBox { // works
        y: 300
        editable: true
        model: ListModel {
            id: model
            ListElement { text: "Banana" }
            ListElement { text: "Apple" }
            ListElement { text: "Coconut" }
        }
        currentIndex: root.comboCurrentIndex

    }


    QQC1.ComboBox { // works
        y: 350
        width: 200
        model: [ "Banana", "Apple", "Coconut" ]
        currentIndex: root.comboCurrentIndex
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
                root.checkState = !root.checkState;
                root.radioChecked = !root.radioChecked;
                root.sliderValue = count;
                root.comboCurrentIndex = 0
            }
        }
    }
}
