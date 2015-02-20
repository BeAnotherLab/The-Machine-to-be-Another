#include "oscController.h"

#define PORT 8015 //for all OSC communications	
#define HOST "192.168.4.117" //other computer to send headtracking to

void oscController::setup(machine* m, soundPlayer* p){
	myMachine = m;
	mySoundPlayer = p;	
	if (myMachine->setup_type == TWO_WAY_SWAP) sender.setup("192.168.4.117", 8015);
	else if (myMachine->setup_type == ONE_WAY_SWAP) sender.setup("localhost", PORT);	
	receiver.setup(PORT);    	
	//tabletOscSender.setup(PHONE_IP, PHONE_SENDER_PORT);    
}	

void oscController::loop() { 
	//receive tablet messages
	//yaw+=
	ofxOscMessage rx_msg;	
	while (receiver.hasWaitingMessages()) {
		receiver.getNextMessage(&rx_msg);				
		if (rx_msg.getAddress() == "/dim") {			
			myMachine->triggerDim();			
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
	}
	
	//send headtracking, either other computer or localhost for pd patch that controls servos.
	ofxOscMessage m;
	m.setAddress("/ori");
	for (int i=0; i<3; i++) {
		m.addFloatArg(myMachine->getHeadtracking()[i]);
	}	
	sender.sendMessage(m); 
}


void oscController::oscRepeat() { //if Computer 1, must repeat tablet OSC control to computer 2 
	ofxOscMessage rx_msg;

	//"lights off" and headtracking data
	if ((rx_msg.getAddress() == "/dim") || (rx_msg.getAddress() == "/ht")) {								
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