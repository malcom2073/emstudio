// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.2

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
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "* Once you hit start, click the \"Stop Test\" button to stop the test, or bump to increase the duration.
* To determine how long your test will run, simply multiply the first three values together!
* To determine the duty cycle, divide the PW (if chosen explicitly) by the result of multiplying the \"Event Per \n\tCycle\" by \"Time Per Event\" together.
* Events To Fire equal to, or in excess of, \"Events Per Cycle\" are considered off.
* PW Source OR Value values have special meanings, 0 = OFF, 1 = RefPW, 2 = Dwell, higher = a literal PW.
* Time and PW literals are in 0.8us ticks, divide by 1250 for milliseconds.
* PW = Pulse Width
* Please select appropriate values for your coils and/or injectors, then click start.
* To use this to test outputs on a board with a multimeter or LED, rather than a high speed scope, set the PW high (at least 50,000), and the period high (65535) and number of events high (255)"
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

                        width:200
                        height:30
                        color: "white"
                        text: "Events Per Cycle (1-255):"
                    }
                    Text {
                        //x:370
                        //y:240
                        width:100
                        height:30
                        color: "white"
                        text: "Number Of Cycles (0-65535):"
                    }
                    Text {
                        //x:10
                        //y:280
                        width:100
                        height:30
                        color: "white"
                        text: "Ticks Per Event (0-65535):"
                    }
                    Text {
                        //x:10
                        //y:340
                        width:100
                        height:30
                        color: "white"
                        text: "Events to Fire From (0-255):"
                    }

                }
                Column {
                    spacing: 5
                    Rectangle {
                       // x:250
                       // y:240
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: eventspercycletext
                                anchors.fill:parent
                                color: "white"
                                text:"12"
                            }
                        }
                    }
                    Rectangle {
                        //x:600
                        //y:240
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: numberofcyclestext
                                anchors.fill:parent
                                color: "white"
                                text:"50"
                            }
                        }
                    }
                    Rectangle {
                        //x:250
                        //y:280
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: tickspereventtext
                                anchors.fill:parent
                                color: "white"
                                text:"10000"
                            }
                        }
                    }
                    Rectangle {
                        //x:250
                        //y:340
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: eventstofirefrom1
                                anchors.fill:parent
                                color: "white"
                                text:"0"
                            }
                        }
                    }

                    Rectangle {
                        //x:250
                        //y:370
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: eventstofirefrom2
                                anchors.fill:parent
                                color: "white"
                                text:"2"
                            }
                        }
                    }

                    Rectangle {
                        //x:250
                        //y:400
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: eventstofirefrom3
                                anchors.fill:parent
                                color: "white"
                                text:"4"
                            }
                        }
                    }

                    Rectangle {
                        //x:250
                        //y:430
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: eventstofirefrom4
                                anchors.fill:parent
                                color: "white"
                                text:"6"
                            }
                        }
                    }

                    Rectangle {
                        //x:250
                        //y:460
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: eventstofirefrom5
                                anchors.fill:parent
                                color: "white"
                                text:"8"
                            }
                        }
                    }

                    Rectangle {
                        //x:250
                        //y:490
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: eventstofirefrom6
                                anchors.fill:parent
                                color: "white"
                                text:"10"
                            }
                        }
                    }
                } //Column
                Column {
                    spacing: 5
                    Text {
                        //x:370
                        //y:290

                        width:200
                        height:30
                        color: "white"
                        text: "PW Source OR value (0-65535):"
                    }
                    Text {
                        //x:370
                        //y:290
                        width:100
                        height:30
                        color: "white"
                        text: "PW Source OR value (0-65535):"
                    }
                    Text {
                        //x:370
                        //y:290
                        width:100
                        height:30
                        color: "white"
                        text: "PW Source OR value (0-65535):"
                    }
                    Text {
                        //x:370
                        //y:290
                        width:100
                        height:30
                        color: "white"
                        text: "PW Source OR value (0-65535):"
                    }
                    Text {
                        //x:370
                        //y:290
                        width:100
                        height:30
                        color: "white"
                        text: "PW Source OR value (0-65535):"
                    }
                    Text {
                        //x:370
                        //y:290
                        width:100
                        height:30
                        color: "white"
                        text: "PW Source OR value (0-65535):"
                    }
                    Text {
                        //x:370
                        //y:290
                        width:100
                        height:30
                        color: "white"
                        text: "Events remaining"
                    }
                    Text {
                        //x:370
                        //y:290
                        width:100
                        height:30
                        color: "white"
                        text: "Current Event"
                    }
                }

                Column {
                    spacing: 5
                    Rectangle {
                        //x:600
                        //y:290
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: sourcetext1
                                anchors.fill:parent
                                color: "white"
                                text:"50000"
                            }
                        }
                    }
                    Rectangle {
                        //x:600
                        //y:320
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: sourcetext2
                                anchors.fill:parent
                                color: "white"
                                text:"50000"
                            }
                        }
                    }
                    Rectangle {
                        //x:600
                        //y:350
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: sourcetext3
                                anchors.fill:parent
                                color: "white"
                                text:"50000"
                            }
                        }
                    }
                    Rectangle {
                        //x:600
                        //y:380
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: sourcetext4
                                anchors.fill:parent
                                color: "white"
                                text:"50000"
                            }
                        }
                    }
                    Rectangle {
                        //x:600
                        //y:410
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: sourcetext5
                                anchors.fill:parent
                                color: "white"
                                text:"50000"
                            }
                        }
                    }
                    Rectangle {
                        //x:600
                        //y:440
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            TextEdit {
                                id: sourcetext6
                                anchors.fill:parent
                                color: "white"
                                text:"50000"
                            }
                        }
                    }
                    Rectangle {
                        width: 100
                        height: 30
                        color: "white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            Text {
                                id: eventsRemaining
                                anchors.fill:parent
                                color:"white"
                            }
                        }
                    }
                    Rectangle {
                        width:100
                        height:30
                        color:"white"
                        Rectangle {
                            x:2
                            y:2
                            width:96
                            height:26
                            color:"grey"
                            Text {
                                id:currentEvent
                                anchors.fill:parent
                                color:"white"
                            }
                        }
                    }




                } //Item (second column)
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
                        text: "Start"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("Capture Clicked");
                                var a2dArray = new Array(6)
                                a2dArray[0] = eventstofirefrom1.text;
                                a2dArray[1] = eventstofirefrom2.text;
                                a2dArray[2] = eventstofirefrom3.text;
                                a2dArray[3] = eventstofirefrom4.text;
                                a2dArray[4] = eventstofirefrom5.text;
                                a2dArray[5] = eventstofirefrom6.text;
                                var a2dArray2 = new Array(6)
                                a2dArray2[0] = sourcetext1.text;
                                a2dArray2[1] = sourcetext2.text;
                                a2dArray2[2] = sourcetext3.text;
                                a2dArray2[3] = sourcetext4.text;
                                a2dArray2[4] = sourcetext5.text;
                                a2dArray2[5] = sourcetext6.text;
                                emscomms.benchTestReply.connect(benchTestReplyFunc);
                                emscomms.startBenchTest(eventspercycletext.text,numberofcyclestext.text,tickspereventtext.text,a2dArray,a2dArray2);
                            }
                        }
                    }
                }
            }

            Rectangle {
                //x:210
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
                        text: "Stop"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                emscomms.stopBenchTest();
                            }
                        }
                    }
                }
            }
            Rectangle {
                //x:440
                //y:550
                width:100
                height:30
                radius:5
                color:"white"
                Rectangle {
                    x:5
                    y:5
                    width:90
                    height:20
                    radius:5
                    color: "blue"
                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "Bump"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (benchtestbumpvaltext.text > 255) {
                                    console.log("Bench Test Bump value too high, valid range from 1-255");
                                }
                                else {
                                    emscomms.bumpBenchTest(benchtestbumpvaltext.text);
                                }
                            }
                        }
                    }
                }
            }
            Rectangle {
                //x:560
                //y:550
                width:100
                height:30
                color:"white"
                Rectangle {
                    x:2
                    y:2
                    width:96
                    height:26
                    color:"grey"
                    TextEdit {
                        id: benchtestbumpvaltext
                        anchors.fill:parent
                        color: "white"
                        text:"50"
                    }
                }
            }
        } //Row (bottom button bar)
    } //Rectangle
} //Item
