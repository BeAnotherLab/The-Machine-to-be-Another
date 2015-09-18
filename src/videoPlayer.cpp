#include "videoPlayer.h"

void videoPlayer::loadVideos(ofxXmlSettings * settings) {
	string s = settings->getValue("settings:videos", " "); //get video names from XML
	cout << "videos : " << s << endl; //print video names
	istringstream iss(s); //convert s to stringstream
	count = 0;
	do {
		string sub; 
		iss >> sub;
		ofVideoPlayer v = *new ofVideoPlayer();
		videos.push_back(v);
		stringstream load;
		load << "videos/" << sub << ".mov"; //preppend and append text to create path to load video
		videos.at(count).loadMovie(load.str());
		cout << "loading " << "videos/"+sub+".mov" << endl;
		count++;
	} while (iss); //while there are still videos to be loaded create new ofVideoPlayer

	cout << count << " videos loaded" << endl;

	
}

void videoPlayer::playVideo(int id) {
	if (!something_is_playing && !videos.at(id).isPlaying()) {
		videos.at(id).play();
		stringstream play;
	}
}

void videoPlayer::update(){
	//update playing videos 
	for (int i=0; i<videos.size(); i++){
	 if (videos.at(i).isPlaying()) videos.at(i).update();
	}
}