#pragma once
#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofTrueTypeFont.h"
#include "ofxBlurShader.h"
#include "Firework.h"

class ofApp : public ofBaseApp
{
	public:

		void setup();
		void update();
		void draw();

		void reset();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        int nearThreshold, farThreshold, blurAmount;
        float strokeWidth;
        bool started, blurEnabled, reseting;

        ofxOpenNIContext niContext;
        ofxDepthGenerator niDepthGenerator;
        ofxImageGenerator niImageGenerator;
        ofxUserGenerator niUserGenerator;
        ofxHandGenerator* niHandGenerator;
        ofxTrackedHand* tracked;
        ofxTrackedHand* tracked2;

        ofPoint lHand, lHandOld, rHand, rHandOld;

        ofImage bg, star, tinkerbell;

        ofTrueTypeFont font;

		ofFbo rgbaFbo;
		ofxBlurShader* blur;
		ofShader shader;

		vector<Firework> fireworks;

		int lSensor, rSensor;
};









