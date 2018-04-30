#include "ofApp.h"
#include "math.h"

//--------------------------------------------------------------
void ofApp::setup(){
 focusBlurShader.load("shaders/wood.vert", "shaders/focusBlur.frag");
 shader2.load("shaders/wood");
    
 ofHideCursor();
 fboBlurOnePass.allocate(ofGetWidth(), ofGetHeight());
    
    
    fboBlurOnePass.begin();
    ofClear(0,0,0,255);
    fboBlurOnePass.end();
    
    fboBlurTwoPass.allocate(ofGetWidth(), ofGetHeight());
    ofClear(0,0,0,255);
    fboBlurTwoPass.end();
    
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
    
        elements.push_back(element);
    }
}



const int MAX_ELEMENTS = 200;

bool isValidGazeCoords(ofVec2f &gazeCoords) {
    return gazeCoords.x >= 0 && gazeCoords.x <= ofGetWidth() && gazeCoords.y >= 0 && gazeCoords.y <= ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::update(){
    int numberElements = elements.size();
    
    ofVec2f gazeCoords = ofVec2f(ofGetMouseX(), ofGetMouseY());
    
//    printf("gaze x, y, %i %i\n", ofGetMouseX(), ofGetMouseY());
    
    if (!isValidGazeCoords(gazeCoords))
        return;
    
    int highestIntersect = -1;
    
    for(int i = 0; i < numberElements; i++) {
        Element* element = elements[i];
        if (element->intersects(gazeCoords.x, gazeCoords.y)) {
            highestIntersect = std::max(i, highestIntersect);
        }
    }
    
    bool hasAddedElement = false;
    
    if (highestIntersect != -1) {
        printf("highest intersect: %i\n", highestIntersect);
    }
    
    for(int i = 0; i < numberElements; i++ ){
        Element* element = elements[i];
        if(element->updateAttention(i == highestIntersect)){
            
            hasAddedElement = true;
            Element* newElement = element->spawnSimilarElement((int)gazeCoords.x, (int)gazeCoords.y);
            elements.push_back(newElement);
//            newElement = element->spawnSimilarElement(gazeCoords);
//            elements.push_back(newElement);
        }
    }
    
    if (elements.size() > MAX_ELEMENTS) {
        delete elements.front();
        
        elements.pop_front();
        
        printf("size after delete: %lu\n", elements.size());
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
//    printf("Gaze coords %lu, %lu\n", ofGetMouseX(), ofGetMouseY());
    
    
    ofBackground(0);
    fboBlurOnePass.begin();
    
    ofClear(255,255,255, 0);
//    shader.begin();
//    shader.setUniform1f("u_time", ofGetElapsedTimef());
//    shader.setUniform2f("u_resolution",ofGetWidth(), ofGetHeight());
//    shader.setUniform2f("u_mouse", ofGetMouseX(), ofGetMouseY());
//    ofDrawRectangle(0, 0, ofGetWidth() / 2, ofGetHeight() / 2);
//    shader.end();
    
    for(unsigned int i = 0; i < elements.size(); i++) {
        elements[i]->render();
    }
    fboBlurOnePass.end();
    //ofSetColor(ofColor::white);
    
    fboBlurTwoPass.begin();
    
    focusBlurShader.begin();
    focusBlurShader.setUniform1f("u_time", ofGetElapsedTimef());
    focusBlurShader.setUniform2f("u_resolution",ofGetWidth(), ofGetHeight());
    focusBlurShader.setUniform2f("u_mouse", ofGetMouseX(), ofGetMouseY());
    
    focusBlurShader.setUniformTexture("outputGraphic", fboBlurOnePass.getTextureReference(), 1);
    
    fboBlurOnePass.draw(0, 0);
    
    focusBlurShader.end();
    fboBlurTwoPass.end();
    
    fboBlurTwoPass.draw(0, 0);
    
    

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
