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
	camWidth = 960;
	camHeight = 720;		
	layer_offset = camWidth*camHeight;
	ofSetFullscreen(true);
		
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(1);
	vidGrabber.setDesiredFrameRate(60);
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
    
	receiver.setup(LISTEN_PORT);
    senderComputer.setup(IP_COMPUTER, SENDER_PORT);
    endTimer = 1;	

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
	ofBackground(0,0,0);	    
    soundPlayer();
		
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
	m.addFloatArg(roll-roll_cal);
	m.addFloatArg(pitch-pitch_cal);
	m.addFloatArg(yaw-yaw_cal);
	
	sender.sendMessage(m);    
	ofSetHexColor(0xffffff);			
	//duplicate video stream. You can also draw videograbbers from 2 different cameras. WASD to adjust the position of image.
	ofPushMatrix();		
		ofTranslate(camWidth/2, camHeight/2, 0);//move pivot to centre
		ofRotate(90, 0, 0, 1);//rotate from centre				
			vidGrabber.draw(y_offset-camWidth/2,-x_offset-camHeight/2);//move back by the centre offset
			vidGrabber.draw(y_offset-camWidth/2,x_offset-880);		
	ofPopMatrix();		
		
	senderComputer.sendMessage(m);   
	float timer_NoSync = ofGetElapsedTimef() - startTime_Sync;
	float timer_Sync = ofGetElapsedTimef() - startTime_NoSync;
    
	/*//    Values mapped to -1 to 1
		Yaw goes from -pi to pi
		Pitch goes from -pi/2 to pi    //2*/
	float yaw_mappedTo1 = ((yaw+3.141592)/(3.141592))-1;
	float yaw_cal_mappedTo1 = ((yaw_cal+3.141592)/(3.141592))-1;
	float pitch_mappedTo1 = pitch/(3.141592/2);
	float pitch_cal_mappedTo1 = pitch_cal/(3.141592/2);
    
	//Vectors of each users view point.
	ofVec2f vec(yaw_mappedTo1-yaw_cal_mappedTo1,pitch_mappedTo1-pitch_cal_mappedTo1);
	ofVec2f rx_vec(((rx_yaw+3.141592)/(3.141592)-1), rx_pitch/(3.141592/2));
   
	// float distance = (abs(vec.length() - rx_vec.length()));
	float distance = vec.distance(rx_vec);
    
	//OSC
	while(receiver.hasWaitingMessages()){
		ofxOscMessage rx_msg;
		receiver.getNextMessage(&rx_msg);
		
		if(rx_msg.getAddress() == "/ori") {
			rx_roll = rx_msg.getArgAsFloat(0);
			rx_pitch = rx_msg.getArgAsFloat(1);
			rx_yaw = rx_msg.getArgAsFloat(2);                  
			///PRINT IF RECEIVED (RX) MESSAGES
			// ofDrawBitmapString("Connection ON",10,10);
          
			}
        
		}
    
	   // ofDrawBitmapString("distance "+ ofToString(distance),10,40);
		// cout << " DistortionK[2]: " << Info.DistortionK[2] << endl;
    
		//if the users are looking too far appart
		if (distance > 0.07) {        
			startTime_NoSync = ofGetElapsedTimef();        
			//if (rx_pitch > -0.7 && rx_pitch < 0.88  && rx_yaw > -2.35 && rx_yaw < 2.35) {
        
				rx_pitch_limited = rx_pitch;
				rx_yaw_limited = rx_yaw;
            
				//headtrack of the other computer (RECEIVE headtracking and draw)
			   // ofSetColor(255,0,0);
			   // ofCircle(x_offset+camWidth/2-100*(rx_yaw_limited),y_offset+camHeight/2-300*(rx_pitch_limited),5);
			   // ofCircle(-x_offset+960-100*(rx_yaw_limited),y_offset+camHeight/2-300*(rx_pitch_limited),5);
        
				//headtrack of this computer (1st person headtracking and draw)
			   // ofSetColor(0,255,0);
			   // ofNoFill();
			   // ofCircle(x_offset+camWidth/2-100*(yaw-yaw_cal),y_offset+camHeight/2-300*(pitch-pitch_cal),12);
			   // ofCircle(-x_offset+960-100*(yaw-yaw_cal),y_offset+camHeight/2-300*(pitch-pitch_cal),12);
			   // ofFill();
		   // }
        
			//else {
				//headtrack of the other computer (RECEIVE headtracking and draw)
				ofSetColor(255,0,0);
				ofCircle(x_offset+camWidth/2-200*(rx_yaw_limited),y_offset+camHeight/2-300*(rx_pitch_limited),5);
				ofCircle(-x_offset+960-200*(rx_yaw_limited),y_offset+camHeight/2-300*(rx_pitch_limited),5);
            
				//headtrack of this computer (1st person headtracking and draw)
				ofSetColor(0,255,0);
				ofNoFill();
				ofCircle(x_offset+camWidth/2-200*(yaw-yaw_cal),y_offset+camHeight/2-300*(pitch-pitch_cal),12);
				ofCircle(-x_offset+960-200*(yaw-yaw_cal),y_offset+camHeight/2-300*(pitch-pitch_cal),12);
				ofFill();
			//    }
        
			///COMMENT THIS IF TO SHOW "OUT OF SYNC MESSAGE" CONSTANTLY
			//     if  (timer_NoSync < endTimer) {
			ofSetColor(255,0,0);
			ofDrawBitmapString("Out of sync",(camWidth/2+x_offset)-40, camHeight/2+y_offset );
			ofDrawBitmapString("Out of sync",(camWidth*1.5-x_offset)-40,camHeight/2+y_offset);
			//     }
        
			}
    
    
		//if the users are looking close enough
    
		else {
       
			startTime_Sync = ofGetElapsedTimef();
				if (timer_Sync < endTimer) {
					ofSetColor(0,0,255);
					ofDrawBitmapString("Synchronized",(camWidth/2+x_offset)-45, camHeight/2+y_offset);
					ofDrawBitmapString("Synchronized",(camWidth*1.5-x_offset)-45,camHeight/2+y_offset);
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
				char str[5];
				sprintf(str, "/btn %d", i);
				//check if button was pressed on phone
				if(msg.getAddress() ==  str) rxButtons[i] = msg.getArgAsInt32(0);
			}
	    }
        //OSC Sender
        ofxOscMessage sendM;
        sendM.setAddress("/nowPlaying");    
    #endif
	
	somethingIsPlaying = false;  
	for (int i=0; i<8; i++) {
		if (sounds[i].getIsPlaying());
		somethingIsPlaying = true;
	}   
              
    //play tracks through OSC buttons
	for (int i=0; i<8; i++) {
		char str[2];
		sprintf(str, "%d", i);

		//play sound i if button i was pressed on phone or on keyboard
		if((rxButtons[i] == 1 || currentKey == str) && !somethingIsPlaying) sounds[i].play();
		    
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
			char str[18];
			sprintf(str, "sound %d is playing", i);
            sendM.addStringArg(str);
            phoneOscSender.sendMessage(sendM);
        #endif
	}    
    
    
    if (somethingIsPlaying)
        sounds[0].setVolume(0.5);    //SIMPLE SIDECHAIN COMPRESSION
    
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
		cout << layer_offset;		
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
        #if (VERSION == VERSION_GENDER_SWAP)
       pitch_cal=pitch;
       yaw_cal=yaw;
       roll_cal=roll;
       rx_pitch_cal = rx_pitch;
       rx_yaw_cal = rx_yaw;
       rx_roll_cal = rx_roll;       
        #endif
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
	#if (VERSION == VERSION_GENDER_SWAP)
		pitch_cal=pitch;
		yaw_cal=yaw;
		roll_cal=roll;
		rx_pitch_cal = rx_pitch;
		rx_yaw_cal = rx_yaw;
		rx_roll_cal = rx_roll;       
    #endif
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

void ofApp::exit(){
    recorder.waitForThread();
}
