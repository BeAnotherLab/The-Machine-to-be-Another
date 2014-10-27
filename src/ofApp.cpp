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
	ofSetFullscreen(true);		
	
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(120);
	vidGrabber.initGrabber(camWidth,camHeight);	
	
	ofSetVerticalSync(true);
	
	tex.setTextureWrap(GL_REPEAT, GL_REPEAT);

	System::Init();
	pitch = 0;
	yaw = 0;
	roll = 0;
	    
    //There are the parameters for the polynomial warp function to correct for the Oculus Rift and Webcam Lenses
    K0 = 1.0;
    K1 = 5.74;
    K2 = 0.27;
    K3 = 0.0;
    _x = 0.0f;
    _y = 0.0f;
    _w = 1.0f;
    _h = 1.0f;
    as = 640.0f/800.0f;
	DistortionXCenterOffset = 90;
	    
    ofEnableNormalizedTexCoords();    
    hmdWarpShader.load("shaders/HmdWarpExp");

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

	//init rxbuttons value
}

//--------------------------------------------------------------
void ofApp::update(){		
	vidGrabber.update();
	if (vidGrabber.isFrameNew() && recording) {
		recorder.addFrame(vidGrabber);   
	}    		
        
	if(pSensor)	{
		Quatf quaternion = FusionResult.GetOrientation();		
		quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);
	}	

	//send angles value over OSC to control the servos
	ofxOscMessage m;
	m.setAddress("/ori");	
	m.addFloatArg(roll);//-roll_cal);
	m.addFloatArg(pitch);//-pitch_cal);
	m.addFloatArg(yaw);//-yaw_cal);	
	sender.sendMessage(m); //send headtracking to pure data

}

//--------------------------------------------------------------
void ofApp::draw(){   
	ofBackground(0,0,0);	        		
	ofSetHexColor(0xffffff);			
	/*
	hmdWarpShader.begin();
	hmdWarpShader.setUniformTexture("tex", vidGrabber.getTextureReference(), 0);
    hmdWarpShader.setUniform2f("LensCenter", DistortionXCenterOffset, 0 );
    hmdWarpShader.setUniform2f("ScreenCenter", _x + _w*1.0f, _y + _h*1.0f );
    hmdWarpShader.setUniform2f("Scale", (_w/1.0f) * 1.0f, (_h/1.0f) * 1.0f * as );
    hmdWarpShader.setUniform2f("ScaleIn", (1.0f/_w), (1.0f/_h) / as );
    hmdWarpShader.setUniform4f("HmdWarpParam", K0, K1, K2, K3 );

	glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(0,0,0);
    glTexCoord2f(0,0); glVertex3f(640,0,0);
    glTexCoord2f(0,1); glVertex3f(640,800,0);
    glTexCoord2f(1,1); glVertex3f(0,800,0);
    glEnd();
	hmdWarpShader.end();

	*/
	//duplicate video stream. You can also draw videograbbers from 2 different cameras. WASD to adjust the position of image.
	ofPushMatrix();		
		ofTranslate(camWidth/2, camHeight/2, 0);//move pivot to centre
		ofRotate(90, 0, 0, 1);//rotate from centre				
			vidGrabber.draw(y_offset-camWidth/2,-x_offset-camHeight/2);//move back by the centre offset
			vidGrabber.draw(y_offset-camWidth/2,x_offset-880);		
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
	
	soundPlayer();
}

void ofApp::soundPlayer()
{    
    bool somethingIsPlaying;
    
    // update the sound playing system:
	ofSoundUpdate();    
  
    #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
        //OSC Receiver
        while(phoneOscReceiver.hasWaitingMessages()){
            ofxOscMessage msg;
            phoneOscReceiver.getNextMessage(&msg);
                    
			for (int i=0; i<8; i++) {				
				stringstream a;
				a << "/btn" << i << endl;				
				//check if button was pressed on phone
				if(msg.getAddress() == a.str()) rxButtons[i] = msg.getArgAsInt32(0);
			}
	    }
        //OSC Sender
        ofxOscMessage sendM;
        sendM.setAddress("/nowPlaying");    
    #endif
	
	somethingIsPlaying = false;  
	stringstream isplaying;
	//isplaying << "hello" << endl;

	for (int i=1; i<8; i++) {
		//isplaying << "is playing : " << i << " " << sounds[i].getIsPlaying() << endl;
		if (sounds[i].getIsPlaying()) somethingIsPlaying = true;
	}   

	ofDrawBitmapString(isplaying.str(), 10, 80);
              
    //play tracks through OSC buttons
	for (int i=0; i<8; i++) {		
		stringstream val, cur, test;
		val << i << endl;
		cur << currentKey << endl;					
		/*
		test << "currentKey" << currentKey << endl;
		ofDrawBitmapString(test.str(), 20, 60);
		*/
		//play sound i if button i was pressed on phone or on keyboard
		if ((rxButtons[i] == 1 || cur.str() == val.str()) && !somethingIsPlaying){						
			sounds[i].play();				
			stringstream play;
			play << "sound " << i << " is playing" << endl;
			ofDrawBitmapString(play.str(), 10, 30);			
		}

		#if (OSC_CONTROL_STATUS == OSC_STATUS_ON)			
            sendM.addStringArg(str);
            phoneOscSender.sendMessage(sendM);
        #endif
	}    
    
	//SIMPLE SIDECHAIN COMPRESSION    
    if (somethingIsPlaying) sounds[0].setVolume(0.5);        
    
	else if (!somethingIsPlaying) {
        sounds[0].setVolume(1);        
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("nothing is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
    }    
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
