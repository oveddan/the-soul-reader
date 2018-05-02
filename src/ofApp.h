#pragma once

#include "ofMain.h"
#include "element.hpp"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
    
public:
    ofApp() : currentFocusedElement(-1) {};
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
    
protected:
    ofVideoGrabber vidGrabber;
    ofxCvColorImage            colorImg;
    void updateProfile();
    ofShader focusBlurShader;
    ofImage image;
    uint16_t currentFocusedElement;
    
    std::deque<Element *> elements;
    
    ofFbo fboElementsPass;
    
    ofFbo fboBlurOnePass;
    ofFbo fboBlurTwoPass;
    
};
