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
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(120);
	vidGrabber.initGrabber(camWidth,camHeight);		
	//vidGrabber.setAnchorPercent(0.5,0.5);
	ofSetFullscreen(true);
	ofSetVerticalSync(true);
	
//	tex.setTextureWrap(GL_REPEAT, GL_REPEAT);
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
    as = 640.0f/480.0f;
	DistortionXCenterOffset = 90;
	        
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
    
	receiver.setup(LISTEN_PORT);
    senderComputer.setup(IP_COMPUTER, SENDER_PORT);
    endTimer = 1;	

	recording = false;	
	recorder.setPrefix(ofToDataPath("recordings/frame_")); // this directory must already exist
    recorder.setFormat("jpg"); //png is really slow but high res, bmp is fast but big, jpg is just right
    
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
	m.addFloatArg(roll-roll_cal);
	m.addFloatArg(pitch-pitch_cal);
	m.addFloatArg(yaw-yaw_cal);	
	sender.sendMessage(m);    

}

//--------------------------------------------------------------
void ofApp::draw(){   
	ofBackground(185);	        		
	ofSetHexColor(0xffffff);			
			/*
    float planeScale = 0.75;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;
    int planeGridSize = 20;
    int planeColumns = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
 */
    //ofPlanePrimitive plane = *(new ofPlanePrimitive());		
	//plane.set(planeWidth, planeHeight, planeColumns, planeRows, OF_PRIMITIVE_TRIANGLES);
	//plane.mapTexCoordsFromTexture(vidGrabber.getTextureReference);		
	//ofRotate(90,0,0,1);
	
	/*hmdWarpShader.begin();
	hmdWarpShader.setUniformTexture("tex", vidGrabber.getTextureReference(), 0);
    hmdWarpShader.setUniform2f("LensCenter", DistortionXCenterOffset, 0 );
    hmdWarpShader.setUniform2f("ScreenCenter", _x + _w*1.0f, _y + _h*1.0f );
    hmdWarpShader.setUniform2f("Scale", (_w/1.0f) * 1.0f, (_h/1.0f) * 1.0f * as );
    hmdWarpShader.setUniform2f("ScaleIn", (1.0f/_w), (1.0f/_h) / as );
    hmdWarpShader.setUniform4f("HmdWarpParam", K0, K1, K2, K3 );
		*/
	/*
	glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(0,0,0);
    glTexCoord2f(0,0); glVertex3f(640,0,0);
    glTexCoord2f(0,1); glVertex3f(640,800,0);
    glTexCoord2f(1,1); glVertex3f(0,800,0);
    glEnd();*/	

	ofPushMatrix();				
		ofTranslate(camWidth/2, camHeight/2, 0);//move pivot to centre
		ofRotate(90, 0, 0, 1);//rotate from centre							
		vidGrabber.draw(y_offset-camWidth/2,x_offset-880);	
		vidGrabber.draw(y_offset-camWidth/2,-x_offset-camHeight/2);
		//vidGrabber.draw(camWidth/2+y_offset,camHeight/2-x_offset);		
	ofPopMatrix();		
	//hmdWarpShader.end();

	//vidGrabber.draw(y_offset-camWidth/2,x_offset-880);		
		
	float timer_NoSync = ofGetElapsedTimef() - startTime_Sync;
	float timer_Sync = ofGetElapsedTimef() - startTime_NoSync;
    
	//Values mapped to -1 to 1 		
	//Yaw goes from -pi to pi 			
	float yaw_mappedTo1 = ((yaw+3.141592)/(3.141592))-1;
	float yaw_cal_mappedTo1 = ((yaw_cal+3.141592)/(3.141592))-1;
	//Pitch goes from -pi/2 to pi
	float pitch_mappedTo1 = pitch/(3.141592/2);
	float pitch_cal_mappedTo1 = pitch_cal/(3.141592/2);
    
	//Vectors of each users view point.
	ofVec2f vec(yaw_mappedTo1-yaw_cal_mappedTo1,pitch_mappedTo1-pitch_cal_mappedTo1);
	ofVec2f rx_vec(((rx_yaw+3.141592)/(3.141592)-1), rx_pitch/(3.141592/2));
   
	//float distance = (abs(vec.length() - rx_vec.length()));
	float distance = vec.distance(rx_vec);
    
	//receive other's headtracking
	while(receiver.hasWaitingMessages()){
		ofxOscMessage rx_msg;
		receiver.getNextMessage(&rx_msg);
		
		if(rx_msg.getAddress() == "/ori") {
			rx_roll = rx_msg.getArgAsFloat(0);
			rx_pitch = rx_msg.getArgAsFloat(1);
			rx_yaw = rx_msg.getArgAsFloat(2);                  
			///PRINT IF RECEIVED (RX) MESSAGES
			//ofDrawBitmapString("Connection ON",10,10);
          
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


