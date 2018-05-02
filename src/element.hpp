//
//  element.hpp
//  soulReader
//
//  Created by Dan Oved on 4/28/18.
//

#ifndef element_hpp
#define element_hpp

#include <stdio.h>
#include "ofMain.h"

#define FOCUS_TIME_MS 700

class Element {
public:
    Element(int x, int y): x(x), y(y) {};
    virtual void animate() {};
    virtual void render() = 0;
    bool updateAttention(bool intersects);
    virtual Element* spawnSimilarElement(int gazeX, int gazeY) = 0;
    virtual bool intersects(int gazeX, int gazeY) = 0;
    virtual ~Element() {}
    uint32_t currentFocusIntervalDuration() { return ofGetElapsedTimeMillis() - focusIntervalStartTime; };
    uint64_t totalFocusTime() { return ofGetElapsedTimeMillis() - focusStartTime; };
    
    float weight;
    int x;
    int y;
    
protected:
    uint64_t focusIntervalStartTime;
    uint64_t focusStartTime;
    bool focusStarted;
    bool intervalStarted;
};

class ColorElement : public Element {
public:
    ColorElement(ofColor color, int cx, int cy, int w, int h): color(color),
    w(w), h(h), Element(cx -w/2, cy - h/2) {};
    
    void render();
    Element* spawnSimilarElement(int gazeX, int gazeY);
    bool intersects(int gazeX, int gazeY);
    
    ofColor color;
    
protected:
    int w;
    int h;
    ofRectangle rectangle;
};

class WordElement: public Element {
public:
    WordElement(ofColor color, int cx, int cy, const ofTrueTypeFont font, const string similarWordSynsetKey) : font(font), similarWordSynsetKey(similarWordSynsetKey), loaded(false), Element(cx, cy) {
        
        ofRegisterURLNotification(this);
        loadSimilarWord();
    }
    
    void render();
    
    Element* spawnSimilarElement(int gazeX, int gazeY);
    bool intersects(int gazeX, int gazeY);
    
    void loadSimilarWord();
    
    void urlResponse(ofHttpResponse & response);
    
protected:
    ofColor color;
    
    string word;
    string synsetKey;
    bool loaded;
    const ofTrueTypeFont font;
    
    const string similarWordSynsetKey;
};

#endif /* element_hpp */
