#include "videoPlayer.h"

void videoPlayer::loadVideos(ofxXmlSettings * settings) {
	string s = settings->getValue("settings:videos", " "); //get video names from XML
	cout << "videos : " << s << endl; //print video names
	istringstream iss(s); //convert s to stringstream
	count = 0;
	do {
		string sub; 
		iss >> sub;		
		stringstream load;		
      	load << "videos/" << sub << ".mov"; //preppend and append text to create path to load video
		if (load.str() != "videos/.mov") {
			ofVideoPlayer v = *new ofVideoPlayer();
			videos.push_back(v);
			videos.at(count).loadMovie(load.str());
			//videos.at(count).loadMovie("videos/test.mov");
			cout << "loading " << "videos/"+sub+".mov" << endl;
			count++;
		} else iss >> sub;
	} while (iss); //while there are still videos to be loaded create new ofVideoPlayer
	
	cout << count << " videos loaded" << endl;
	something_is_playing = false;
	is_playing = 0;
}

void videoPlayer::playVideo(int id) {
	if (!something_is_playing && !videos.at(id).isPlaying()) {
		videos.at(id).play();
		something_is_playing = true;
		is_playing = id;
	}
}

void videoPlayer::stopVideo() {
	videos.at(is_playing).stop();
	something_is_playing = false;
}

void videoPlayer::setImage() {
	if (something_is_playing) {
		for (int i=0; i<videos.size(); i++) {			
			img.setFromPixels(videos.at(is_playing).getPixelsRef());		
		}
	}
}


void videoPlayer::update(){
	//update playing videos 
	for (int i=0; i<videos.size(); i++){
	 if (videos.at(i).isPlaying()) videos.at(i).update();
	}		

	//set new ofImage
	setImage();
	//check if video still playing
	for (int i=0; i<videos.size(); i++) {
		if (videos.at(i).isPlaying()) something_is_playing = true;				
	}   		    	

	if (videos.at(is_playing).getIsMovieDone()) something_is_playing = false;

	//img = getImage(is_playing);
}