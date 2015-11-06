The-Machine-to-be-Another
=========================
Welcome to the repository for The Machine to be Another! This is the part built with openFrameworks. To get the (soon to be deprecated) puredata component go [there](https://github.com/BeAnotherLab/Servos-Control)

The software allows you to run two different kind of setups. The gender swap (or two-way swap setup) requires two oculus rift and two USB cameras. In the two-way setup, both video feeds are fed to the other person's head-mounted display. The classic setup requires building your own open-hardware camera vest. The performer wears the vest so that the user can see from his perspective.

It can be controlled from the keyboard or through a phone or tablet running TouchOSC.
The software supports both DK1 and DK2, as well as PS3 cameras (our favorite), ovrvision cameras, and regular USB cameras.
Only Windows is supported, but the code should easily be ported to OS X.

![alt tag](https://github.com/BeAnotherLab/The-Machine-to-be-Another/blob/master/files/body%20swap%20rider.png?raw=true)
Two-way swap setup

Setting up the software
=========================
Here's an overview of the parameters necessary to setup the application. They should be set in the `settings.xml` file located in bin/data.
  - setup_type : 0 is one-way swap, 1 is two-way swap
  - IP : the IP address of the other computer to send the the headtracking data to.
  - computer_type : in the two way setup, helps differentiating the two computers running the software
  - camera_id : the id of the camera device you want to use. If you are using a laptop with an integrated webcam, its ID will   usually be 0, so that if you connect another USB camera you should use camera 1.
  - camera_type : depending on if you're using a regular USB camera or a ovrvision camera. using two USB cameras is not yet     supported  
  - ps3_position : allows you to choose between vertical or horizontal orientation of the camera
  - servo_roll : choose whether the roll angle of the point of view should be handled in software or in hardware (using the     rotation of a servo  
  - zoom : the size at which the video feed will be displayed. Use this to set realistic proportions for the point of view.     Can be set with the `O`and `P` keys.
  - IPD : the distance at which each eye will be displayed from each other. Tweak to set at a comfortable position. Might be    different for different people. Can be set with `←` and `→` keys.
  - alignment : the vertical alignment of each eye. When using stereo cameras, might be necessary to adjust for physical        misalignment of the lenses. Can be set via the `N` and `B`keys.
  - speed : parameter that links the difference between headtrackings to the place the video feed will be displayed. Can be     set via the `K` and `L`keys.
  - swapLR : swap left and right eyes if they were wrongly assignated.
  - sounds : here you should put the names of the sound files in mp3 format that you can load from the bin/data/sounds         folder. The first one is the background music that will be looped and sidechained behind the audio instructions. We have instructions in English, Chinese and French, as well as some recordings from previous performances that you are free to use.

Running the software
=========================
First you need to install the [oculus runtime 0.4.4](https://developer.oculus.com/downloads/pc/0.4.4-beta/Oculus_Runtime_for_Windows/)
and if you're using a PS3 camera, you'll need to buy the [CLeye PS3 driver](https://codelaboratories.com/downloads/)
for the classic setup, you need to install the [arduino driver](https://www.arduino.cc/en/guide/windows#toc4) if you don't have already


To run the software for the first time, rename the file `settingsexample` in bin/data to `settings.xml` then open it with a text editor.
Following is a description of the parameters necessary to run each setup.

For the classic (one-way swap) setup :
  - setup_type should be set to 0.
  - The other settings are optional but keep in mind you can choose the orientation of the camera and whether to use a roll servo depending on how you built the vest


For the gender swap (two-way swap) setup :
  - setup_type should be set to 1.
  - IP should be set to the IP of the other computer to send the headtracking data to.
  - computer_type should be set to a different value on each computer (1 or 2). Computer 1 is the one that will receive         TouchOSC messages and forward them to the other computer.
  - servo_roll should be set to 0.
  - The other settings are optional.

Before running the software, make sure the oculus is detected and if you're using a DK2, that it is set in extended display mode. Remember that you must rotate the DK2 display to be in landscape mode using the windows screen resolution menu.

Once you have set the parameters, run `The-Machine-to-be-Another.exe`. Use the console to check for error messages.
Two windows will show up. One is a debugging view, meant to be displayed on the computer screen, while the other should be dragged onto the Rift display. Press `F` to switch the selected window to fullscreen.
   
If you are running the classic setup, check the `Servos Control` part of this readme.
If you are running the body swap setup, you should be ready to go. Check that moving one Rift moves the display in opposite directions on the other computer. If not, you might need some troubleshooting.

Compiling the software
=========================
The project was tested with Openframeworks 0.7.4 on Windows 7&8 + Visual C++ 2010 Express.

1. Download openframeworks 0.7.4 and unzip it
2. Using the git console execute```git clone https://github.com/BeAnotherLab/The-Machine-to-be-Another``` inside the ```openframeworks_folder/apps/myapps```. If you download the project as a zip file using the github web interface remove the "-master" extension from the name of the folder.
3. Clone the modified [ofxFenster](https://github.com/ReallyRad/ofxFenster) and [ofxUI](https://github.com/ReallyRad/ofxUI) into the ```openframeworks_folder/addons``` folder. 
4. Open the project from Visual Studio and compile.


TouchOSC control
=========================
You can load background music and as many audio samples in mp3 format as you wish with the machine to be another. in `settings.xml` putting the file names in the `<sounds>` tag without their extension automatically assigns them numpad keys to trigger them. The music will automaticly loop and the next samples can be triggered by pressing keys 1-9 or sending a trigger on /btn*x* through TouchOSC.
TouchOSC layouts depend on your devices resolution but you can check in [this folder](https://github.com/BeAnotherLab/The-Machine-to-be-Another/tree/master/files/TouchOSC%20layouts)
for examples.
We also include files we used in different installations or performances made through the years.
You can also send /dimon /dimoff and /ht to respectively dim the display to black and back and reset the headtracking in case of drift (people must be looking straight for it to work). Also /driftPlus and /driftMinus allow you to nudge drift correction manually.

Building the Vest
=========================
We designed a 3D printable vest that you can build yourself. The plastic parts are combined with off-the shelf components so that it is very easy and cheap to replicate. Instructions on how to build it are coming, but here is what you need to get 
- stl files printable by any modern 3D printer. 
https://github.com/BeAnotherLab/The-Machine-to-be-Another/tree/master/files/3D
- this [wide FOV lens](http://peauproductions.com/store/index.php?main_page=product_info&products_id=76)
- this [visible light filter](http://peauproductions.com/store/index.php?main_page=product_info&products_id=44)
- an [M12 mount with those dimensions](http://www.rlx.sk/en/servo-motor/1498-analog-180-micro-servo-arduino-connector-tinkerkit.html) (we included a 3D-printable one in the repo but they usually come out with bad quality) 
- a PS3 camera that you open as follows http://www.instructables.com/id/The-EyeWriter/step6/Hack-the-PS3-Eye/ 
- remember to save the screws from the camera to build the arm
- [3 of these servos](http://www.servodatabase.com/servo/springrc/sm-s2309s) and corresponding connectors 
- arduino uno
- some elastic fabric to sew on the harness pieces
- 4 AA batteries and a battery box
- small interruptor
- an LED
- print or make the circuit that connects to the arduino as a [shield](https://github.com/BeAnotherLab/The-Machine-to-be-Another/blob/master/files/arduino_shield.zip)

Servos control
========================
Servo control pure data patch is [here](https://github.com/BeAnotherLab/Servos-Control)
run the patch and start the openFrameworks app. check the pitch, yaw, roll faders are moving when you move the oculus
plug-in your arduino with firmata servo control example loaded on it https://github.com/firmata/arduino/blob/master/examples/ServoFirmata/ServoFirmata.ino
and click on "devices" to see on with serial port your arduino is plugged in. Edit the box that says "open x" to replace x with the COMport and click it to activate it. Check the console for errors.


TODOs
========================	
If you wish to contribute, then please consider the following :

- porting to oF 0.9.0 rc and get rid of ofxFenster and ofxUI to use built-in addons.
- integrate [ofxOculusDK2](https://github.com/obviousjim/ofxOculusDK2) or update to 0.8.0 Oculus SDK
- Add servomotor control with OpenFrameworks. (at the moment it is done with pure data)
- Create a GUI for the manipulation of xml settings.
- Detailed documentation of Vest fabrication
- Map video on a sphere through a shader, and use quaternions instead of euler angles for orientation.
- DK2 position tracking support.
- Camera abstraction to support different devices. (logitech C310, ovrvision, clduo, PS3eye, etc)
- Barrel Shader class to adapt to different lenses.
- Creating an abstraction for headtracking to support different kind of devices (GearVR, DK1, DK2, other headsets etc)
- Your idea here :)
