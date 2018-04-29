//
//  element.cpp
//  soulReader
//
//  Created by Dan Oved on 4/28/18.
//

#include "element.hpp"

#include "ofMain.h"


void Element::updateAttention(ofVec2f gaze) {
    if (intersects(gaze)) {
        if (!focusStarted) {
            focusStarted = true;
            focusStartTime = ofGetElapsedTimeMillis();
        } else {
            focusStarted = false;
        }
    }
}

bool Element::capturedFocus() {
    return focusStarted && ofGetElapsedTimeMillis() - focusStartTime >= FOCUS_TIME_MS;
}

void Element::resetAttention() {
    focusStarted = false;
}

bool ColorElement::intersects(ofVec2f gaze) {
    return gaze.x >= l() && gaze.x <= r() && gaze.y >= b() && gaze.y <= t();
}

void ColorElement::render() {
    ofSetColor(color);
    printf("%i %i %i %i\n", l(), t(), w, h);
    ofDrawRectangle(l(), t(), w, h);
}

int minDistance = 300;

int findValidCoord(int currentDimention, int max) {
    if (currentDimention > minDistance) {
        return std::round(ofRandom(0, currentDimention - minDistance));
    } else {
        return std::round(ofRandom(currentDimention + minDistance, max));
    }
}

Element* ColorElement::spawnSimilarElement(ofVec2f gaze) {
    int maxWidth = ofGetWidth() - w / 2;
    int maxHeight = ofGetHeight() - h / 2;
    
    int randomWidth =  std::round(ofRandom(1, maxWidth));
    int randomHeight = std::round(ofRandom(1, maxHeight));
    
    bool validXFound = false;
    int x = findValidCoord(x, ofGetWidth());
    int y = findValidCoord(y, ofGetHeight());
    
    return new ColorElement(color, x, y, randomWidth, randomHeight);
}
