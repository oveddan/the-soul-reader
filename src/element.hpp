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
    
    float weight;
    int x;
    int y;
    
protected:
    uint64_t focusStartTime;
    bool focusStarted;
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

#endif /* element_hpp */
