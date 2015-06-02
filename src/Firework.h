#ifndef FIREWORK
#define FIREWORK

#include "ofMain.h"
#include "particle.h"

class Firework {
public:

    Firework();

    void    init(int _x, int _y);

    bool    finish();

    void    update();
    void    draw();

private:
    vector<Particle> particles;

    ofColor color;

    ofPoint pos;
    ofPoint dst;

    float   timer;

    bool    bArrive;
    float gravityAmount;

    ofSoundPlayer  fireworksStart;
    ofSoundPlayer  fireworksEnd;
};

#endif
