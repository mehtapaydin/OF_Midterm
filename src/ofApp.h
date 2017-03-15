#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxFaceTracker.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    
//	void urlResponse(ofHttpResponse & response);
    
    ofxBox2d box2d;
    vector <ofPtr<ofxBox2dCircle> > circles;
    vector <ofPtr<ofxBox2dRect> > rects;

    
    ofVideoGrabber cam;
    ofxFaceTracker tracker;
    ExpressionClassifier classifier;
    
    //detect the object
    ofxCv::ObjectFinder finder;

    ofImage mask;
    ofTrueTypeFont myFont1;
    ofTrueTypeFont myFont2;

    ofSoundPlayer player1;
    ofSoundPlayer player2;
    ofSoundPlayer player3;
    ofSoundPlayer player4;
    
    
    
    
};
