#include "testApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){		
	ofSetVerticalSync(false);	
	recording = false;	
	recorder.setPrefix(ofToDataPath("recordings/frame_")); // this directory must already exist
    recorder.setFormat("jpg"); //png is really slow but high res, bmp is fast but big, jpg is just right    			
	player.loadSounds("genderswapmusic welcome_fr lookaround_fr objects_fr shakehands_fr bye_fr slowly_fr follow_fr calibrate_fr view_fr legs_fr"); //genderswapmusic welcome_ch standby_ch shakehands_ch goodbye_ch moveslowly_ch lookathands_ch movefingers_ch lookaround_ch welcome_en standby_en shakehands_en goodbye_en moveslowly_en lookathands_en movefingers_en lookaround_en"

    machine.setup(TWO_WAY_SWAP, MONO); 
	controller.setup(&machine, &player, COMPUTER);

	ofxFenster* win = ofxFensterManager::get()->createFenster(640, 480, OF_WINDOW);
	win->addListener(this);		
	
	//put some some settings into a file
	ofxXmlSettings settings;
	settings.setValue("settings:blinkRate", 10);
	settings.setValue("settings:gravity", 9.8);
	settings.setValue("settings:sceneName", "field");
	settings.saveFile("settings.xml"); //puts settings.xml file in the bin/data folder
	
}

//--------------------------------------------------------------
void ofApp::update(){				    
	machine.update();
	player.update();
	controller.loop();			
	record();		
}

//--------------------------------------------------------------
void ofApp::draw(ofxFenster* window){  
//void ofApp::draw(){  
	if (window->id==0) {
		ofBackground(0);	        		
		ofSetHexColor(0xffffff);									    
		machine.drawVideo();
		machine.drawOverlay();    
		string framerate = ofToString(ofGetFrameRate());		
		ofDrawBitmapString(framerate, 10, 10);
//		machine.debug();
	} else if (window->id==1) {
		ofBackground(0);	 
		machine.drawVideo();		
	}
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
void ofApp::keyPressed(int key, ofxFenster* window){		
	

	if (key == OF_KEY_LEFT) { //decrease IPD
		machine.x_offset -= 2;		
		cout << machine.x_offset;
	}

	if (key == OF_KEY_RIGHT) { //increase IPD
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
		machine.dimmed = false;
	}

	if (key == OF_KEY_DOWN) {
		machine.dimmed = true;
	}

	if (key == 'm' || key == 'M') {
		player.muteUnmute();
	}

	if (key == 'o' || key == 'O') {
		machine.zoom -= 0.1;
	}

	if (key == 'p' || key == 'P') {
		machine.zoom += 0.1;
	}

	if (key == 'k' || key == 'K') {
		machine.speed -= 10;
	}

	if (key == 'l' || key == 'L') {
		machine.speed += 10;
	}

	if (key == 'n' || key == 'N') {
		machine.alignment -= 1;
	}

	if (key == 'm' || key == 'M') {   
		machine.alignment += 1;
	}
	
	if ((key == 'f' || key == 'F')) {   		
		window->toggleFullscreen();
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