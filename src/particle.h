#ifndef PARTICLE
#define PARTICLE

#include "ofMain.h"

class Particle : public ofPoint
{
    public:

        Particle();

        void addForce( ofPoint &_force );
        void update();
        void draw();

        ofFloatColor color;

        ofPoint vel, acc;

        float trailLength, damping, size;

    private:

        vector <ofPoint> trail;

};

#endif
