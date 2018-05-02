//
//  element.cpp
//  soulReader
//
//  Created by Dan Oved on 4/28/18.
//

#include "element.hpp"

#include "ofMain.h"


bool Element::updateAttention(bool intersects) {
    if (intersects) {
//        printf("elapsed time %lu", ofGetElapsedTimeMillis() - focusStartTime);
        if (!focusStarted) {
            //printf("starting focus\n");
            focusStarted = true;
            focusStartTime = ofGetElapsedTimeMillis();
            return false;
        } else if (ofGetElapsedTimeMillis() - focusStartTime >= FOCUS_TIME_MS) {
            focusStarted = false;
            return true;
        }
    } else {
        focusStarted = false;
        focusStartTime = 0;
        elapsedFocusStartTime = 0;
        return false;
    }
}

bool ColorElement::intersects(int gazeX, int gazeY) {
//    printf("gaze %i %i, rect %i %i %i %i\n", gazeX, gazeY, x, y, x+w, y+h);
//
//
    return gazeX >= x && gazeX <= x + w && gazeY >= y && gazeY <= y + h;
}

void ColorElement::render() {
    ofSetColor(color);
//    printf("render rect %i %i %i %i %i %i\n", rectangle.x, rectangle.y, rectangle.getTop(), rectangle.getLeft(), rectangle.getRight(), rectangle.getBottom());
    ofDrawRectangle(x, y, w, h);
}

const int minDistance = 500;

const int sections = 3;

int findValidCoord(int currentDimention, int totalSize) {
    int result;
    int sectionSize = totalSize / sections;
    int section = std::floor(currentDimention / sectionSize);
    int otherSectionIndex = std::round(ofRandom(0, sections - 1));
    if (otherSectionIndex == section) {
        otherSectionIndex++;
        if (otherSectionIndex > sections)
            otherSectionIndex = 0;
    }
    
    float min = sectionSize * otherSectionIndex;
    float max = min + sectionSize;
    return std::round(ofRandom(min, max));
}

Element* ColorElement::spawnSimilarElement(int gazeX, int gazeY) {
    int maxWidth = ofGetWidth() / 4;
    int maxHeight = ofGetHeight() / 4;
    
    int randomWidth =  std::round(ofRandom(1, maxWidth));
    int randomHeight = std::round(ofRandom(1, maxHeight));
    
    bool validXFound = false;
    int newX = findValidCoord(gazeX, ofGetWidth());
    int newY = findValidCoord(gazeY, ofGetHeight());
    
//    printf("new x and y, w, h: %i %i\n", newX, newY, randomWidth, randomHeight);
    
    int newSaturation = ofClamp(ofRandom(-20, 20) + color.getSaturation(), 0., 255.);
    int newHue = ofClamp(ofRandom(-20, 20) + color.getHue(), 0., 255.);
    
    ofColor newColor = ofColor::fromHsb(newHue, newSaturation, color.getBrightness());
    
    return new ColorElement(newColor, newX, newY, randomWidth, randomHeight);
}
