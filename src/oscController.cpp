#include "oscController.h"

void oscController::setup(machine* m, soundPlayer* p, ofxXmlSettings* s){
	myMachine = m;
	mySoundPlayer = p;	
	computerType = s->getValue("settings:computer_type", 0);
	if (myMachine->setup_type == TWO_WAY_SWAP) sender.setup(s->getValue("settings:ip", "localhost"), 8015);
	else if (myMachine->setup_type == ONE_WAY_SWAP) sender.setup("localhost", 8015);	
	receiver.setup(8015);    	
	//tabletOscSender.setup(PHONE_IP, PHONE_SENDER_PORT);    
}	

void oscController::loop() { 
	//receive tablet messages
	ofxOscMessage rx_msg;	
	while (receiver.hasWaitingMessages()) {
		receiver.getNextMessage(&rx_msg);						
		if (rx_msg.getAddress() == "/driftPlus") {			
			if(computerType==1){ 
				myMachine->calibration+=20;
			} else {
				myMachine->calibration-=20;
			}
		}				
		if (rx_msg.getAddress() == "/driftMinus") {			
			if(computerType==1){ 
				myMachine->calibration-=20;
			} else {
				myMachine->calibration+=20;
			}
		}
		if (rx_msg.getAddress() == "/dimoff") {			
			myMachine->dimmed=false;			
		}
		if (rx_msg.getAddress() == "/dimon") {			
			myMachine->dimmed=true;			
		}		
		else if (rx_msg.getAddress() == "/ht") {
			myMachine->calibrate();		
		}		
		//receive headtracking when two-way swap
		else if ((rx_msg.getAddress() == "/ori") && (myMachine->setup_type == TWO_WAY_SWAP)) {
			myMachine->rx_roll = rx_msg.getArgAsFloat(0);
			myMachine->rx_pitch = rx_msg.getArgAsFloat(1);
			myMachine->rx_yaw = rx_msg.getArgAsFloat(2);                  
		}
		for (int i=0; i<mySoundPlayer->count; i++) {
			stringstream a;
			a << "/btn" << i;
			if (rx_msg.getAddress() == a.str()) {
				mySoundPlayer->playSound(i); //play sound at i				
			} 
		}		
		
		if(computerType==1) oscRepeat(rx_msg);
	}
	
	//send headtracking, either other computer or localhost for pd patch that controls servos.
	ofxOscMessage m;
	m.setAddress("/ori");
	for (int i=0; i<3; i++) {
		m.addFloatArg(myMachine->getHeadtracking()[i]);
	}	
	sender.sendMessage(m); 
}


void oscController::oscRepeat(ofxOscMessage rx_msg) { //if Computer 1, must repeat tablet OSC control to computer 2

	//"lights off" and headtracking data
	if ((rx_msg.getAddress() == "/dimoff") || (rx_msg.getAddress() == "/dimon") || (rx_msg.getAddress() == "/ht") || (rx_msg.getAddress() == "/driftPlus") || (rx_msg.getAddress() == "/driftMinus" )) {								
		sender.sendMessage(rx_msg);			
	}			

	//sound player control
	for (int i=0; i<mySoundPlayer->count; i++) {
		stringstream a;
		a << "/btn" << i;
		if (rx_msg.getAddress() == a.str()) {		
			sender.sendMessage(rx_msg);			
		} 
	}
}
/*
void oscController::sendSoundPlaying(bool p, int i){ //TODO implement this
	ofxOscMessage message;
	message.setAddress("/1/label2");
    message.addStringArg(mySoundPlayer->getCurrentlyPlaying());
	message.addStringArg("nothing is playing");
    tabletOscSender.sendMessage(message);
}
*/