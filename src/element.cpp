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

ofColor getSimilarColor(ofColor color) {
    float newSaturation = std::round(ofClamp(ofRandom(-20, 20) + color.getSaturation(), 0., 255.));
    float newHue = std::round(ofClamp(ofRandom(-20, 20) + color.getHue(), 0., 255.));
    float newBrightness =color.getBrightness();
    
    ofColor newColor = ofColor::fromHsb(newHue, newSaturation, newBrightness);
    
    
    printf("in similar color %f %f %f\n", newColor.getHue(), newColor.getSaturation(), color.getBrightness());

    return newColor;
}

Element* ColorElement::spawnSimilarElement(int gazeX, int gazeY) {
    int maxWidth = ofGetWidth() / 3;
    int maxHeight = ofGetHeight() / 3;
    
    int randomWidth =  std::round(ofRandom(10, maxWidth));
    int randomHeight = std::round(ofRandom(10, maxHeight));
    
    bool validXFound = false;
    int newX = findValidCoord(gazeX, ofGetWidth());
    int newY = findValidCoord(gazeY, ofGetHeight());
    
    ofColor newColor = getSimilarColor(color);
    
    return new ColorElement(newColor, newX, newY, randomWidth, randomHeight);
}

Element* WordElement::spawnSimilarElement(int gazeX, int gazeY) {
    int newX = findValidCoord(gazeX, ofGetWidth());
    int newY = findValidCoord(gazeY, ofGetHeight());
    ofColor newColor = getSimilarColor(color);

    return new WordElement(newColor, newX, newY, font, synsetKey);
}

void WordElement::render() {
    if (loaded) {
        ofEnableAlphaBlending();
        ofSetColor(0, 0, 0, 100);
        ofRectangle rect = font.getStringBoundingBox(word, x, y);
        float padding = 5;
        float expandedWidth =rect.width + padding * 2;
        float expandedHeight =rect.height + padding * 2;
        ofDrawRectangle(rect.x - padding, rect.y - padding, expandedWidth, expandedHeight);
        
        ofSetColor(color);
        font.drawString(word, x, y);
        
    }
}

float probabilityDifferentWord = 0.25;

void WordElement::loadWord() {
    float randomValue = ofRandom(0., 1.);
    
    if (randomValue <= probabilityDifferentWord) {
        ofLoadURLAsync("http://localhost:5000/random_word",
                       "async_req");
    } else {
        ofLoadURLAsync("http://localhost:5000/similar_word/" + similarWordSynsetKey,
                       "async_req");
    }
}

bool WordElement::intersects(int gazeX, int gazeY) {
    if (!loaded) return false;
    
    ofRectangle rect = font.getStringBoundingBox(word, x, y);

    return rect.inside(gazeX, gazeY);
}

// HACK - there is no way to register different instance of this
// class to handle the url response, so for now we just need to
// fake it and set the status to negative 1
#define ALREADY_HANDLED -1

void WordElement::urlResponse(ofHttpResponse & response) {
    if (loaded || response.status == ALREADY_HANDLED)
        return;
    
    if (response.status==200) {
        response.status = ALREADY_HANDLED;
        string body = response.data;
        int keyBreak = body.find(":");
        synsetKey = body.substr(0, keyBreak);
        word = body.substr(keyBreak + 1, body.length() - keyBreak - 1);
        loaded = true;
        cout << "got key and word " << synsetKey << " " << word;
    } else {
        cout << response.status << " " << response.error << endl;
    }
}
