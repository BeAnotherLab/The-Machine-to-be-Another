#include "testApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){		
	ofSetVerticalSync(true);	
	//ofSetFrameRate(80);
	recording = false;	
	recorder.setPrefix(ofToDataPath("recordings/frame_")); // this directory must already exist
    recorder.setFormat("bmp"); //png is really slow but high res, bmp is fast but big, jpg is just right    			
	//player.loadSounds("genderswapmusic welcome_fr lookaround_fr objects_fr shakehands_fr bye_fr slowly_fr follow_fr calibrate_fr view_fr legs_fr"); //genderswapmusic welcome_ch standby_ch shakehands_ch goodbye_ch moveslowly_ch lookathands_ch movefingers_ch lookaround_ch welcome_en standby_en shakehands_en goodbye_en moveslowly_en lookathands_en movefingers_en lookaround_en"
	settings.loadFile("settings.xml");			

	mySoundPlayer.loadSounds(&settings);		

    machine.setup(&settings); 
	controller.setup(&machine, &mySoundPlayer, &machine.videoPlayer, &settings);

	ofxFenster* win = ofxFensterManager::get()->createFenster(640, 480, OF_WINDOW);
	win->addListener(this);		

}

//--------------------------------------------------------------
void ofApp::update(){				    
	machine.update();
	mySoundPlayer.update();	
	controller.loop();				
	record();
}

//--------------------------------------------------------------
void ofApp::draw(ofxFenster* window){  
	if (window->id==0) { //monitor window
		window->setWindowTitle("monitoring window");
		ofBackground(0);	        				
		machine.drawMonitor(window);		
		machine.debug();		
		recordDebug();
	} else if (window->id==1) { //oculus window
		window->setWindowTitle("oculus window");		
		ofBackground(0);	 
		machine.drawVideo();		
	}
}

//--------------------------------------------------------------
void ofApp::record() { //uses memo akten ofxImageSequenceRecorder	
	if (machine.vidGrabberLeft.isFrameNew() && recording) {
		recorder.addFrame(machine.vidGrabberLeft);   
	}    		    

	if (recorder.q.size() > 1600) { 
		recording = false; //1600 is maximum queue size for 32-bit program
		cout << "reached queue max size, stopping recording..." << endl;
	}
}

void ofApp::recordDebug() {	
	stringstream c;		

	if (recording) {
		c << "Recording" <<  " | Queue Size: " << recorder.q.size() << endl;
	} else if (!recording && recorder.q.size() > 0) {
		c << "Queue Size: " << recorder.q.size() << endl;
	}
	else if (recorder.q.size() == 0) {
		recorder.stopThread();		
	}
	ofDrawBitmapString(c.str(), 10, 130);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key, ofxFenster* window){			

	if (key == OF_KEY_LEFT) { //decrease IPD
		machine.ipd -= 2;		
		cout << machine.ipd;
		settings.setValue("settings:ipd", machine.ipd);	
	}

	if (key == OF_KEY_RIGHT) { //increase IPD
		machine.ipd += 2;
		cout << machine.ipd;
		settings.setValue("settings:ipd", machine.ipd);	
	}	    
   
	if (key == ' ') { 	   
		machine.calibrate();
	}
   
	if (key == 'd' || key == 'D') { 	   
		machine.setDebug();
	}

	if (key == 'r') {
        recording = !recording;
        if (recording) recorder.startThread(false, true);
    }       
	if (key == OF_KEY_UP) {
		machine.dimmed = false;
	}

	if (key == OF_KEY_DOWN) {
		machine.dimmed = true;
	}

	if (key == 'm' || key == 'M') {
		mySoundPlayer.muteUnmute();
	}

	if (key == 'o' || key == 'O') {
		machine.zoom -= 0.1;
		settings.setValue("settings:zoom", machine.zoom);	
	}

	if (key == 'p' || key == 'P') {
		machine.zoom += 0.1;
		settings.setValue("settings:zoom", machine.zoom);	
	}

	if (key == 'k' || key == 'K') {
		machine.speed -= 10;
		settings.setValue("settings:speed", machine.speed);	
	}

	if (key == 'l' || key == 'L') {
		machine.speed += 10;
		settings.setValue("settings:speed", machine.speed);	
	}

	if (key == 'n' || key == 'N') {
		machine.alignment -= 1;
		settings.setValue("settings:alignment", machine.alignment);	
	}

	if (key == 'b' || key == 'B') {   
		machine.alignment += 1;
		settings.setValue("settings:alignment", machine.alignment);	
	}
		if (key == 't' || key == 'T') {
		machine.drift_correction -= 20;
	}

	if (key == 'y' || key == 'Y') {   
		machine.drift_correction += 20;		
	}
	
	if ((key == 'f' || key == 'F')) {   		
		window->toggleFullscreen();
	}

	if ((key == 'v' || key == 'V')) {   
		if(machine.videoPlayer.videos.size()>0) machine.videoPlayer.playVideo(0);
	}

	if ((key == 'c' || key == 'C')) {   		
		machine.videoPlayer.stopVideo();
	}
	
	//play tracks through keys 0-9 
    if ((key>47) && (key < (48 + mySoundPlayer.sounds.size()))) {
		mySoundPlayer.playSound(key-48); //my non programmer solution to using keys 0-9        
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
	settings.saveFile("settings.xml");
}