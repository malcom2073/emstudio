// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.2

Item {
    width: 600
    height: 400
    Rectangle {
        x:0
        y:0
        width: 600
        height: 400
        color: "grey"
        Text {
            x:10
            y:130
            width:200
            color: "white"
            text: "Volume per cylinder (In CCs):"
            horizontalAlignment: Text.AlignRight
        }
        TextEdit {
            id: configcylindervolume
            x:220
            y:130
            width:100
            color: "white"
            text: "Test"
        }
        Text {
            x:10
            y:150
            width:200
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "Injector size (In CC/Minute):"
        }
        TextEdit {
            id: configinjectorsize
            x:220
            y:150
            width:100
            color: "white"
        }
        Text {
            x:10
            y:170
            width:200
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "RPM Fuel Cutoff:"
        }
        TextEdit {
            id: configrpmfuelcutoff
            x:220
            y:170
            width:100
            color: "white"
        }
        Text {
            x:10
            y:190
            width:200
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "RPM Fuel Reenable:"
        }
        TextEdit {
            id: configrpmfuelreenable
            x:220
            y:190
            width:100
            color: "white"
        }
        Text {
            x:10
            y:210
            width:200
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "RPM Ignition Cutoff:"
        }
        TextEdit {
            id: configrpmtimingcutoff
            x:220
            y:210
            width:100
            color: "white"
        }
        Text {
            x:10
            y:230
            width:200
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "RPM Ignition Reenable:"
        }
        TextEdit {
            id: configrpmtimingreenable
            x:220
            y:230
            width:100
            color: "white"
        }
        Text {
            x:340
            y:130
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "MAP Minimum:"
        }
        TextEdit {
            id: configmapminimum
            x:500
            y:130
            width:100
            color: "white"
            text: "testtext"
        }
        Text {
            x:340
            y:150
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "MAP Range:"
        }
        TextEdit {
            id: configmaprange
            x:500
            y:150
            width:100
            color: "white"
        }





        Text {
            x:340
            y:170
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "AAP Minimum:"
        }
        TextEdit {
            id: configaapminimum
            x:500
            y:170
            width:100
            color: "white"
        }


        Text {
            x:340
            y:190
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "AAP Range:"
        }
        TextEdit {
            id: configaaprange
            x:500
            y:190
            width:100
            color: "white"
        }


        Text {
            x:340
            y:210
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "EGO Minimum:"
        }
        TextEdit {
            id: configegominimum
            x:500
            y:210
            width:100
            color: "white"
        }


        Text {
            x:340
            y:230
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "EGO Range:"
        }
        TextEdit {
            id: configegorange
            x:500
            y:230
            width:100
            color: "white"
        }

        Text {
            x:340
            y:250
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "BRV Minimum:"
        }
        TextEdit {
            id: configbrvminimum
            x:500
            y:250
            width:100
            color: "white"
        }


        Text {
            x:340
            y:270
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "BRV Range:"
        }
        TextEdit {
            id: configbrvrange
            x:500
            y:270
            width:100
            color: "white"
        }


        Text {
            x:340
            y:290
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "TPS Minimum ADC:"
        }
        TextEdit {
            id: configtpsminimum
            x:500
            y:290
            width:100
            color: "white"
        }


        Text {
            x:340
            y:310
            width:150
            horizontalAlignment: Text.AlignRight
            color: "white"
            text: "TPS Maximum ADC:"
        }
        TextEdit {
            id: configtpsrange
            x:500
            y:310
            width:100
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
            y:310
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
            //decoder.payloadDecoded.connect(decode);
            //decoderEngineOffset.update.connect(offsetupdate);
            //numberOfConfiguredOutputEvents.update.connect(outputeventupdate);
            //numberOfInjectionsPerEngineCycle.update.connect(injectionspercycleupdate);
            //anglesOfTDC.update.connect(anglesoftdcupdate);
            //outputEventPinNumbers.update.connect(outputeventpinsupdate);
            //schedulingConfigurationBits.update.connect(configschedulingbitsupdate);


            //anglesoftdcupdate();
            configcylindervolume.text = perCylinderVolume.value();
            configinjectorsize.text = injectorFlow.value();
            configrpmfuelcutoff.text = InjectionRPMDisableThreshold.value();
            configrpmfuelreenable.text = InjectionRPMEnableThreshold.value();

            perCylinderVolume.update.connect(percylindervolumeupdate);
            injectorFlow.update.connect(injectorflowupdate);
            InjectionRPMDisableThreshold.update.connect(injectionrpmdisableupdate);
            InjectionRPMEnableThreshold.update.connect(injectionrpmreenableupdate);

            //tpstext.text = decoderEngineOffset.value();
            //configoutputeventstext.text = numberOfConfiguredOutputEvents.value();
            //configinjectionspercycle.text = numberOfInjectionsPerEngineCycle.value();

        }
        function percylindervolumeupdate() {
            configcylindervolume.text = perCylinderVolume.value();
        }
        function injectorflowupdate() {
            configinjectorsize.text = injectorFlow.value();
        }
        function injectionrpmdisableupdate() {
            configrpmfuelcutoff.text = InjectionRPMDisableThreshold.value();
        }
        function injectionrpmreenableupdate() {
            configrpmfuelreenable.text = InjectionRPMEnableThreshold.value();
        }

        function offsetupdate() {
        console.log("Offset update");
            //tpstext.text = decoderEngineOffset.value();
        }
        function outputeventupdate() {
            //configoutputeventstext.text = numberOfConfiguredOutputEvents.value();
        }
        function injectionspercycleupdate() {
            //configinjectionspercycle.text = numberOfInjectionsPerEngineCycle.value();
        }
        function decode(map){
        //console.log("Decoded");
    //        tpstext.text = map["TPS"];
        }
    }
}
