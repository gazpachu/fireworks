#pragma once
#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofTrueTypeFont.h"
#include "ofxBlur.h"
#include "Firework.h"

class ofApp : public ofBaseApp
{
	public:

		void setup();
		void update();
		void draw();

        void updateSky();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        int lSensor, rSensor, nearThreshold, farThreshold, blurAmount, skyPos, sky2Pos, cloudPos, cloud2Pos, cloud3Pos, currentTime, totalParticles, maxIdleTime;
        float strokeWidth, skyVel, cloudVel;
        bool started, ready, blurEnabled, showInfo;

        ofxOpenNIContext niContext;
        ofxHandGenerator* niHandGenerator;
        ofxTrackedHand* tracked;
        ofxTrackedHand* tracked2;

        ofPoint lHand, lHandOld, rHand, rHandOld;

        ofImage bg, sky, sky2, cloud, cloud2, cloud3, logo, hands;

        ofTrueTypeFont font;
		ofxBlur blur;

		vector<Firework> fireworks;
    
        ofSoundPlayer  crowd, finale;
};









