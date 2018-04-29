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

#define FOCUS_TIME_MS 300

class Element {
public:
    Element(int x, int y): x(x), y(y) {};
    virtual void animate() {};
    virtual void render() = 0;
    void updateAttention(ofVec2f gaze);
    bool capturedFocus();
    virtual Element* spawnSimilarElement(ofVec2f gaze) = 0;
    void resetAttention();
    
    float weight;
    int x;
    int y;
    
protected:
    virtual bool intersects(ofVec2f gaze) {};
    uint64_t focusStartTime;
    bool focusStarted;
};

class ColorElement : public Element {
public:
    ColorElement(ofColor color, int x, int y, int w, int h): color(color),
     w(w), h(h), Element(x, y) {};
    
    void render();
    Element* spawnSimilarElement(ofVec2f gaze);
    
    ofColor color;
    
    int l() { return x - w/2;}
    int r() { return x + w/2;}
    int t() { return y + h/2;}
    int b() { return y - h/2;}
    
protected:
    int w;
    int h;
    bool intersects(ofVec2f gaze);
    
};

#endif /* element_hpp */
