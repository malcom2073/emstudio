// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.2

Rectangle {
    width: 840
    height: 800
    color: "#D4D0C8"
    Component.onCompleted: {
        decoder.payloadDecoded.connect(decode);
        decoderEngineOffset.update.connect(offsetupdate);
    }
    function decode(map) {
        advancetext.text = map["Advance"].toFixed(2);
    }
    function offsetupdate() {
        offsettext.text = decoderEngineOffset.value().toFixed(2);
    }
    Rectangle {
        x:200
        y:0
        width:440
        height:200
        color:"black"
        Text {
            id: advancetext
            anchors.fill: parent
            clip: true
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignTop
            color:"white"
            font.pixelSize: 150
            text: ""
        }
        Text {
            x:0
            y:180
            width:400
            height:20
            horizontalAlignment: Text.AlignHCenter
            color:"white"
            text: "Ignition Advance (degrees)"
        }
    }


    Rectangle {
        x:200
        y:205
        width:440
        height:200
        color:"black"
        Text {
            id: offsettext
            anchors.fill: parent
            clip: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
            color:"white"
            font.pixelSize: 100
            text: ""
        }
        Text {
            x:0
            y:180
            width:440
            height:20
            horizontalAlignment: Text.AlignHCenter
            color:"white"
            text: "Current Offset (degrees)"
        }
    }
    Rectangle {
        x:0
        y:205
        width:100
        height:200
        color:"black"
        Rectangle {
            x:2
            y:2
            width:96
            height:196
            color:"Red"
            Text {
                color:"black"
                anchors.fill: parent
                font.pixelSize: 50
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "-10"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        decoderEngineOffset.setValue((parseFloat(offsettext.text) - 10.0));
                    }
                }
            }
        }
    }
    Rectangle {
        x:100
        y:205
        width:100
        height:200
        color:"black"
        Rectangle {
            x:2
            y:2
            width:96
            height:196
            color:"Red"
            Text {
                color:"black"
                anchors.fill: parent
                font.pixelSize: 50
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "-1"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        decoderEngineOffset.setValue((parseFloat(offsettext.text) - 1.0));
                    }
                }
            }
        }
    }


    Rectangle {
        x:740
        y:205
        width:100
        height:200
        color:"black"
        Rectangle {
            x:2
            y:2
            width:96
            height:196
            color:"green"
            Text {
                color:"black"
                anchors.fill: parent
                font.pixelSize: 50
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "+10"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        decoderEngineOffset.setValue((parseFloat(offsettext.text) + 10.0));
                    }
                }
            }
        }
    }
    Rectangle {
        x:640
        y:205
        width:100
        height:200
        color:"black"
        Rectangle {
            x:2
            y:2
            width:96
            height:196
            color:"green"
            Text {
                color:"black"
                anchors.fill: parent
                font.pixelSize: 50
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "+1"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        decoderEngineOffset.setValue((parseFloat(offsettext.text) + 1.0));
                    }
                }
            }
        }
    }
    Rectangle {
        x:0
        y:400
        width:840
        height:400
        color:"black"
        Rectangle {
            x:1
            y:1
            width: 838
            height:398
            color: "#D4D0C8"
            Text {
                color:"black"
                anchors.fill: parent
                font.pixelSize: 20
                wrapMode: Text.Wrap

                text: "With FreeEMS to make the timing more advanced you increase the offset and to make it more retarded you decrease it.\n\
The offset must be less than the decoder total angle (360 for a crank mounted missing tooth setup), you will get no outputs if it is larger.\n\
If the position that you know is really zero is 60 degrees away from the position of the first tooth post the gap, then your offset is one of two\
values:\n\
60 or 300 (360 - 60)\n\
This depends upon whether it is before or after.\n\

Make the offset number larger to advance the base timing, make it smaller to retard the base timing. For example, if you have 10* BTDC in your\
 table, flat, and a timing light shows 5btdc on the engine, then increase this number by 5 degrees. The same applies statically, except you have\
 to think more.This is where some descriptive text goes, on how to configure this screen."
            }

        }

    }
}
