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

========================
TODOs
========================	
If you wish to contribute, then please consider the following :

- Add servomotor control natively in OpenFrameworks. (at the moment it is done with pure data)
- Use ofxXmlSettings for settings such as experiment type, camera choice, Ip adresses etc.
- Create a GUI for the corresponding XmlSettings that allows to save them.
- Map video on a sphere, and use quaternions instead of euler angles for orientation.
- DK2 display + headtrcking support.
- DK2 position tracking support.
- Camera abstraction to support different devices. (logitech C310, ovrvision, clduo, PS3eye, etc)
- Create debug view.
- Barrel Shader class to adapt to different lenses.
- Creating an abstraction for headtracking to support different kind of devices (GearVR, DK1, DK2, other heasets etc)

Check the corresponding issues
