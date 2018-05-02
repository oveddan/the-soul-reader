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
        if (!focusStarted) {
            focusStarted = true;
            focusStartTime = ofGetElapsedTimeMillis();
        } if (!intervalStarted) {
            intervalStarted = true;
            focusIntervalStartTime = ofGetElapsedTimeMillis();
        }
        
        if (ofGetElapsedTimeMillis() - focusIntervalStartTime >= FOCUS_TIME_MS) {
            intervalStarted = false;
            focusIntervalStartTime = ofGetElapsedTimeMillis();
            return true;
        } else
            return false;
    } else {
        focusStarted = false;
        intervalStarted = false;
        focusIntervalStartTime = 0;
        focusStartTime = 0;
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
    if (focusStarted) {
        ofColor ligtherColor = ofColor::fromHsb(
                                                color.getHue(),
                                                color.getSaturation() + 30,
                                                color.getBrightness()
                                                );
        ofSetColor(ligtherColor);
    } else {
        ofSetColor(color);
    }
    
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
    int maxWidth = ofGetWidth() / 3;
    int maxHeight = ofGetHeight() / 3;
    
    int randomWidth =  std::round(ofRandom(10, maxWidth));
    int randomHeight = std::round(ofRandom(10, maxHeight));
    
    bool validXFound = false;
    int newX = findValidCoord(gazeX, ofGetWidth());
    int newY = findValidCoord(gazeY, ofGetHeight());
    
    //    printf("new x and y, w, h: %i %i\n", newX, newY, randomWidth, randomHeight);
    
    int newSaturation = ofClamp(ofRandom(-20, 20) + color.getSaturation(), 0., 255.);
    int newHue = ofClamp(ofRandom(-20, 20) + color.getHue(), 0., 255.);
    
    ofColor newColor = ofColor::fromHsb(newHue, newSaturation, color.getBrightness());
    
    return new ColorElement(newColor, newX, newY, randomWidth, randomHeight);
}
