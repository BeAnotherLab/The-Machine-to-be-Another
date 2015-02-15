#include "ofApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){		
	ofSetFullscreen(true);
	ofSetVerticalSync(true);	
	recording = false;	
	recorder.setPrefix(ofToDataPath("recordings/frame_")); // this directory must already exist
    recorder.setFormat("jpg"); //png is really slow but high res, bmp is fast but big, jpg is just right    			
	player.loadSounds(""); //genderswapmusic welcome_ch standby_ch shakehands_ch goodbye_ch moveslowly_ch lookathands_ch movefingers_ch lookaround_ch welcome_en standby_en shakehands_en goodbye_en moveslowly_en lookathands_en movefingers_en lookaround_en"
	machine.setup(ONE_WAY_SWAP, STEREO); 
	controller.setup(&machine, &player);
}

//--------------------------------------------------------------
void ofApp::update(){				    
	machine.update();
	//player.update();
	//controller.loop();
	record();		
}

//--------------------------------------------------------------
void ofApp::draw(){   
	ofBackground(0);	        		
	ofSetHexColor(0xffffff);									    
	machine.drawVideo();
	machine.drawOverlay();    
}

//--------------------------------------------------------------
void ofApp::record() { //uses memo akten ofxImageSequenceRecorder
	stringstream c;		

	if (machine.vidGrabberLeft.isFrameNew() && recording) {
		recorder.addFrame(machine.vidGrabberLeft);   
	}    		
    
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

//--------------------------------------------------------------
void ofApp::keyPressed(int key){		
	
	if (key == OF_KEY_LEFT){ //decrease IPD
		machine.x_offset -= 2;		
		cout << machine.x_offset;
	}

	if (key == OF_KEY_RIGHT){ //increase IPD
		machine.x_offset += 2;
		cout << machine.x_offset;
	}	    
   
	if (key == ' ') { 	   
		machine.calibrate();
	}
   
	if (key == 'r') {
        recording = !recording;
        recorder.startThread(false, true);   
    }
       
	if (key == OF_KEY_UP) {
		machine.triggerDim();
	}

	if (key == 'm' || key == 'M') {
		player.muteUnmute();
	}

	//playtracks through keys 0-9 
    if ((key>47) && (key < (48 + player.sounds.size()))) {
		player.playSound(key-48); //my non programmer solution to using keys 0-9        
	}          
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){	

}

//--------------------------------------------------------------
void ofApp::clear()
{
	machine.clear();	
}

//--------------------------------------------------------------
void ofApp::exit(){
    recorder.waitForThread();
}