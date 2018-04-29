#include "ofApp.h"
#include "math.h"

//--------------------------------------------------------------
void ofApp::setup(){
 shader.load("shaders/wood");
 shader2.load("shaders/wood");
 fboBlurOnePass.allocate(ofGetWidth(), ofGetHeight());
    
    fboBlurOnePass.begin();
    ofClear(0,0,0,255);
    fboBlurOnePass.end();
    int randomHue = std::round(ofRandom(0, 255));
    int randomSaturation =std::round(ofRandom(100, 255));
    ofColor baseColor = ofColor::fromHsb(randomHue, randomSaturation, 255);
    
    int numberOfColors = 3;
    
    for(int i = 0; i < numberOfColors; i++) {
        int newHue = randomHue + ( 255 / numberOfColors) * i;
        if (newHue > 255) { newHue = newHue - 255; }
        ofColor color = ofColor::fromHsb(newHue, randomSaturation, 255);
        int width = ofGetWidth() / numberOfColors;
        int height = ofGetHeight();
        int x= width * i + width/2;
        int y = height/2;
        Element* element = new ColorElement(color, x, y, width, height);
    
        colorElements.push_back(element);
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

ofVec2f getGazeCoords() {
    return ofVec2f(ofGetMouseX(), ofGetMouseY());
}

void ofApp::updateProfile() {
    int numberElements = colorElements.size();
    ofVec2f gazeCoords = getGazeCoords();

    for(int i = 0; i < numberElements; i++) {
        Element* element = colorElements[i];
        element->updateAttention(gazeCoords);
        
        if (element->capturedFocus()) {
            Element* newElement = element->spawnSimilarElement(gazeCoords);
            colorElements.push_back(newElement);
            element->resetAttention();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    void updateProfile();
    
    ofBackground(0);
    fboBlurOnePass.begin();
//    shader.begin();
//    shader.setUniform1f("u_time", ofGetElapsedTimef());
//    shader.setUniform2f("u_resolution",ofGetWidth(), ofGetHeight());
//    shader.setUniform2f("u_mouse", ofGetMouseX(), ofGetMouseY());
//    ofDrawRectangle(0, 0, ofGetWidth() / 2, ofGetHeight() / 2);
//    shader.end();
    
    for(unsigned int i = 0; i < colorElements.size(); i++) {
        colorElements[i]->render();
    }
    fboBlurOnePass.end();
    
    fboBlurOnePass.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
