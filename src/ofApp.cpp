#include "ofApp.h"
ofFbo fbo, spFbo;

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------


bool drawCircles = true;
bool drawRects = false;
bool expressionSet = false;

int nObjects = 50;
int rectColor = 0xD62D14;
int circleColor = 0xA2D927;


void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    
//    //FBO
//    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F); // higher precision alpha (no artifacts)
//    spFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
//    
//    fbo.begin();
//    ofClear(255,255,255, 0);
//    fbo.end();
//    
//    spFbo.begin();
//    ofClear(255,255,255, 0);
//    spFbo.end();
    
    
    
    //detect the Mask Object
    ofSetFrameRate(120);
    
    finder.setup("haarcascade_frontalface_default.xml");
    finder.setPreset(ObjectFinder::Fast);
    finder.getTracker().setSmoothingRate(.3);
    cam.setup(1280, 720);


    ofEnableAlphaBlending();
    
    //FontLoad
    myFont1.load("FuturaPTMedium.otf", 20);
    myFont2.load("BebasNeueBold.otf", 20);
    
    //SoundLoad freesound.com
//    player1.load("TribalCelebration.wav");
//    player2.load("TribalDeath.wav");
//    player3.load("TribalWar.wav");
//    player4.load("TribalDailyLife.wav");
    


    //Initialize box2D
    box2d.init();
    box2d.setGravity(0,0);
    box2d.createBounds();
    box2d.setFPS(30.0);
    box2d.registerGrabbing();
    
    //init Camera //
    cam.initGrabber(1280, 720);
    tracker.setup();
    tracker.setRescale(.5);
    
    classifier.load("expressions");
    
    for(int i=0; i<=nObjects; i++){
        int xPos = ofRandom(100, 380);
        int yPos = ofRandom(100, 540);
        int rad = ofRandom(1,30);
        //circles
        circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), xPos, yPos, rad);
        //rectangles
        rects.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
        rects.back().get()->setPhysics(3.0, 0.53, 0.1);
        rects.back().get()->setup(box2d.getWorld(), xPos, yPos, rad*2, rad*2);
    }

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    box2d.update();
    cam.update();
    if(cam.isFrameNew()) {
       //detect the object
        finder.update(cam);
        if(tracker.update(toCv(cam))) {
            classifier.classify(tracker);
           
        }
    }
    
    
    
    
    //Change gravity based on expression
    //int expressions = classifier.size();
    int expression = classifier.getPrimaryExpression();
    cout<<expression;
    
    if (expression==0){
        mask.load("mask5.png");
        box2d.setGravity(0,-20);
        drawRects = false;
        drawCircles = true;
        circleColor = 0xA2D927;
        cout<<"up";
        // Play this music
//        player4.stop();
        player1.load("TribalDailyLife.wav");
        player1.play();
        
        
    } else if (expression==1){
        mask.load("mask4.png");
        box2d.setGravity(0,20);
        drawCircles = false;
        drawRects = true;
        rectColor = 0xD62D14;
        cout<<"down";
//        player1.stop();
        player2.load("TribalCelebration.wav");
        player2.play();

        
    } else if (expression==2){
        mask.load("mask2.png");
        box2d.setGravity(20,0);
        drawCircles = false;
        drawRects = true;
        rectColor = 0x612851;
        cout<<"down";
//        player2.stop();
        player3.load("TribalWar.wav");
        player3.play();

       
    } else if (expression==3) {
        mask.load("mask3.png");
        box2d.setGravity(-20,0);
        drawCircles = true;
        drawRects = false;
        circleColor = 0x2AA9D9;
        cout<<"down";
//        player3.stop();
        player4.load("TribalDeath.wav");
        player4.play();
       
     }

  
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    

    box2d.drawGround();
    
  
    //Cam tracker
    ofSetColor(255);
    cam.draw(0, 0);
    tracker.draw();
    
    
    
    if (drawCircles==true){
        for(int i=0; i<circles.size(); i++){
//          fbo.begin();
            ofFill();
//            ofSetColor(49,178,43);
            ofSetHexColor(circleColor);
            circles[i].get()->draw();
//          fbo.end();
    
        }
    }
    
//    spFbo.begin();  // write the existing fbo to the scratchpad but a bit smaller
//    ofSetColor(255,255,255);  // always set the color before drawing the fbo
//    fbo.draw(10, 10, ofGetWidth()-20, ofGetHeight()-20);
//    spFbo.end();
//    
//    fbo = spFbo;    // write the scratchpad back to the original
//    
//    ofSetColor(255,255,255);  // always set the color before drawing the fbo
//    fbo.draw(0,0);
//    
    
    
    if (drawRects==true){
        for(int i=0; i<rects.size(); i++){
            ofFill();
//          ofSetHexColor(rectColor);
            rects[i].get()->draw();
            
        }
    }

    
    for(int i = 0; i < finder.size(); i++) {
        ofRectangle object = finder.getObjectSmoothed(i);
        mask.setAnchorPercent(.5, .5);
        float scaleAmount = .85 * object.width / mask.getWidth();
        ofPushMatrix();
        ofTranslate(object.x + object.width / 2., object.y + object.height * .42);
        ofScale(scaleAmount, scaleAmount);
        mask.draw(0,0);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(object.getPosition());
        ofDrawBitmapStringHighlight(ofToString(finder.getLabel(i)), 0, 0);
        ofDrawLine(ofVec2f(), toOf(finder.getVelocity(i)) * 10);
        ofPopMatrix();
    }
    
    //Page Title
    
    ofPushMatrix();
    ofSetColor(255);
    myFont2.drawString("Native American Tribal Masks Gestural Experiment", ofGetWidth()/2-600, ofGetHeight()/2-300);
    
    myFont1.drawString("War", ofGetWidth()/2+480, ofGetHeight()/2);
    myFont1.drawString("Celebration", ofGetWidth()/2-50, ofGetHeight()/2-200);
    myFont1.drawString("Daily Life", ofGetWidth()/2-50, ofGetHeight()/2+300);
    myFont1.drawString("Death", ofGetWidth()/2-600, ofGetHeight()/2);
    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
//    if (key =='a'){
//        classifier.addExpression();
//        classifier.addSample(tracker);
//        classifier.save("expressions");
//        expressionSet = true;
//    }

//        if (key =='a'){
//            player1.play();
//        }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
