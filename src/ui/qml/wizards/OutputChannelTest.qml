// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.3

Item {
    width:800
    height:600
    function benchTestReplyFunc(remaining,current) {
        currentEvent.text = current;
        eventsRemaining.text = remaining;
    }
    Rectangle {
        x:0
        y:0
        width: 800
        height: 600
        color: "grey"

        Rectangle {
            id: inforect
            x: 0
            y: 0
            radius:5
            width:800
            height:250
            color: "white"
            Rectangle {
                x:5
                y:5
                width:790
                height:240
                radius:5
                color: "grey"
                Text {
                    x:5
                    y:5
                    width:780
                    height:230
                    color:"white"
                    wrapMode: Text.WordWrap
                    font.pointSize: 20
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "Informative text on what to do, TBD by Sean Keys"
                }
            }
        }

        Row
        {
            id: inputlayout
            anchors.top: inforect.bottom
            width: 800
            //height:350
            spacing: 50
            Row
            {
                spacing: 20
                Column {
                    spacing: 5
                    Text {
                     //   x:10
                //           y:240

                        width:300
                        height:40
                        color: "white"
                        font.pointSize: 20
                        text: "Delay Ticks (1-2.4billion)"
                    }
                    Text {
                        //x:370
                        //y:240
                        width:100
                        height:40
                        color: "white"
                        font.pointSize: 20
                        text: "Runtime Ticks (0-65535)"
                    }
                    Text {
                        //x:10
                        //y:280
                        width:100
                        height:40
                        color: "white"
                        font.pointSize: 20
                        text: "Test Interval (0-65535)"
                    }
                    Text {
                        //x:10
                        //y:340
                        width:100
                        height:40
                        color: "white"
                        font.pointSize: 20
                        text: "Channel (0-24):"
                    }
                    Text {
                        //x:10
                        //y:340
                        width:100
                        height:40
                        color: "white"
                        font.pointSize: 20
                        text: "Number of events (0-255)"
                    }

                }
                Column {
                    spacing: 5
                    Rectangle {
                       // x:250
                       // y:240
                        width:100
                        height:40
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:36
                            color:"grey"
                            TextEdit {
                                id: delaytext
                                anchors.fill:parent
                                color: "white"
                                font.pointSize: 20
                                text:"1000"
                            }
                        }
                    }
                    Rectangle {
                        //x:600
                        //y:240
                        width:100
                        height:40
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:36
                            color:"grey"
                            TextEdit {
                                id: runtimetext
                                anchors.fill:parent
                                color: "white"
                                font.pointSize: 20
                                text:"1000"
                            }
                        }
                    }
                    Rectangle {
                        //x:250
                        //y:280
                        width:100
                        height:40
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:36
                            color:"grey"
                            TextEdit {
                                id: testintervaltext
                                anchors.fill:parent
                                color: "white"
                                font.pointSize: 20
                                text:"1000"
                            }
                        }
                    }
                    Rectangle {
                        //x:250
                        //y:340
                        width:100
                        height:40
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:36
                            color:"grey"
                            TextEdit {
                                id: channeltext
                                anchors.fill:parent
                                color: "white"
                                font.pointSize: 20
                                text:"10"
                            }
                        }
                    }

                    Rectangle {
                        //x:250
                        //y:370
                        width:100
                        height:40
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:36
                            color:"grey"
                            TextEdit {
                                id: numbereventstext
                                anchors.fill:parent
                                color: "white"
                                font.pointSize: 20
                                text:"100"
                            }
                        }
                    }


                } //Column

            } //Row (First Column)
        } //Column
        Row {
            anchors.top: inputlayout.bottom
            spacing: 5
            Rectangle {
                //x:10
                //y:550
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
                        font.pointSize: 20
                        color: "white"
                        text: "Start"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("Capture Clicked");
                                var valueArray = new Array()
                                valueArray[0] = 0x8892;
                                valueArray[1] = parseInt(delaytext.text) //Delay ticks 32bit
                                valueArray[2] = parseInt(runtimetext.text) //Runtime ticks 16bits
                                valueArray[3] = parseInt(testintervaltext.text) //test interval 16bits
                                valueArray[4] = parseInt(channeltext.text) //Channel, 0-24
                                valueArray[5] = parseInt(numbereventstext.text) //Number of events, 8bits
                                var sizeArray = new Array()
                                sizeArray[0] = 2
                                sizeArray[1] = 4;
                                sizeArray[2] = 2;
                                sizeArray[3] = 2;
                                sizeArray[4] = 1;
                                sizeArray[5] = 1;
                                emscomms.sendCustomPacket(valueArray,sizeArray);
                            }
                        }
                    }
                }
            }



        } //Row (bottom button bar)
    } //Rectangle
} //Item
