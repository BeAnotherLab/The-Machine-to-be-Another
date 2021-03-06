#pragma once
#include "ofxXmlSettings.h"

//oscMessages from and to Phone
#define PHONE_SENDER_PORT 8015
#define PHONE_IP "169.254.215.94"

class soundPlayer
{
public:
	int count; //the number of sounds deduced by the input string		

	vector<ofSoundPlayer> sounds; 
	stringstream sound_names;		
	float music_volume;	

	bool something_is_playing;
	void loadSounds(ofxXmlSettings * settings); //list sounds in /data/bin/sounds to be loaded 
	void playSound(int id);		
	void update();	
	string getCurrentlyPlaying(); //TODO implement
	void soundPlayer::muteUnmute();
};
