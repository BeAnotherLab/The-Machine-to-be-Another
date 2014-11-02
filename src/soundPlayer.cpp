#include "soundPlayer.h"

void soundPlayer::loadSounds(string s)
{
	//load sounds
	//string s("fab10 Welcome Legs Part2 Slowly CloseEyes Part3 Goodbye");
	
	cout << "sounds" << s << endl;
    istringstream iss(s);
	count = 0;
    do
    {
		count++;
        string sub;
        iss >> sub;
        cout << "Substring: " << sub << endl;
		ofSoundPlayer so = *new ofSoundPlayer();
		sounds.push_back(so);		
		sounds.at(count).loadSound("sounds/"+sub+".mp3");					
		cout << "loading " << "sounds/"+sub+".mp3" << endl;
    } while (iss);
		//sounds[i].loadSound(sound_names.
		
	sounds.at(0).play(); //Initialize music on startup
		
    //OSC CONTROLER
    #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
		phoneOscReceiver.setup(PHONE_LISTENER_PORT);
		phoneOscSender.setup(PHONE_IP, PHONE_SENDER_PORT);
    #endif    

}

void soundPlayer::loop() 
{

}
/*
soundPlayer::~soundPlayer(void)
{
}
*/