import QtQuick 2.2
import GaugeImage 0.1
import "WarningLabel"
Rectangle {
	x:0
	y:0
	width: 1280
	height: 900
	color: "black"
    property string plugincompat: "FREEEMS"

	GaugeImage {
		x:0
		y:0
		width:300
		height:300
		minimum:0
		maximum:6000
		numLabels:3
		startAngle:45
		endAngle:315
		text: "RPM"
		property string propertyMapProperty: "RPM"
		//Behavior on m_value {  PropertyAnimation { properties: "m_value"; duration: (propertyMap["RPM_DURATION"] ? propertyMap["RPM_DURATION"] : 50) } }
		m_value: (propertyMap["RPM"] ? propertyMap["RPM"] : 0)
	}
	GaugeImage {
		x:300
		y:0
		width:300
		height:300
		minimum:0
		maximum:40
		numLabels:3
		startAngle:45
		endAngle:315
		text: "Ign Timing"
		property string propertyMapProperty:"Advance"
		//Behavior on m_value {  PropertyAnimation { properties: "m_value"; duration: (propertyMap["Advance_DURATION"] ? propertyMap["Advance_DURATION"] : 50) } }
		m_value: (propertyMap["Advance"] ? propertyMap["Advance"] : 0)
	}
	GaugeImage {
		x:0
		y:300
		width:300
		height:300
		minimum:0
		maximum:50
		numLabels:5
		startAngle:45
		endAngle:315
		text: "Pulse Width"
		property string propertyMapProperty:"EffectivePW"
		//Behavior on m_value {  PropertyAnimation { properties: "m_value"; duration: (propertyMap["EffectivePW_DURATION"] ? propertyMap["EffectivePW_DURATION"] : 50) } }
		m_value: (propertyMap["EffectivePW"] ? propertyMap["EffectivePW"] : 0)
	}
	GaugeImage {
		x:300
		y:300
		width:300
		height:300
		minimum:-20
		maximum:120
		numLabels:7
		startAngle:45
		endAngle:315
		text: "Coolant"
		property string propertyMapProperty:"CHT"
		//Behavior on m_value {  PropertyAnimation { properties: "m_value"; duration: (propertyMap["CHT_DURATION"] ? propertyMap["CHT_DURATION"] : 50) } }
		m_value: (propertyMap["CHT"] ? propertyMap["CHT"] : 0)
	}
	GaugeImage {
		x:600
		y:0
		width:300
		height:300
		minimum:0
		maximum:2
		numLabels:2
		startAngle:45
		endAngle:315
		text: "O2"
		property string propertyMapProperty:"EGO"
		//Behavior on m_value {  PropertyAnimation { properties: "m_value"; duration: (propertyMap["EGO_DURATION"] ? propertyMap["EGO_DURATION"] : 50) } }
		m_value: (propertyMap["EGO"] ? propertyMap["EGO"] : 0)
	}
	GaugeImage {
		x:600
		y:300
		width:300
		height:300
		minimum:0
		maximum:250
		numLabels:5
		startAngle:45
		endAngle:315
		text: "MAP"
		property string propertyMapProperty:"MAP"
		//Behavior on m_value {  PropertyAnimation { properties: "m_value"; duration: (propertyMap["MAP_DURATION"] ? propertyMap["MAP_DURATION"] : 50) } }
		m_value: (propertyMap["MAP"] ? propertyMap["MAP"] : 0)
	}
	Rectangle {
		x:900
		y:0
		width:120
		height:60
		color:"grey"
		property string propertyMapProperty:"IAT"
		Rectangle {
			x:5
			y:5
			width:110
			height:50
			color:"black"
			Text {
				x:0
				y:35
				font.pixelSize:13
				color:"white"
				text: "Intake Air Temp (C)"
			}
			Text {
				x:10
				y:3
				font.pixelSize:30
				color:"white"
				text: Math.round(((propertyMap["IAT"] ? propertyMap["IAT"] : "0.0")*100))/100;
			}
		}
	}
	Rectangle {
		x:0
		y:600
		width:300
		height:60
		color:"grey"
		property string propertyMapProperty:"IAT"
		Rectangle {
			x:1
			y:1
			width:parent.width-2
			height:parent.height-2
			color:"black"
			Text {
				x:10
				y:0
				font.pixelSize:30
				color:"white"
				text: Math.round(((propertyMap["IAT"] ? propertyMap["IAT"] : "0.0")*100))/100;
			}
			Text {
				x:0
				y:35
				font.pixelSize:13
				color:"white"
				text: "Intake Air Temp (C)"
			}
			Rectangle {
				x:125
				y:(parent.height/2)-10
				width:parent.width-125
				height:20
				color:"grey"
				Rectangle {
					x:1
					y:1
					width:parent.width-2
					height:18
					color:"black"
				}
				Rectangle {
					x:1
					y:1
					width:(parent.width-125 * (propertyMap["IAT"] / 655.35))
					height:18
					color:(propertyMap["IAT"] < 370 ? "green" : propertyMap["IAT"] < 390 ? "yellow" : propertyMap["IAT"] < 410 ? "orange" : "red")
				}
			}
		}
	}
    Component.onCompleted: {

    }

    WarningLabel {
        id: warning;
        x: (parent.width / 2.0) - 200
        width:200
        y: (parent.height / 2.0) - 100
        height:100
        propertyMapProperty: "CHT";
        conditiontype: "over";
        conditionval: 215;
        conditionvalue: propertyMap["CHT"];
        message: "Warning!\nEngine is overheating!!!!!";
    }
}
