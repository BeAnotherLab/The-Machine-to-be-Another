loadedInterfaceName = "TheMachineToBeAnother";

interfaceOrientation = "portrait";

pages = [[

{
    "name": "refresh",
    "type": "Button",
    "bounds": [.6, .905, .2, .1],
    "startingValue": 0,
    "isLocal": true,
    "mode": "contact",
    "ontouchstart": "interfaceManager.refreshInterface()",
    "stroke": "#aaa",
    "label": "refrsh",
},
{
    "name": "tabButton",
    "type": "Button",
    "bounds": [.8, .905, .2, .1],
    "mode": "toggle",
    "stroke": "#aaa",
    "isLocal": true,
    "ontouchstart": "if(this.value == this.max) { control.showToolbar(); } else { control.hideToolbar(); }",
    "label": "menu",
},

{
    "name": "nowPlaying",
    "type": "Label",
    "x": -0.325, "y": .455,
    "width": 1, "height": 1,
    "value": "nothing is playing",
},

{
	"name":"button1",
	"type":"Button",
	"x":0,
	"y":0,
	"width":1,
	"height":.125,
	"mode": "momentary",
	"address":"/btn1"
},

{
	"name":"button2",
	"type":"Button",
	"x":0,
	"y":.130,
	"width":1,
	"height":.125,
	"mode": "momentary",
	"address":"/btn2"
},

{
	"name":"button3",
	"type":"Button",
	"x":0,
	"y":.260,
	"width":1,
	"height":.125,
	"mode": "momentary",
	"address":"/btn3"
},

{
	"name":"button4",
	"type":"Button",
	"x":0,
	"y":.390,
	"width":1,
	"height":.125,
	"mode": "momentary",
	"address":"/btn4"
},

{
	"name":"button5",
	"type":"Button",
	"x":0,
	"y":.520,
	"width":1,
	"height":.125,
	"mode": "momentary",
	"address":"/btn5"
},

{
	"name":"button6",
	"type":"Button",
	"x":0,
	"y":.650,
	"width":1,
	"height":.125,
	"mode": "momentary",
	"address":"/btn6"
},

{
	"name":"button7",
	"type":"Button",
	"x":0,
	"y":.780,
	"width":1,
	"height":.125,
	"mode": "momentary",
	"address":"/btn7"
},


]

];