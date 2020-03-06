// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.2

Item {
    width: 500
    height: 400
    Rectangle {
        x:0
        y:0
        width: 500
        height: 400
        color: "grey"
        Text {
            x:10
            y:130
            color: "white"
            text: "Current Decoder Offset:"
        }
        TextEdit {
            id: tpstext
            x:200
            y:130
            color: "white"
        }
	Text {
		x:10
		y:150
		color: "white"
		text: "Configured Output Events"
	}
	TextEdit {
		id: configoutputeventstext
		x:200
		y:150
		color: "white"
	}
    Text {
        x:10
        y:170
        color: "white"
        text: "injections Per Cyle (Per Injector)"
    }
    TextEdit {
        id: configinjectionspercycle
        x:250
        y:170
        color: "white"
    }
    Text {
        x:10
        y:190
        color: "white"
        text: "Angles of TDC"
    }
    TextEdit {
        id: configanglesoftdc
        x:250
        y:190
        color: "white"
    }
    Text {
        x:10
        y:210
        color: "white"
        text: "Output Pin Numbers"
    }
    TextEdit {
        id: configoutputpinnumbers
        x:250
        y:210
        color: "white"
    }
    Text {
        x:10
        y:230
        color: "white"
        text: "Scheduling Bits"
    }
    TextEdit {
        id: configschedulingbits
        x:250
        y:230
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
                    text: "These are the scheduler settings for your engine. Current values are displayed, enter new values and then click Write Values to save them to flash."
                }
            }
        }

        Rectangle {
            x:10
            y:260
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
                            numberOfConfiguredOutputEvents.setValue(configoutputeventstext.text);
                            numberOfInjectionsPerEngineCycle.setValue(configinjectionspercycle.text);
                            decoderEngineOffset.setValue(tpstext.text);
                            var retval = new Array();
                            var split = configanglesoftdc.text.split(",");
                            for (var i = 0; i < numberOfConfiguredOutputEvents.value(); i++) {
                                retval[i] = parseFloat(split[i]);
                            }
                            anglesOfTDC.setValue(retval);

                            var retval1 = new Array();
                            var split1 = configoutputpinnumbers.text.split(",");
                            for (var i = 0; i < numberOfConfiguredOutputEvents.value(); i++) {
                                retval1[i] = parseFloat(split1[i]);
                            }
                            outputEventPinNumbers.setValue(retval1);

                            var retval2 = new Array();
                            var split2 = configschedulingbits.text.split(",");
                            for (var i = 0; i < numberOfConfiguredOutputEvents.value(); i++) {
                                retval2[i] = parseFloat(split2[i]);
                            }
                            schedulingConfigurationBits.setValue(retval2);
                        }
                    }
                }
            }
        }
        Component.onCompleted: {
            decoder.payloadDecoded.connect(decode);
            decoderEngineOffset.update.connect(offsetupdate);
            numberOfConfiguredOutputEvents.update.connect(outputeventupdate);
            numberOfInjectionsPerEngineCycle.update.connect(injectionspercycleupdate);
            anglesOfTDC.update.connect(anglesoftdcupdate);
            outputEventPinNumbers.update.connect(outputeventpinsupdate);
            schedulingConfigurationBits.update.connect(configschedulingbitsupdate);


            anglesoftdcupdate();

            tpstext.text = decoderEngineOffset.value();
            configoutputeventstext.text = numberOfConfiguredOutputEvents.value();
            configinjectionspercycle.text = numberOfInjectionsPerEngineCycle.value();

        }
        function offsetupdate() {
        console.log("Offset update");
            tpstext.text = decoderEngineOffset.value();
        }
        function outputeventupdate() {
            configoutputeventstext.text = numberOfConfiguredOutputEvents.value();
        }
        function injectionspercycleupdate() {
            configinjectionspercycle.text = numberOfInjectionsPerEngineCycle.value();
        }
        function decode(map){
        //console.log("Decoded");
    //        tpstext.text = map["TPS"];
        }
        function anglesoftdcupdate() {
            //anglesOfTDC.value()  - An array of doubles.
            var mymap = anglesOfTDC.value();
            console.log("Elements:" + anglesOfTDC.elements());

            configanglesoftdc.text = "";
            for (var i = 0; i < numberOfConfiguredOutputEvents.value(); ++i) {
                if (i == numberOfConfiguredOutputEvents.value()-1)
                {
                    configanglesoftdc.text += mymap[i].toString();
                }
                else
                {
                    configanglesoftdc.text += mymap[i].toString() + ",";
                }

                console.log("Value:" + mymap[i].toString());
            }
        }
        function outputeventpinsupdate() {
            var mymap = outputEventPinNumbers.value();
            configoutputpinnumbers.text = "";
            for (var i = 0; i < numberOfConfiguredOutputEvents.value(); i++) {
                if (i == numberOfConfiguredOutputEvents.value()-1)
                {
                    configoutputpinnumbers.text += mymap[i].toString();
                }
                else
                {
                    configoutputpinnumbers.text += mymap[i].toString() + ",";
                }
            }

        }
        function configschedulingbitsupdate() {
            var mymap = schedulingConfigurationBits.value();
            configschedulingbits.text = "";
            for (var i = 0; i < numberOfConfiguredOutputEvents.value(); i++) {
                if (i == numberOfConfiguredOutputEvents.value()-1)
                {
                    configschedulingbits.text += mymap[i].toString();
                }
                else
                {
                    configschedulingbits.text += mymap[i].toString() + ",";
                }
            }
        }
    }
}
