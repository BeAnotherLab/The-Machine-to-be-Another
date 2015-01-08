#include "soundPlayer.h"

void soundPlayer::loadSounds(string s) //file names passed as parameters will be loaded from sounds folder
{
	//load sounds
	cout << "sounds : " << s << endl;
    istringstream iss(s);
	count = 0;
    do
    {		
        string sub;
        iss >> sub;        
		ofSoundPlayer so = *new ofSoundPlayer();
		sounds.push_back(so);		
		stringstream load;
		load << "sounds/" << sub << ".mp3";		
		sounds.at(count).loadSound(load.str());					
		cout << "loading " << "sounds/"+sub+".mp3" << endl;
		count++;
    } while (iss); //while there are still sounds to be loaded create new ofSoundPlayer for each one of them
	cout << count << " sounds loaded " << endl;
	
	music_volume = 1.f;
	sounds.at(0).play(); //Initialize music on startup	

}

void soundPlayer::playSound(int id) {
	//play sound i if button i was pressed on phone or on keyboard
		if (!something_is_playing && !sounds.at(id).getIsPlaying()){						
			sounds.at(id).play();				
			stringstream play;
			play << "sound " << id << " is playing" << endl;

			//then send to phone to display which sound is playing
		}			
}

void soundPlayer::update() {	
    // update the sound playing system:
	ofSoundUpdate();
	
	something_is_playing = false;  
	stringstream is_playing;	

	//set something_is_playing
	for (int i=1; i<sounds.size(); i++) {
		if (sounds.at(i).getIsPlaying()) something_is_playing = true;
		sounds[0].setVolume(music_volume/2); //simple sidechaining
	}   		
    
	if (!something_is_playing) {		
        sounds[0].setVolume(music_volume);        
		//TODO send "nothing is playing" to phone
    }       
}

void soundPlayer::muteUnmute(){
	if (music_volume == 1.f) {
		music_volume = 0.f;
	}	
	else if (music_volume == 0.f) {
		music_volume = 1.f;
	}
}