// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.2

Item {
    width: 400
    height: 300
    Rectangle {
        x:0
        y:0
        width: 400
        height: 300
        color: "grey"
        Text {
            x:10
            y:190
            color: "white"
            text: "TPS:"
        }

        Text {
            id: tpstext
            x:50
            y:190
            color: "white"
        }
        Rectangle {
            x: 10
            y: 10
            radius:5
            width:380
            height:120
            color: "white"
            Rectangle {
                x:5
                y:5
                width:370
                height:110
                radius:5
                color: "grey"
                Text {
                    x:5
                    y:5
                    width:360
                    height:100
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "Please ensure your throttle is completly closed, then press \"Set Minimum\". Once this is complete, ensure your throttle is completly open, and click \"Set Maximum\". This will calibrate the throttle min and max values for you";
                }
            }
        }


        Rectangle {
            x:10
            y:130
            width:200
            height:30
            radius:5
            color:"white"
            Rectangle {
                x:5
                y:5
                width:190
                height:20
                radius:5
                color: "blue"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "Capture Minimum"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Min Clicked");
                            tpsMinText.text = tpstext.text
                        }
                    }
                }
            }
        }
        Rectangle {
            x:210
            y:130
            width:180
            height:30
            radius: 5
            color:"white"
            Rectangle {
                x:5
                y:5
                width:170
                height:20
                radius:5
                color:"grey"
                Text {
                    x:5
                    y:3
                    width:80
                    height:13
                    id: tpsMinText
                }
            }
        }
        Rectangle {
            x:10
            y:160
            width:200
            height:30
            radius:5
            color:"white"
            Rectangle {
                x:5
                y:5
                width:190
                height:20
                radius:5
                color: "blue"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "Capture Maximum"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Max Clicked");
                            tpsMaxText.text = tpstext.text
                        }
                    }
                }
            }
        }
        Rectangle {
            x:210
            y:160
            width:180
            height:30
            radius: 5
            color:"white"
            Rectangle {
                x:5
                y:5
                width:170
                height:20
                radius:5
                color:"grey"
                Text {
                    id: tpsMaxText
                    x:5
                    y:3
                    width:80
                    height:13
                }
            }
        }
        Rectangle {
            x:10
            y:230
            width:200
            height:30
            radius:5
            color:"white"
            Rectangle {
                x:5
                y:5
                width:190
                height:20
                radius:5
                color: "blue"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "Write Values"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Capture Clicked");
                            var min = parseFloat(tpsMinText.text);
                            var max = parseFloat(tpsMaxText.text);
                            var minADC = (min / 100.0) * 1024.0;
                            var maxADC = (max / 100.0) * 1024.0;
                            var memory = [((minADC << 8) & 0xFF),(minADC & 0xFF),((maxADC << 8) & 0xFF),(maxADC & 0xFF)];
                            decoder.setMemory(49186,16,4,memory);

                        }
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        decoder.payloadDecoded.connect(decode);
    }
    function decode(map){
    //console.log("Decoded");
        tpstext.text = map["TPS"];
    }

}
