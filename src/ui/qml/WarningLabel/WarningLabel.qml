import QtQuick 2.2


Rectangle {
    id: warninglabel
	color:"red"
    property bool flash: false
    property string propertyMapProperty;
    property string conditiontype;
    property int conditionval;
    property double conditionvalue;
    property string message;
    Timer {
        id: flashertimer
        interval: 500
        repeat: true
        running: false
        onTriggered: {
            //console.log("Wee:" + conditionvalue);
            if (parent.flash)
            {
                parent.flash = false;
                parent.color = "orange";
            }
            else
            {
                parent.flash = true;
                parent.color = "red";
            }
        }
    }
    onConditionvalueChanged: {
        //console.log("CCalue:" + conditionvalue);
        if (conditiontype == "over")
        {
            //console.log("CValue:" + conditionvalue + conditionval);
            if (conditionvalue > conditionval)
            {
                flashertimer.running = true;
                warninglabel.visible = true;
            }
            else
            {
                flashertimer.running = false;
                warninglabel.visible = false;
            }
        }
    }
    Component.onCompleted:
    {
        warninglabel.visible = false;
    }

    Text {
        anchors.fill:parent
        horizontalAlignment: Text.AlignHCenter;
        verticalAlignment: Text.AlignVCenter;
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
        font.pointSize: 14
        text: message;
    }
}
