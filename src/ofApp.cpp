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
	x_offset = 74;
	y_offset = 164;
	camWidth = 640;
	camHeight = 480;		
	layer_offset = camWidth*camHeight;
	ofSetFullscreen(true);
		
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(0);//1
	vidGrabber.setDesiredFrameRate(20);//60
	vidGrabber.initGrabber(camWidth,camHeight);	
	
	ofSetVerticalSync(true);
	
	System::Init();
	pitch = 0;
	yaw = 0;
	roll = 0;

	//init oculus headtracking
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
    
	sender.setup(HOST, PORT);
    

	recording = false;	
	recorder.setPrefix(ofToDataPath("recordings/frame_")); // this directory must already exist
    recorder.setFormat("jpg"); //png is really slow but high res, bmp is fast but big, jpg is just right
    

    //SOUND PLAYER
    sounds[0].loadSound("sounds/fab10.mp3");
    sounds[1].loadSound("sounds/Welcome.mp3");
	sounds[2].loadSound("sounds/Legs.mp3");
	sounds[3].loadSound("sounds/Part2.mp3");
    sounds[4].loadSound("sounds/Slowly.mp3");
	sounds[5].loadSound("sounds/CloseEyes.mp3");
	sounds[6].loadSound("sounds/Part3.mp3");
    sounds[7].loadSound("sounds/Goodbye.mp3");    
    sounds[0].play(); //INITIALIZE MUSIC ON STARTUP, COMMENT IN OR OUT
     
    
    //OSC CONTROLER
    #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
		phoneOscReceiver.setup(PHONE_LISTENER_PORT);
		phoneOscSender.setup(PHONE_IP, PHONE_SENDER_PORT);
    #endif    

}

//--------------------------------------------------------------
void ofApp::update(){	
	ofBackground(0,0,0);	        
    
        vidGrabber.update();
		if (vidGrabber.isFrameNew() && recording) {
			recorder.addFrame(vidGrabber);   
		}    		
        
	if(pSensor)	{
		Quatf quaternion = FusionResult.GetOrientation();		
		quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);
	}
    
}

//--------------------------------------------------------------
void ofApp::draw(){   
	//output();
	//send angles value over OSC to control the servos
	ofxOscMessage m;
	m.setAddress("/ori");	
	m.addFloatArg(roll);//-roll_cal);
	m.addFloatArg(pitch);//-pitch_cal);
	m.addFloatArg(yaw);//-yaw_cal);
    
    if (rxButtons[10] ==1 || currentKey == 32){ //servo correction control, either /btn10 or space key
        m.clear();
        m.setAddress("/correct");
        m.addIntArg(1);
        currentKey = NULL;
    }
    
    sender.sendMessage(m); //send headtracking to pure data

	ofSetHexColor(0xffffff);
	//duplicate video stream. You can also draw videograbbers from 2 different cameras. WASD to adjust the position of image.
	ofPushMatrix();		
		ofTranslate(camWidth/2, camHeight/2, 0);//move pivot to centre
		ofRotate(90, 0, 0, 1);//rotate from centre				
	
    
    //Draw screen if screen toggle is on on iPad
    #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
    if (rxButtons[11] == 1 || screenToggle){ //	ERROR at: "throw SystemException("cannot lock mutex")"  when stopping seems to be coming from the screenToggle for some reason?
            vidGrabber.draw(y_offset-camWidth/2,-x_offset-camHeight/2);//move back by the centre offset
			vidGrabber.draw(y_offset-camWidth/2,x_offset-880);
    }
    #else
            vidGrabber.draw(y_offset-camWidth/2,-x_offset-camHeight/2);//move back by the centre offset
            vidGrabber.draw(y_offset-camWidth/2,x_offset-880);
    #endif
    
	ofPopMatrix();
	
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
	
    oscManager();
	soundPlayer();
    
}


//--------------------------------------------------------------
void ofApp::oscManager() {
    
    #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
    
    while(phoneOscReceiver.hasWaitingMessages()){
            ofxOscMessage msg;
            phoneOscReceiver.getNextMessage(&msg);
            
        for (int i=0; i<12; i++) {
            stringstream a;
            a << "/btn" << i;
            if(msg.getAddress() == a.str()) rxButtons[i] = msg.getArgAsFloat(0);
           
            }
        }
    
    
        if (!somethingIsPlaying) {
            ofxOscMessage sendM;
            sendM.setAddress("/nowPlaying");
            sendM.addStringArg("nothing is playing");
            phoneOscSender.sendMessage(sendM);
    }
   
    #endif

}


//--------------------------------------------------------------
void ofApp::soundPlayer() {
    
    // update the sound playing system:
	ofSoundUpdate();
	
	somethingIsPlaying = false;  
	stringstream isplaying;

	for (int i=1; i<9; i++) if (sounds[i].getIsPlaying()) somethingIsPlaying = true;

    if (!somethingIsPlaying) {
        sounds[0].setVolume(1.);
        
        #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
        //play tracks through OSC buttons
        for (int i=0; i<9; i++) {
            
            if (rxButtons[i] != 0) {
                sounds[i].play();
                sounds[0].setVolume(0.5);
                }
            }
        #endif
        
        //play tracks through keys 0-9
       if (currentKey >= 48 && currentKey <= 57){
            sounds[(currentKey-48)].play(); //my non programmer solution to using keys 0-9
            sounds[0].setVolume(0.5);
            }
        }
    
    currentKey = NULL;
   
}


//--------------------------------------------------------------
void ofApp::clear()
{
	pSensor.Clear();
        pHMD.Clear();
	pManager.Clear();  	
	System::Destroy();	
}


//--------------------------------------------------------------
void ofApp::output()
{
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
	if (key == 'o' || key == 'O'){
		layer_offset +=2;
		cout << layer_offset;		
	}

	if (key == 'i' || key == 'I'){
		layer_offset -=2;
		cout << layer_offset << endl;		
	}

	if (key == 'w' || key == 'w'){
		y_offset -= 2;
		cout << y_offset;
		//5016
	}

	if (key == 's' || key == 'S'){
		y_offset += 2;
		cout << y_offset;
	}
	
	if (key == 'a' || key == 'A'){
		x_offset -= 2;
		//37
		cout << x_offset;
	}

	if (key == 'd' || key == 'D'){
		x_offset += 2;
		//37
		cout << x_offset;
	}	    
   
    if (key == 't' || key == 'T'){
        screenToggle = false;
    }
    
   if (key == ' ') {
        
   }
   
	if (key == 'r') {
        recording = !recording;
        recorder.startThread(false, true);   
    }
    
    currentKey = key;
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 't' || key == 'T'){
        screenToggle = true;
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
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

void ofApp::exit(){
    recorder.waitForThread();
}
