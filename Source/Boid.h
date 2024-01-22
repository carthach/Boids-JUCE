//
//  Boid.h
//  myBoids1
//
//  Created by Cárthach Ó Nuanáin on 21/01/2014.
//
//

#ifndef __myBoids1__Boid__
#define __myBoids1__Boid__

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Boid : public Component
{
public:
    Boid();
    Boid(Point<float> position, Point<float> velocity, Colour color);
    ~Boid();
    
    Point<float> position;
    Point<float> velocity;
    void update(OwnedArray<Boid>& boids);
    void paint(juce::Graphics &g);

private:
    //TWEAKS
    float SEEDVELOCITY = 5.0f;
    float RADIUS = 5.0f;
    float MAXVELOCITY = 8.0f;
    float MIN_DISTANCE = 8.0f; //8.0f
    float ANIMATECOUNTERMAX = 30.0;

    Point<float> steer(OwnedArray<Boid>& boids);
    Point<float> flock(OwnedArray<Boid>& boids);
    Point<float> separate(OwnedArray<Boid>& boids);
    Point<float> align(OwnedArray<Boid>& boids);
    
    Point<float> avgPosOfOthers;
    Point<float> avgVelOfOthers;
    
    bool animateCounterIncreasing;
    float animateCounter;
    float animateInc;
    
    void clampPosition();
    void clampVelocity();
    
    Point<float> absVec2f(Point<float> theVector);
    Colour color;
    Random random;

};

#endif /* defined(__myBoids1__Boid__) */