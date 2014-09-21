The-Machine-to-be-Another
=========================
This is the repository for the Machine to be Another, built with openFrameworks. At the moment we are still using bits of software written in Pure Data (check the repo for Servos-Control) that should be merged into the main project soon.

It includes both the traditional version for a user and a performer with a servomotor (currently run by PureData), as well as the GenderSwap version for two users without servomotors. The version may be selected from the testApp.h. 

The communication between computers, between OpenFrameworks and PureData, and between a smartphone controller and OpenFrameworks, is achieved via Open Sound Control. OSC settings may be changed from the testApp.h.

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
2) Create a GUI for selecting the version, camera and adding IPs and ports.
3) Creating a black screen that may be faded in and out via OSC from a smartPhone.

Check the corresponding issues
