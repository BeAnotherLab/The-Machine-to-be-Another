The-Machine-to-be-Another
=========================
This is the software for using The Machine to Be Another on an Oculus Rift. It's built fundamentaly on Open Frameworks, except for a few aspects that are still running on PureData.

It includes both the traditional version for a user and a performer with a servomotor (currently run by PureData), as well as the GenderSwap version for two users without servomotors. The version may be selected from the testApp.h. 

The communication between computers, between OpenFrameworks and PureData, and between a smartphone controller and OpenFrameworks, is achieved via Open Sound Control. OSC settings may be changed from the testApp.h.

=========================
Getting Started
=========================
The project was tested with Openframeworks 0.8.4 on Windows 7&8 + VS Express 2012 and Maverick + XCode

1. Download the openframeworks 0.8.4 toolkit and unzip it
2. Using the git console execute```git clone https://github.com/BeAnotherLab/The-Machine-to-be-Another``` inside the ```openframeworks_folder/apps/myapps```. If you download the project as a zip file using the github web interface remove the "-master" extension from the name of the folder.
3. TODO: Download the required addons inside the folder ```openframeworks_folder/addons``` by running the clone_addons script or manually by.. TODO
4. Open the project from Visual Studio or XCode and compile.


=========================
Smartphone Open Sound Control
=========================
This is achieved using Charlie Roberts' GPL Control app which allows users to define interfaces via javascript and upload them through a web server. The current interface for controlling The Machine to Be Another can be found at /oscControllers/theMachineButtons.js, or at http://marte.me/osc/theMachineButtons .

In order to upload an interface in the Control app go to "Interfaces" and select the "+" button on the upper right corner of the screen. Afterwards add the path of your javascript file (it's recommended that you downloaded from a web server, e.g. http://marte.me/osc/theMachineButtons) and click Submit. Your new interface should appear in the Interfaces menu as "TheMachineToBeAnother".

To run, add your the IP and port (6999 by default) of the computer running TheMachineToBeAnother. Also add the IP of your smartphone on OpenFrameworks under "#define PHONE_IP" on the testApp.h.


========================
TODOs
========================	
If you wish to contribute, then please consider the following :

1) Add the servomotor control natively in OpenFrameworks.
2) Use ofxXmlSettings for settings such as experiment type, camera choice, Ip adresses etc
3) Create a GUI for selecting the version, camera and adding IPs and ports.
5) Barrel Shader class to adapt to different lenses
6) creating an abstraction for headtracking to support different kind of devices (GearVR, DK1, DK2, Cardboard)

Check the corresponding issues
