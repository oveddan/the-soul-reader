#include "ofApp.h"
#include "math.h"



//--------------------------------------------------------------
void ofApp::setup(){
    //     bool connected = client.setup("localhost", 5555);
    
    vidGrabber.setPixelFormat(OF_PIXELS_BGR);
    vidGrabber.setup(1024,768);
    colorImg.allocate(1024,768);
    
    font.load("fonts/NewsCycle-Bold.ttf", 32);
    
    //  context = zmqMakeContext(2);
    //  socket = zmqMakeSocket(*context, ZMQ_PUSH);
    //  socket->bind("tcp://localhost:5555");
    
    focusBlurShader.load("shaders/wood.vert", "shaders/focusBlur.frag");
    
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
        int width = ofRandom(0, ofGetWidth() / 4);
        int height = ofRandom(0, ofGetHeight() / 4);
        int x = ofRandom(0, ofGetWidth());
        int y =  ofRandom(0, ofGetHeight());
        Element* element = new ColorElement(color, x, y, width, height);
        
        elements.push_back(element);
    }
    
    Element* wordElement = new WordElement(baseColor, 500, 500, font, "n10569926");
    elements.push_back(wordElement);
}

const int MAX_ELEMENTS = 100;

bool isValidGazeCoords(ofVec2f &gazeCoords) {
    return gazeCoords.x >= 0 && gazeCoords.x <= ofGetWidth() && gazeCoords.y >= 0 && gazeCoords.y <= ofGetHeight();
}

int focusDuration = 0;

//--------------------------------------------------------------
void ofApp::update(){
    
    vidGrabber.update();
    bool newFrame = vidGrabber.isFrameNew();
    
    int numberElements = elements.size();
    
    if (newFrame) {
        colorImg.setFromPixels(vidGrabber.getPixels());
    }
    
    ofVec2f gazeCoords = ofVec2f(ofGetMouseX(), ofGetMouseY());
    
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
    
    currentFocusedElement = -1;
    for(uint16_t i = 0; i < numberElements; i++ ){
        Element* element = elements[i];
        bool isHighestInteract =i == highestIntersect;
        
        if (isHighestInteract) {
            currentFocusedElement = i;
        }
        
        if(element->updateAttention(isHighestInteract)){
            
            hasAddedElement = true;
            Element* newElement = element->spawnSimilarElement((int)gazeCoords.x, (int)gazeCoords.y);
            elements.push_back(newElement);
            newElement = element->spawnSimilarElement((int)gazeCoords.x, (int)gazeCoords.y);
            elements.push_back(newElement);
        }
    }
    
    if (elements.size() > MAX_ELEMENTS) {
        Element* front = elements.front();
        
        elements.pop_front();
        
        if (currentFocusedElement == 0) {
            Element* otherFront = elements.front();
            elements.pop_front();
            delete otherFront;
            
            elements.push_front(front);
        } else {
            currentFocusedElement--;
            delete front;
        }
        
        printf("size after delete: %lu\n", elements.size());
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    fboBlurOnePass.begin();
    
    ofClear(255,255,255, 0);
    
    colorImg.draw(0, 0);
    
    for(unsigned int i = 0; i < elements.size(); i++) {
        elements[i]->render();
    }
    fboBlurOnePass.end();
    
    fboBlurTwoPass.begin();
    
    focusBlurShader.begin();
    focusBlurShader.setUniform1f("u_time", ofGetElapsedTimef());
    focusBlurShader.setUniform2f("u_resolution",ofGetWidth(), ofGetHeight());
    focusBlurShader.setUniform2f("u_mouse", ofGetMouseX(), ofGetMouseY());
    
    if (currentFocusedElement > 0 && currentFocusedElement < elements.size()) {
        Element* focusedElement = elements[currentFocusedElement];
        focusBlurShader.setUniform1f("u_focusDuration", focusedElement->currentFocusIntervalDuration());
        focusBlurShader.setUniform1f("u_totalFocusDuration", focusedElement->totalFocusTime());
    } else {
        focusBlurShader.setUniform1f("u_focusDuration", 0);
        focusBlurShader.setUniform1f("u_totalFocusDuration", 0);
    }
    
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
