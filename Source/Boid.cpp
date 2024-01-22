//
//  Boid.cpp
//  myBoids1
//
//  Created by Cárthach Ó Nuanáin on 21/01/2014.
//
//

#include "Boid.h"

//Default Constructor - randomise the lot
Boid::Boid()
{
//    position = Point<float>(random.nextInt(getWidth()),random.nextInt(getHeight()));
//    velocity = Point<float>(random.nextFloat()*SEEDVELOCITY,random.nextFloat()*SEEDVELOCITY);
//    color = Colour((int)random.nextInt(255), (int)random.nextInt(255), (int)random.nextInt(255),(float)180.0f);
    
    position = Point<float>(random.nextInt(800),random.nextInt(600));
    velocity = Point<float>(random.nextFloat()*SEEDVELOCITY,random.nextFloat()*SEEDVELOCITY);

    //    color = Colour((int)random.nextInt(255), (int)random.nextInt(255), (int)random.nextInt(255),(float)180.0f);
    
    animateCounter = 0.0f;
    animateInc = random.nextFloat() * 10.0f;
    animateCounterIncreasing = true;

}

Boid::Boid(Point<float> position, Point<float> velocity, Colour colour)
{
    this->position = position;
    this->velocity = velocity;
    this->color = color;
}

Boid::~Boid()
{
    
}

void Boid::update(OwnedArray<Boid>& boids)
{
    //Get the key changes
    Point<float> change1 = flock(boids);
    Point<float> change2 = separate(boids);
    Point<float> change3 = align(boids);
    Point<float> change4 = steer(boids);

    //Apply changes
//    velocity = velocity + change1 + change2 + change3;
    
    //Mouse
    velocity = velocity + change1 + change2 + change3 + change4;

    position = position + velocity;

    //Clamp bounds and speed
    clampPosition();
    clampVelocity();
}

void Boid::clampPosition()
{
    if(position.x >= getWidth())
    {
        position.x = getWidth() - 5.0f;
        velocity.x *= -1.0f;
    }
    if(position.x <= 0.0f)
    {
        position.x = 5.0f;
        velocity.x *= -1.0f;
    }
    if(position.y >= getHeight())
    {
        position.y = getHeight() - 5.0f;
        velocity.y *= -1.0f;
    }
    if(position.y <= 0.0f)
    {
        position.y = 5.0f;
        velocity.y *= -1.0f;
    }
}

void Boid::clampVelocity()
{
    if(abs(velocity.x) >= MAXVELOCITY)
    {
        int signX = (velocity.x < 0) ? -1 : (velocity.x > 0);
        velocity.x = MAXVELOCITY * (float)signX;
    }
    if(abs(velocity.y) >= MAXVELOCITY)
    {
        int signY = (velocity.y < 0) ? -1 : (velocity.y > 0);
        velocity.y = MAXVELOCITY * (float)signY;
    }
}
void Boid::paint(juce::Graphics &g)
{
//    ofSetColor(color);
    
//    g.fillAll(Colours::white);
    g.setColour(Colour::greyLevel(0.5).withAlpha(0.4f));
    
    
//DRAW CIRCLES
//    ofCircle(position.x, position.y, RADIUS);
//    float theta = std::atan2(position.y, position.x) ;
    float theta = std::atan2(velocity.y, velocity.x);

    g.drawEllipse(position.x, position.y, 25, 25, 1);
    Path myPath;
    
    myPath.addTriangle(0, 0, 0, 20, -20-animateCounter, 25+animateCounter);
    myPath.addTriangle(0, 0, 0, 20, 20+animateCounter, 25+animateCounter);

    //Rotation
    AffineTransform transform = AffineTransform::rotation(theta+M_PI_2);
    myPath.applyTransform(transform);
    
    //Translation
    transform = AffineTransform::translation(position.x, position.y);
    myPath.applyTransform(transform);
    
    g.fillPath(myPath);
    
//        ofTranslate(position.x, position.y);
    
//
//        ofMesh quad;
//        
//        // first triangle
//        quad.addVertex(ofVec3f(0, 0, 1));
//        quad.addVertex(ofVec3f(0, 20, 1));
//        quad.addVertex(ofVec3f(-20-animateCounter, 25 + animateCounter, 1));
//        
//        quad.addVertex(ofVec3f(0, 0, 1));
//        quad.addVertex(ofVec3f(0, 20, 1));
//        quad.addVertex(ofVec3f(20+animateCounter, 25+animateCounter, 1));
//
//        ofRotate(ofRadToDeg(theta) + 90);
//        quad.draw(); // now you'll see a square
//    ofPopMatrix();
    

    if(animateCounter<=ANIMATECOUNTERMAX && animateCounterIncreasing){
        animateCounter+=animateInc;
        
        if(animateCounter >= (ANIMATECOUNTERMAX-animateInc))
            animateCounterIncreasing = false;
    }
    else{
        animateCounter-=animateInc;
        
        if(animateCounter <= animateInc)
            animateCounterIncreasing = true;
    }
}

/*
 Flock towards mouse
 */

Point<float> Boid::steer(OwnedArray<Boid>& boids)
{
    Point<float> mousePos = getMouseXYRelative().toFloat();
    
    Point<float> changes = (mousePos - this->position) / 100.0f;
    return changes;
}

/*
 Flock towards the average position of the others
 */

Point<float> Boid::flock(OwnedArray<Boid>& boids)
{
    Point<float> sumOfPosOfOthers;
    int n = 0;
    
    for(int i=0; i<boids.size(); i++)
    {
        if (boids[i] != this)
        {
            sumOfPosOfOthers += boids[i]->position;
            n++;
        }
    }
    avgPosOfOthers = (sumOfPosOfOthers) / (float)n;
    Point<float> changes = (avgPosOfOthers - this->position) / 100.0f;
    return changes;
}

/*
 Avoid other boids
 */

Point<float> Boid::separate(OwnedArray<Boid>& boids)
{
    Point<float> changes;
    for(int i=0; i<boids.size(); i++)
    {
        if (boids[i] != this)
        {
            float dist = this->position.getDistanceFrom(boids[i]->position);
            if(abs(dist) < MIN_DISTANCE)
            {
                changes = (changes - (this->position - boids[i]->position));
            }
        }
    }
    return changes;
}

Point<float> Boid::align(OwnedArray<Boid>& boids)
{
    Point<float> sumOfVelOfOthers;
    int n =0;
    
    for(int i=0; i<boids.size(); i++)
    {
        if (boids[i] != this)
        {
            sumOfVelOfOthers += boids[i]->velocity;
            n++;
        }
    }
    avgVelOfOthers = sumOfVelOfOthers / (float)n;
    Point<float> changes = (avgVelOfOthers - this->velocity) / 20.0f;
    return changes;
}

Point<float> Boid::absVec2f(Point<float> theVector)
{
    return Point<float>(abs(theVector.x), abs(theVector.y));
}