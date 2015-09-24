#pragma once
#include "ofxXmlSettings.h"

//oscMessages from and to Phone
#define PHONE_SENDER_PORT 8015
#define PHONE_IP "169.254.215.94"

class videoPlayer {
public:

	int count;

	vector<ofVideoPlayer> videos;
	stringstream video_names;
	bool something_is_playing;
	int is_playing;
	void loadVideos(ofxXmlSettings * settings);
	void update();
	void playVideo(int id);
	void stopVideo();
	void setImage();
	ofImage img;
};