/*
  ==============================================================================

    Boid.h
    Created: 14 Oct 2015 7:22:35pm
    Author:  Cárthach Ó Nuanáin

  ==============================================================================
*/

#ifndef BOID_H_INCLUDED
#define BOID_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Boid.h"

#define NUMOFBOIDS 20

//==============================================================================
/*
*/
class Boids    : public AnimatedAppComponent
{
public:
    Boids()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        for (int i=0; i<NUMOFBOIDS; i++) {
            boids.add(new Boid());
            addAndMakeVisible(boids[i]);
        }

        setFramesPerSecond(15);
    }

    ~Boids()
    {
    }

    void paint (Graphics& g)
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (Colours::white);   // clear the background
        

//        g.setColour (Colours::grey);
//        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//        g.setColour (Colours::lightblue);
//        g.setFont (14.0f);
//        g.drawText ("Boid", getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
//        
        for (int i=0; i<NUMOFBOIDS; i++) {
            boids[i]->paint(g);
        }
    }
    
    void update()
    {
        for (int i=0; i<NUMOFBOIDS; i++) {
            boids[i]->update(boids);
        }
        

    }

    void resized()
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        for (int i=0; i<NUMOFBOIDS; i++) {
            boids[i]->setBounds(getBounds());
        }
    }

private:
    OwnedArray<Boid> boids;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Boids)
};


#endif  // BOID_H_INCLUDED
