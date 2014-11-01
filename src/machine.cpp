#include "machine.h"

void machine::setup()
{
	x_offset = 74;
	y_offset = 164;
	camWidth = 800;
	camHeight = 640;		
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(1);
	vidGrabber.setDesiredFrameRate(120);
	vidGrabber.initGrabber(camWidth,camHeight);			
	ofSetFullscreen(true);
	ofSetVerticalSync(true);
		
	pitch = 0;
	yaw = 0;
	roll = 0;
	    
    //There are the parameters for the polynomial warp function to correct for the Oculus Rift and Webcam Lenses
    K0 = 1.0;
    K1 = 5.74;
    K2 = 0.27;
    K3 = 0.0;
    _x = 0.0f;
    _y = 0.0f;
    _w = 1.0f;
    _h = 1.0f;
    as = 640.0f/480.0f;
	DistortionXCenterOffset = 90;
	        
    hmdWarpShader.load("shaders/HmdWarpExp");
	endTimer = 1;		
}

void machine::update() {
	vidGrabber.update();	    	
}

void machine::drawVideo() {	
	ofPushMatrix();				
		ofTranslate(camWidth/2, camHeight/2, 0);//move pivot to centre
		ofRotate(90, 0, 0, 1);//rotate from centre					
		vidGrabber.draw(y_offset-camWidth/2,x_offset-880); //draw left
		vidGrabber.draw(y_offset-camWidth/2,-x_offset-camHeight/2); //draw right		
	ofPopMatrix();		
}

void machine::drawOverlay(){
	ofVec2f mine = ofVec2f(pitch,yaw);
	ofVec2f their = ofVec2f(rx_pitch,rx_yaw);

	if (mine.distance(their)>0.7) {        
		time_no_sync = ofGetElapsedTimef();        		        		    
		ofSetColor(255,0,0);
		ofCircle(x_offset+camWidth/2-200*(rx_yaw),y_offset+camHeight/2-300*(rx_pitch),5);
		ofCircle(-x_offset+960-200*(rx_yaw),y_offset+camHeight/2-300*(rx_pitch),5);
            
		//headtrack of this computer (1st person headtracking and draw)
		ofSetColor(0,255,0);
		ofNoFill();
		ofCircle(x_offset+camWidth/2-200*(yaw),y_offset+camHeight/2-300*(pitch),12);
		ofCircle(-x_offset+960-200*(yaw),y_offset+camHeight/2-300*(pitch),12);
		ofFill();

		///COMMENT THIS IF TO SHOW "OUT OF SYNC MESSAGE" CONSTANTLY
		     if  (time_no_sync < endTimer) {
		ofSetColor(255,0,0);
		ofDrawBitmapString("Out of sync",(camWidth/2+x_offset)-40, camHeight/2+y_offset );
		ofDrawBitmapString("Out of sync",(camWidth*1.5-x_offset)-40,camHeight/2+y_offset);
		     }
        
	}
        
	//if the users are looking close enough    
	else {       
		time_sync = ofGetElapsedTimef();
			if (ofGetElapsedTimef() < endTimer) {
				ofSetColor(0,0,255);
				ofDrawBitmapString("Synchronized",(camWidth/2+x_offset)-45, camHeight/2+y_offset);
				ofDrawBitmapString("Synchronized",(camWidth*1.5-x_offset)-45,camHeight/2+y_offset);
				}
		}    	
}

void machine::calibrate() {
	pitch_cal=pitch;
	yaw_cal=yaw;
	roll_cal=roll;
}

machine::machine(void)
{
}


machine::~machine(void)
{
}

/*hmdWarpShader.begin();
	hmdWarpShader.setUniformTexture("tex", vidGrabber.getTextureReference(), 0);
    hmdWarpShader.setUniform2f("LensCenter", DistortionXCenterOffset, 0 );
    hmdWarpShader.setUniform2f("ScreenCenter", _x + _w*1.0f, _y + _h*1.0f );
    hmdWarpShader.setUniform2f("Scale", (_w/1.0f) * 1.0f, (_h/1.0f) * 1.0f * as );
    hmdWarpShader.setUniform2f("ScaleIn", (1.0f/_w), (1.0f/_h) / as );
    hmdWarpShader.setUniform4f("HmdWarpParam", K0, K1, K2, K3 );*/
	
	//hmdWarpShader.end();

		/*
    float planeScale = 0.75;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;
    int planeGridSize = 20;
    int planeColumns = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
 */
    //ofPlanePrimitive plane = *(new ofPlanePrimitive());		
	//plane.set(planeWidth, planeHeight, planeColumns, planeRows, OF_PRIMITIVE_TRIANGLES);
	//plane.mapTexCoordsFromTexture(vidGrabber.getTextureReference);		
	//ofRotate(90,0,0,1);
	
	
	/*
	glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(0,0,0);
    glTexCoord2f(0,0); glVertex3f(640,0,0);
    glTexCoord2f(0,1); glVertex3f(640,800,0);
    glTexCoord2f(1,1); glVertex3f(0,800,0);
    glEnd();*/	
