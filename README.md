The-Machine-to-be-Another
=========================
Welcome to the the Machine to be Another software repo! It is made with openFrameworks and pure data.

The software allows you to run two different kind of setups. The gender swap (or two-way swap setup) requires two oculus rift and two usb cameras. In the two-way setup, both video feeds are fed to the other person's head-mounted display. The classic setup requires building your own open-hardware camera vest. The performer carries the vest so that the user can see through his perspective.

The communication between computers, between OpenFrameworks and PureData, and between a smartphone controller and OpenFrameworks, is achieved via Open Sound Control. OSC settings may be changed from the testApp.h.


Compiling the software
=========================
The project was tested with Openframeworks 0.8.4 on Windows 7&8 + VS Express 2012 and Maverick + XCode

1. Download the openframeworks 0.8.4 toolkit and unzip it
2. Using the git console execute```git clone https://github.com/BeAnotherLab/The-Machine-to-be-Another``` inside the ```openframeworks_folder/apps/myapps```. If you download the project as a zip file using the github web interface remove the "-master" extension from the name of the folder.
3. TODO: Download the required addons inside the folder ```openframeworks_folder/addons``` by running the clone_addons script or manually by.. TODO
4. Open the project from Visual Studio or XCode and compile.


Triggering sounds and TouchOSC control
=========================
You can load background music and as many audio samples in mp3 format as you wish with the machine to be another. in https://github.com/BeAnotherLab/The-Machine-to-be-Another/blob/master/src/ofApp.cpp setup() function, calling player.loadSounds("background_music sample_1 sample_2 sample_3"); loads the files named background_music.mp3, sample_1, sample_2, sample_3 and automatically assigns them numpad keys to trigger them. The music will automaticly loop and the next samples can be triggered by pressing keys 1-9 or sending a trigger on /btn*x* through TouchOSC.
TouchOSC layouts depend on your devices resolution but you can check in https://github.com/BeAnotherLab/The-Machine-to-be-Another/tree/master/files/TouchOSC%20layouts
for examples.
We also include files we used in different installations or performances made through the years.
You can also send /dim and /ht to respectively dim the display to black and back and reset the headtracking in case of drift (people must be looking straight for it to work)

Building the Vest
=========================
We designed a 3D printable vest that you can build yourself. The plastic parts are combined with off-the shelf components so that it is very easy and cheap to replicate. Instructions on how to build it are coming, but here is what you need to get 
- stl files printable by any modern 3D printer. 
https://github.com/BeAnotherLab/The-Machine-to-be-Another/tree/master/files/3D
- this wide FOV lens http://peauproductions.com/store/index.php?main_page=product_info&products_id=76
- this visible light filter http://peauproductions.com/store/index.php?main_page=product_info&products_id=44
- a PS3 camera that you open as follows http://www.instructables.com/id/The-EyeWriter/step6/Hack-the-PS3-Eye/ 
- remember to save the screws from the camera to build the arm
- 3 of these servos and corresponding connectors http://www.servodatabase.com/servo/springrc/sm-s2309s
- arduino uno
- some elastic fabric to sew on the harness pieces
- 4 AA batteries and a battery box
- small interruptor
- an LED
- print or make the circuit that connects to the arduino as a shield https://github.com/BeAnotherLab/The-Machine-to-be-Another/blob/master/files/arduino_shield.zip

we are working on an instructable with detailed fabrication instructions.

Servos control
========================
Servo control pure data patch is here https://github.com/BeAnotherLab/Servos-Control
run the patch and start the openFrameworks app. check the pitch, yaw, roll faders are moving when you move the oculus
plug-in your arduino with firmata servo control example loaded on it https://github.com/firmata/arduino/blob/master/examples/ServoFirmata/ServoFirmata.ino
and click on "devices" to see on with serial port your arduino is plugged in. Edit the box that says "open x" to replace x with the COMport and click it to activate it. Check the console for errors.


TODOs
========================	
If you wish to contribute, then please consider the following :

- Add servomotor control natively in OpenFrameworks. (at the moment it is done with pure data)
- Use ofxXmlSettings for settings such as experiment type, camera choice, Ip adresses etc.
- Create a GUI for the corresponding XmlSettings that allows to save them.
- Detailed documentation of Vest fabrication
- Map video on a sphere, and use quaternions instead of euler angles for orientation.
- DK2 display + headtrcking support.
- DK2 position tracking support.
- Camera abstraction to support different devices. (logitech C310, ovrvision, clduo, PS3eye, etc)
- Create debug view.
- Barrel Shader class to adapt to different lenses.
- Creating an abstraction for headtracking to support different kind of devices (GearVR, DK1, DK2, other headsets etc)
- Your idea here :)

Check the corresponding issues or create one to start exchanging! Thank you so much!
