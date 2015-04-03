#include "testApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){		
	ofSetVerticalSync(false);	
	recording = false;	
	recorder.setPrefix(ofToDataPath("recordings/frame_")); // this directory must already exist
    recorder.setFormat("jpg"); //png is really slow but high res, bmp is fast but big, jpg is just right    			
	player.loadSounds("genderswapmusic welcome_fr lookaround_fr objects_fr shakehands_fr bye_fr slowly_en follow_fr calibrate_fr view_fr legs_fr"); //genderswapmusic welcome_ch standby_ch shakehands_ch goodbye_ch moveslowly_ch lookathands_ch movefingers_ch lookaround_ch welcome_en standby_en shakehands_en goodbye_en moveslowly_en lookathands_en movefingers_en lookaround_en"

    machine.setup(TWO_WAY_SWAP, MONO); 
	controller.setup(&machine, &player, COMPUTER);

	ofxFenster* win = ofxFensterManager::get()->createFenster(640, 480, OF_WINDOW);
	win->addListener(this);		
	
	setGUI();
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
//		machine.debug();
	} else if (window->id==1) {
		ofBackground(0);	 
		machine.drawVideo();
		//gui->draw();
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
	
	if (key == OF_KEY_END) {
		hideGUI != hideGUI; 
	}
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
void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl; 	
	
	if(name == "DRAW GRID")	{
		ofxUIButton *button = (ofxUIButton *) e.widget; 
		//bdrawGrid = button->getValue(); 
	}
	else if(name == "D_GRID") {
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
		//bdrawGrid = toggle->getValue(); 
	}
    else if(name == "TEXT INPUT") {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget; 
        if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)  {
            cout << "ON ENTER: "; //ofUnregisterKeyEvents((testApp*)this); 
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
        {
            cout << "ON FOCUS: "; 
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cout << "ON BLUR: "; 
//            ofRegisterKeyEvents(this);             
        }        
        string output = textinput->getTextString(); 
        cout << output << endl; 
    }			
}

void ofApp::setGUI()
{		
	//gui->loadSettings("GUI/settings.xml"); 

	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
	hideGUI = false; 
	
    vector<string> names; 
	names.push_back("RAD1");
	names.push_back("RAD2");
	names.push_back("RAD3");	

    vector<string> names2; 
	names2.push_back("RAD4");
	names2.push_back("RAD5");
	names2.push_back("RAD6");	
	
	gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight()); 
	gui->addWidgetDown(new ofxUILabel("PANEL 1: BASICS", OFX_UI_FONT_LARGE)); 
    gui->addWidgetDown(new ofxUILabel("Press 'h' to Hide GUIs", OFX_UI_FONT_LARGE)); 

    gui->addSpacer(length-xInit, 2); 
	gui->addWidgetDown(new ofxUILabel("H SLIDERS", OFX_UI_FONT_MEDIUM)); 
//	gui->addSlider("RED", 0.0, 255.0, red, length-xInit,dim);

    gui->addSpacer(length-xInit, 2); 
    gui->addWidgetDown(new ofxUILabel("V SLIDERS", OFX_UI_FONT_MEDIUM)); 
	gui->addSlider("0", 0.0, 255.0, 150, dim,160);
	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	gui->addSlider("1", 0.0, 255.0, 150, dim,160);
	gui->addSlider("2", 0.0, 255.0, 150, dim,160);
	gui->addSlider("3", 0.0, 255.0, 150, dim,160);
	gui->addSlider("4", 0.0, 255.0, 150, dim,160);
	gui->addSlider("5", 0.0, 255.0, 150, dim,160);
	gui->addSlider("6", 0.0, 255.0, 150, dim,160);
	gui->addSlider("7", 0.0, 255.0, 150, dim,160);
	gui->addSlider("8", 0.0, 255.0, 150, dim,160);
	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer(length-xInit, 2);     
	gui->addWidgetDown(new ofxUIRadio( dim, dim, "RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL)); 
	gui->addWidgetDown(new ofxUIRadio( dim, dim, "RADIO VERTICAL", names2, OFX_UI_ORIENTATION_VERTICAL)); 

    gui->addSpacer(length-xInit, 2); 
	gui->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM)); 
	gui->addWidgetDown(new ofxUIButton( dim, dim, false, "DRAW GRID")); 	
	gui->addWidgetDown(new ofxUILabel("TOGGLES", OFX_UI_FONT_MEDIUM)); 
	gui->addWidgetDown(new ofxUIToggle( dim, dim, false, "D_GRID")); 	
    
    gui->addSpacer(length-xInit, 2); 
    gui->addWidgetDown(new ofxUILabel("RANGE SLIDER", OFX_UI_FONT_MEDIUM)); 
	gui->addWidgetDown(new ofxUIRangeSlider(length-xInit,dim, 0.0, 255.0, 50.0, 100.0, "RSLIDER")); 	

    gui->addSpacer(length-xInit, 2); 
	gui->addWidgetDown(new ofxUILabel("2D PAD", OFX_UI_FONT_MEDIUM)); 
	gui->addWidgetDown(new ofxUI2DPad(length-xInit,120, ofPoint((length-xInit)*.5,120*.5), "PAD")); 	
    
	ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
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
	gui->saveSettings("settings.xml");     
	delete gui; 
}