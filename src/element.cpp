//
//  element.cpp
//  soulReader
//
//  Created by Dan Oved on 4/28/18.
//

#include "element.hpp"

#include "ofMain.h"


void Element::updateAttention(bool intersects) {
    if (intersects) {
        if (!focusStarted) {
            //printf("starting focus\n");
            focusStarted = true;
            focusStartTime = ofGetElapsedTimeMillis();
        }
    } else {
        focusStarted = false;
        focusStartTime = 0;
    }
}

bool Element::capturedFocus() {
    return focusStarted && ofGetElapsedTimeMillis() - focusStartTime >= FOCUS_TIME_MS;
}

void Element::resetAttention() {
    focusStarted = false;
}

bool ColorElement::intersects(ofVec2f gaze) {
    return gaze.x >= l() && gaze.x <= r() && gaze.y <= b() && gaze.y >= t();
}

void ColorElement::render() {
    ofSetColor(color);
    ofDrawRectangle(l(), t(), w, h);
}

int minDistance = 500;

int findValidCoord(int currentDimention, int max) {
    int result;
    if (currentDimention > max/2) {
        result= std::round(ofRandom(0, currentDimention - minDistance));
    } else {
        result= std::round(ofRandom(currentDimention + minDistance, max));
    }
    
    return std::min(std::max(result, 0), max);
}

Element* ColorElement::spawnSimilarElement(ofVec2f gaze) {
    int maxWidth = ofGetWidth() - w / 2;
    int maxHeight = ofGetHeight() - h / 2;
    
    int randomWidth =  std::round(ofRandom(1, maxWidth));
    int randomHeight = std::round(ofRandom(1, maxHeight));
    
    bool validXFound = false;
    int newX = findValidCoord(gaze.x, ofGetWidth());
    int newY = findValidCoord(gaze.y, ofGetHeight());
    
    printf("new x and y: %i %i\n", newX, newY);
    
    int newSaturation = ofClamp(ofRandom(-20, 20) + color.getSaturation(), 0., 255.);
    int newHue = ofClamp(ofRandom(-20, 20) + color.getHue(), 0., 255.);
    
    ofColor newColor = ofColor::fromHsb(newHue, newSaturation, color.getBrightness());
    
    return new ColorElement(newColor, newX, newY, randomWidth, randomHeight);
}
