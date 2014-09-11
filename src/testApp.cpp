#include "testApp.h"
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

#if (CAMERA == CAMERA_OVR)              
    #include <ovrvision.h>
    OVR::Ovrvision* g_pOvrvision;
#endif
//#include <conio.h>

//--------------------------------------------------------------
void testApp::setup(){	
	//tested with PS3Eye camera.
	x_offset = 74;
	y_offset = 164;
	camWidth = 640;
	camHeight = 480;		
	layer_offset = camWidth*camHeight;
	ofSetFullscreen(true);
		
    #if (CAMERA == CAMERA_WEBCAM)
		vidGrabberLeft.setVerbose(true);
		vidGrabberLeft.setDeviceID(0);
		vidGrabberLeft.setDesiredFrameRate(60);
		vidGrabberLeft.initGrabber(camWidth,camHeight);
	#endif
	
	#if (CAMERA == CAMERA_OVR)
	    g_pOvrvision = new OVR::Ovrvision();
	    g_pOvrvision->Open(0,OVR::OV_CAMVGA_FULL);   
	    left.allocate(640,480,GL_RGB);
	    right.allocate(640,480,GL_RGB);
	#endif
	
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

    
	#if (VERSION == VERSION_GENDER_SWAP)
		receiver.setup(LISTEN_PORT);
        senderComputer.setup(IP_COMPUTER, SENDER_PORT);
        endTimer = 1;
	#endif

	ofEnableAlphaBlending();	

	recording = false;	
	recorder.setPrefix(ofToDataPath("recordings/frame_")); // this directory must already exist
    recorder.setFormat("jpg"); //png is really slow but high res, bmp is fast but big, jpg is just right
    
    //SOUND PLAYER
    sound0.loadSound("sounds/fab10.mp3");
    sound1.loadSound("sounds/Welcome.mp3");
	sound2.loadSound("sounds/Legs.mp3");
	sound3.loadSound("sounds/Part2.mp3");
    sound4.loadSound("sounds/Slowly.mp3");
	sound5.loadSound("sounds/CloseEyes.mp3");
	sound6.loadSound("sounds/Part3.mp3");
    sound7.loadSound("sounds/Goodbye.mp3");
    
    sound0.play(); //INITIALIZE MUSIC ON STARTUP, COMMENT IN OR OUT
    
    //OSC CONTROLER
    #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
    phoneOscReceiver.setup(PHONE_LISTENER_PORT);
    phoneOscSender.setup(PHONE_IP, PHONE_SENDER_PORT);
    #endif
    

}

//--------------------------------------------------------------
void testApp::update(){	
	ofBackground(0,0,0);	
//	cout << "1/(ofGetElapsedTimef()-lastCaptureTime)  " << 1/(ofGetElapsedTimef()-lastCaptureTime) << "\n";
    
    soundPlayer();

		
	#if (CAMERA == CAMERA_WEBCAM)
//		vidGrabberLeft.grabFrame();
        vidGrabberLeft.update();
		if (vidGrabberLeft.isFrameNew() && recording) {
			recorder.addFrame(vidGrabberLeft);   
		}
    
	#endif

	#if (CAMERA == CAMERA_OVR)        
		left.loadData(g_pOvrvision->GetCamImage(OVR::OV_CAMEYE_LEFT),640,480,GL_RGB);
        right.loadData(g_pOvrvision->GetCamImage(OVR::OV_CAMEYE_RIGHT),640,480,GL_RGB);
    #endif
        
	if(pSensor)	{
		Quatf quaternion = FusionResult.GetOrientation();		
		quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){   
	output();
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
		#if (CAMERA == CAMERA_WEBCAM)
			vidGrabberLeft.draw(y_offset-camWidth/2,-x_offset-camHeight/2);//move back by the centre offset
			vidGrabberLeft.draw(y_offset-camWidth/2,x_offset-880);
		#endif
	ofPopMatrix();
	ofPushMatrix();
	    ofTranslate(365,224+y_offset, 0); //move to top left image corner		
		#if (CAMERA == CAMERA_OVR)            
			left.draw(-camWidth/2,-camHeight/2);                                     
        #endif		
	ofPopMatrix();	
	ofPushMatrix();	
	
	ofTranslate(365+490+x_offset,224+y_offset, 0);
	#if (CAMERA == CAMERA_OVR)            
		right.draw(-camWidth/2,-camHeight/2);                                     
    #endif	
	ofPopMatrix();
	
	#if (VERSION == VERSION_GENDER_SWAP)    
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
		
			if(rx_msg.getAddress() == "/ori"){
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
    
	#endif
			
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

void testApp::soundPlayer()
{
    int rxButton1, rxButton2, rxButton3, rxButton4, rxButton5, rxButton6, rxButton7;
    bool somethingIsPlaying;
    
    // update the sound playing system:
	ofSoundUpdate();
    
  
    #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
        //OSC Receiver
        while(phoneOscReceiver.hasWaitingMessages()){
            ofxOscMessage msg;
            phoneOscReceiver.getNextMessage(&msg);
        
            //had to add independent buttons for each separator
            
            if(msg.getAddress() == "/btn1")
                rxButton1 = msg.getArgAsInt32(0);
            
            if(msg.getAddress() == "/btn2")
                rxButton2 = msg.getArgAsInt32(0);
        
            if(msg.getAddress() == "/btn3")
                rxButton3 = msg.getArgAsInt32(0);
        
            if(msg.getAddress() == "/btn4")
            rxButton4 = msg.getArgAsInt32(0);
        
            if(msg.getAddress() == "/btn5")
                rxButton5 = msg.getArgAsInt32(0);
        
            if(msg.getAddress() == "/btn6")
            rxButton6 = msg.getArgAsInt32(0);
        
            if(msg.getAddress() == "/btn7")
            rxButton7 = msg.getArgAsInt32(0);
        
    }
        //OSC Sender
        ofxOscMessage sendM;
        sendM.setAddress("/nowPlaying");
    
    #endif
    
    
    if (sound1.getIsPlaying() || sound2.getIsPlaying() || sound3.getIsPlaying() || sound4.getIsPlaying() || sound5.getIsPlaying() || sound6.getIsPlaying() || sound7.getIsPlaying()) {
        somethingIsPlaying = true;
    }
    
    else somethingIsPlaying = false;
    
    
    //play tracks through OSC buttons
    
    if ((rxButton1 == 1 || currentKey == "1") && !somethingIsPlaying){
		sound1.play();
        
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("sound 1 is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
	}
    
    
    else if ((rxButton2 == 1 || currentKey == "2") && !somethingIsPlaying){
		sound2.play();
        
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("sound 2 is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
	}
    
    else if ((rxButton3 == 1  || currentKey == "3") && !somethingIsPlaying){
		sound3.play();
        
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("sound 3 is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
    }
    
    else if ((rxButton4 == 1|| currentKey == "4") && !somethingIsPlaying){
		sound4.play();
        
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("sound 4 is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
    }
    
    else if ((rxButton5 == 1 || currentKey == "5") && !somethingIsPlaying){
		sound5.play();
        
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("sound 5 is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
    }
    
    else if ((rxButton6 == 1 || currentKey == "6") && !somethingIsPlaying){
		sound6.play();
        
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("sound 6 is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
    }
    
    else if ((rxButton7 == 1 || currentKey == "7") && !somethingIsPlaying){
		sound7.play();
       
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("sound 7 is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
    }
    
    
    if (somethingIsPlaying)
        sound0.setVolume(0.5);    //SIMPLE SIDECHAIN COMPRESSION
    
    else if (!somethingIsPlaying) {
        sound0.setVolume(1);
        
        #if (OSC_CONTROL_STATUS == OSC_STATUS_ON)
            sendM.addStringArg("nothing is playing");
            phoneOscSender.sendMessage(sendM);
        #endif
    }
    
}

void testApp::clear()
{
	pSensor.Clear();
        pHMD.Clear();
	pManager.Clear();
  	#if (CAMERA == CAMERA_OVR)
		delete g_pOvrvision;
    #endif
	System::Destroy();	
}



void testApp::output()
{
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
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
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
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
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

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

void testApp::exit(){
    recorder.waitForThread();
}
