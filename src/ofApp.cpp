#include "ofApp.h"
#include "OVR.h"

using namespace OVR;
using namespace std;
OVR::Ptr<OVR::DeviceManager>	pManager;
OVR::Ptr<OVR::HMDDevice>		pHMD;
OVR::Ptr<OVR::SensorDevice>		pSensor;
OVR::SensorFusion				FusionResult;
OVR::HMDInfo					Info;
bool							InfoLoaded;    
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){	
	//tested with PS3Eye camera.
	
	recording = false;	
	recorder.setPrefix(ofToDataPath("recordings/frame_")); // this directory must already exist
    recorder.setFormat("jpg"); //png is really slow but high res, bmp is fast but big, jpg is just right
    	
	//init oculus rift
	System::Init();
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
	if (pHMD) {
       	InfoLoaded = pHMD->GetDeviceInfo(&Info);
	   	pSensor = *pHMD->GetSensor();
	}	
	else {
		pSensor = *pManager->EnumerateDevices<SensorDevice>().CreateDevice();
	}
	if (pSensor) {
		FusionResult.AttachToSensor(pSensor);
	}	

	sender.setup(HOST, SENDER_PORT);    
	receiver.setup(LISTEN_PORT);    

	machine.setup();

	//init rxbuttons value	
}

//--------------------------------------------------------------
void ofApp::update(){			
	if (machine.vidGrabber.isFrameNew() && recording) {
		recorder.addFrame(machine.vidGrabber);   
	}    		
        
	if(pSensor)	{
		Quatf quaternion = FusionResult.GetOrientation();		
		quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&machine.yaw, &machine.pitch, &machine.roll);
	}	
	
	//send calibrated angles value over OSC.
	ofxOscMessage m;
	m.setAddress("/ori");	
	m.addFloatArg(machine.roll-machine.roll_cal);
	m.addFloatArg(machine.pitch-machine.pitch_cal);
	m.addFloatArg(machine.yaw-machine.yaw_cal);	
	sender.sendMessage(m);     	
    
	machine.update();

	//receive other's headtracking
	while(receiver.hasWaitingMessages()){
		ofxOscMessage rx_msg;
		receiver.getNextMessage(&rx_msg);				
        if(rx_msg.getAddress() == "/ori") {
			machine.rx_roll = rx_msg.getArgAsFloat(0);
			machine.rx_pitch = rx_msg.getArgAsFloat(1);
			machine.rx_yaw = rx_msg.getArgAsFloat(2);                  
			///PRINT IF RECEIVED (RX) MESSAGES
			ofDrawBitmapString("Connection ON",10,10);          
		}
	}	

	stringstream c;		
	if (recording) {
		c << "Recording" <<  " | Queue Size: " << recorder.q.size() << endl;
	} else if (!recording && recorder.q.size() > 0) {
		c << "Queue Size: " << recorder.q.size() << endl;
	}
	else if (recorder.q.size() == 0) {
		recorder.stopThread();		
	}
	ofDrawBitmapString(c.str(), 650, 10);

	cout << "yaw " << machine.yaw << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){   
	ofBackground(185);	        		
	ofSetHexColor(0xffffff);									    
	machine.drawVideo();
	machine.drawOverlay();    
}

void ofApp::clear()
{
	pSensor.Clear();
        pHMD.Clear();
	pManager.Clear();  	
	System::Destroy();	
}



void ofApp::output()
{
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){		
	
	if (key == 'w' || key == 'w'){
		machine.y_offset -= 2;
		cout << machine.y_offset;
		//5016
	}

	if (key == 's' || key == 'S'){
		machine.y_offset += 2;
		cout << machine.y_offset;
	}
	
	if (key == 'a' || key == 'A'){
		machine.x_offset -= 2;
		//37
		cout << machine.x_offset;
	}

	if (key == 'd' || key == 'D'){
		machine.x_offset += 2;
		//37
		cout << machine.x_offset;
	}	    
   
   if (key == ' ') { 	   
	   machine.calibrate();
   }
   
	if (key == 'r') {
        recording = !recording;
        recorder.startThread(false, true);   
    }
    
    currentKey = key;

}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){	
	machine.calibrate();		
}

void ofApp::exit(){
    recorder.waitForThread();
}

/*		int totalPixels = camWidth*camHeight;
		unsigned char * pixels = vidGrabber.getPixels();
		for (int layer=0; layer<3; layer++){
			layer_offset=camWidth*camHeight*layer;
			for (int x=0; x<camWidth/2; x++){
				for (int y=0; y<camHeight; y++){
					pixels[(y*camWidth+x)+layer_offset] = pixels[(y*camWidth+(camWidth-x))+layer_offset];
				}			
			}*/
		//}
	//}

/*	cout << "----- Oculus Console -----" << endl;

	if (pHMD)
	{
		cout << " [x] HMD Found" << endl;
	}
	else
	{
		cout << " [ ] HMD Not Found" << endl;
	}

	if (pSensor)
	{
		cout << " [x] Sensor Found" << endl;
	}
	else
	{
		cout << " [ ] Sensor Not Found" << endl;
	}

	cout << "--------------------------" << endl;

	if (InfoLoaded)
        {
		cout << " DisplayDeviceName: " << Info.DisplayDeviceName << endl;
		cout << " ProductName: " << Info.ProductName << endl;
		cout << " Manufacturer: " << Info.Manufacturer << endl;
		cout << " Version: " << Info.Version << endl;
		cout << " HResolution: " << Info.HResolution<< endl;
		cout << " VResolution: " << Info.VResolution<< endl;
		cout << " HScreenSize: " << Info.HScreenSize<< endl;
		cout << " VScreenSize: " << Info.VScreenSize<< endl;
		cout << " VScreenCenter: " << Info.VScreenCenter<< endl;
		cout << " EyeToScreenDistance: " << Info.EyeToScreenDistance << endl;
		cout << " LensSeparationDistance: " << Info.LensSeparationDistance << endl;
		cout << " InterpupillaryDistance: " << Info.InterpupillaryDistance << endl;
		cout << " DistortionK[0]: " << Info.DistortionK[0] << endl;
		cout << " DistortionK[1]: " << Info.DistortionK[1] << endl;
		cout << " DistortionK[2]: " << Info.DistortionK[2] << endl;
		cout << "--------------------------" << endl;
        }

	cout << endl << " Press ENTER to continue" << endl;

	cin.get();
*/


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


