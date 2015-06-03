#include "Firework.h"

Firework::Firework()
{
    color.set(ofRandom(255), ofRandom(255), ofRandom(255));
    //color.setHue( ofRandom(100) );

    bArrive = false;
    vignette = 200;
}

void Firework::init(int _x, int _y)
{
    particles.clear();

    dst.set(_x, _y);
    pos.set(_x + ofRandom(-100,100), ofGetHeight());

    bArrive = false;
    timer = 0.0f;

    gravityAmount = ofRandom(0.0, 0.5);

    int start = ofRandom(3);
    if( start == 1 )
        fireworksStart.loadSound("fireworks_start_1.mp3");
    else if( start == 2 )
        fireworksStart.loadSound("fireworks_start_2.mp3");
    else
        fireworksStart.loadSound("fireworks_start_3.mp3");

    int end = ofRandom(3);
    if( end == 1 )
        fireworksEnd.loadSound("fireworks_end_1.mp3");
    else if( end == 2 )
        fireworksEnd.loadSound("fireworks_end_2.mp3");
    else
        fireworksEnd.loadSound("fireworks_end_3.mp3");

    fireworksStart.setMultiPlay(true);
    fireworksEnd.setMultiPlay(true);
    fireworksStart.play();
}

bool Firework::finish()
{
    if ( bArrive && (particles.size() == 0) )
        return true;
    else
        return false;
}

void Firework::update()
{
    if (!bArrive){

        if ( pos.distance(dst) < 5 ){

            //  Explotion!
            //
            fireworksEnd.play();

            int nParticles = ofRandom(15,60);

            for (int i = 0; i < nParticles; i++)
            {
                Particle myParticle;
                myParticle.set( dst );
                myParticle.color.set(color);
                //myParticle.color.setHue(  myParticle.color.getHue() + ofRandom(-0.05,0.05) );
                //myParticle.color.setSaturation(  ofRandom(0.5,1.0) );
                //myParticle.color.setBrightness(  ofRandom(0.5,1.0) );

                myParticle.trailLength = ofRandom(10,50);
                myParticle.damping = ofRandom(0.001, 0.01);

                particles.push_back(myParticle);
            }

            bArrive = true;

        } else {

            float pct = powf(timer, 0.6);

            pos.x = (1-pct) * pos.x + (pct) * dst.x;
            pos.y = (1-pct) * pos.y + (pct) * dst.y;

            timer += 0.001f;
        }

    } else {


        ofPoint gravity = ofPoint(0.0, gravityAmount) ;

        for (int i = particles.size()-1; i >= 0; i--){

            //  Erase particles that are not longer visible
            //  or update the ones that are
            //
            if (particles[i].y > ofGetHeight() || particles[i].size < 0.3 ){
                particles.erase(particles.begin()+i);
            } else {
                particles[i].addForce( gravity );
                particles[i].update();

                //  Reduce progressively the brighness, size and trail
                //
                //particles[i].color.setBrightness( particles[i].color.getBrightness() * 0.995 );
                particles[i].size *= 0.998;
                particles[i].trailLength *= 0.995;
            }
        }
    }
}

void Firework::draw()
{
    if( !bArrive )
    {
        ofSetColor(255);
        ofCircle(pos, 4);
    }
    else
    {
        for (int i = 0; i < particles.size(); i++)
        {
            particles[i].draw();
        }

        if (vignette > 0) {
            ofBackgroundGradient(ofColor(0,0,0,0), ofColor(0,0,0,vignette), OF_GRADIENT_CIRCULAR);
            vignette -= 50;
        }
    }
}

int Firework::getTotalParticles() {
    return particles.size();
}



