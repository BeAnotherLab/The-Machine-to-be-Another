/*
 *  ofxBlurShader.h
 *
 *  Created by Rick Companje on 25-10-10.
 *  updated on 15-07-11 for of007
 *  Based on the blurShader example by Theo
 *
 */

#pragma once 

#include "ofMain.h"

class ofxBlurShader {
public:
	
    ofxBlurShader(); 
    
	void setup(float w, float h);
	void begin(int amount=5, int iterations=2);
	void end();
	void draw(float x=0, float y=0, float w=0, float h=0);
    
    int iterations, amount;
    bool enabled;
    
    ofFbo fbo1,fbo2;
    ofShader hShader, vShader;

private:
    bool initialized;
};

