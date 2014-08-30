/*

Based on code by Memo from this thread:
http://forum.openframeworks.cc/index.php?topic=1687.0

*/

#include "ofMain.h"    

typedef struct { 
    string fileName;    
    ofPixels image;
} QueuedImage;

class ofxImageSequenceRecorder : public ofThread {    
public:    
  
    int counter;  
    queue<QueuedImage> q;
    string prefix;
    string format;
      
    ofxImageSequenceRecorder(){  
        counter=0;  
        
    }  
    
    void setPrefix(string pre){
        prefix = pre;
    }
    
    void setFormat(string fmt){
        format = fmt;
    }
       
    void threadedFunction() {    
        while(isThreadRunning()) {
            if(!q.empty()){
                QueuedImage i = q.front();
                ofSaveImage(i.image, i.fileName);
                q.pop();
            }
        }
        

        
    }   
    
    void addFrame(ofImage &img){
        addFrame(img.getPixelsRef());
    }
    
    void addFrame(ofVideoGrabber &cam){
        addFrame(cam.getPixelsRef());
    }
    
    void addFrame(ofVideoPlayer &player){
        addFrame(player.getPixelsRef());
    }
        
    void addFrame(ofPixels imageToSave) {  

        
        
        char fileName[100]; 
        sprintf(fileName,  "%s%.4i.%s" , prefix.c_str(), counter, format.c_str());     
        counter++;   
        
        QueuedImage qImage;
        
        qImage.fileName = fileName;    
        qImage.image = imageToSave; 
        
        q.push(qImage);
        
    }    
};  