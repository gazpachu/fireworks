#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//ofBackground(0);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	if( ! bg.loadImage("london-eye.png")) printf("Bg loading failed");
    if( ! sky.loadImage("sky.jpg")) printf("Sky loading failed");
    if( ! sky2.loadImage("sky.jpg")) printf("Sky loading failed");
	font.loadFont("bebasneue-webfont.ttf", 25);
    
    sky2.mirror(false, true);
    skyPos = 0;
    sky2Pos = ofGetWidth();
    skyVel = 4;

	nearThreshold = 100;
    farThreshold = 1300;
    strokeWidth = 8.0f;
    blurAmount = 0;

    started = false;
    blurEnabled = false;

	niContext.setup();
	//niContext.toggleRegisterViewport();
	niContext.toggleMirror();

    //niDepthGenerator.setup( &niContext );
    //niImageGenerator.setup( &niContext );

    //niDepthGenerator.setDepthThreshold(0, nearThreshold, farThreshold);

    //niUserGenerator.setup( &niContext );
    //niUserGenerator.setSmoothing(filterFactor); // built in openni skeleton smoothing...
	//niUserGenerator.setMaxNumberOfUsers(1);

    niHandGenerator = new ofxHandGenerator();
	niHandGenerator->setup(&niContext,2);
	niHandGenerator->setMaxNumHands(2);
	niHandGenerator->setSmoothing(0.0f); // built in openni hand track smoothing...
	//niHandGenerator->setFilterFactors(filterFactor); // custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with
    //niHandGenerator->startTrackHands();
    
    blur.setup(ofGetWidth(), ofGetHeight(), blurAmount, .2, 4);
    
    //ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update()
{
    niContext.update();
    //niDepthGenerator.update();
    //niImageGenerator.update();
    //niUserGenerator.update();

    tracked = niHandGenerator->getHand(0);
    tracked2 = niHandGenerator->getHand(1);

    if( ( tracked->projectPos.y || tracked2->projectPos.y ) )
        started = true;

    if( started )
    {
        ofSetLineWidth( ofRandom(15) );

        lHandOld.x = lHand.x;
        lHandOld.y = lHand.y;
        lHand.x = tracked->projectPos.x * (ofGetWidth() / 640);
        lHand.y = tracked->projectPos.y * (ofGetHeight() / 480);

        rHandOld.x = rHand.x;
        rHandOld.y = rHand.y;
        rHand.x = tracked2->projectPos.x * (ofGetWidth() / 640);
        rHand.y = tracked2->projectPos.y * (ofGetHeight() / 480);

        for( int i = fireworks.size()-1; i >=0; i-- )
        {
            if( fireworks[i].finish() )
                fireworks.erase( fireworks.begin() + i );
            else
                fireworks[i].update();
        }

        if( fireworks.size() < 20 && lHand.y + 50 < lHandOld.y )
        {
            Firework newFirework;
            newFirework.init( lHand.x, lHand.y - 100 );
            fireworks.push_back( newFirework );

            lSensor = 200;
        }

        if( fireworks.size() < 20 && rHand.y + 50 < rHandOld.y )
        {
            Firework newFirework;
            newFirework.init( rHand.x, rHand.y - 100 );
            fireworks.push_back( newFirework );

            rSensor = 200;
        }
        
        if( blurEnabled && started )
        {
            blur.setScale(ofMap(200, 0, ofGetWidth(), 0, blurAmount));
            blur.setRotation(ofMap(200, 0, ofGetHeight(), -PI, PI));
        }
    }
    
    
    
    if (skyPos > -sky.getWidth() && sky2Pos > 0) {
        skyPos -= skyVel;
        
        if (skyPos < -(sky.getWidth() - ofGetWidth())) {
            sky2Pos -= skyVel;
        }
    }
    else if (sky2Pos >= 0) {
        skyPos = ofGetWidth();
        sky2Pos -= skyVel;
    }
    
    if (sky2Pos > -sky2.getWidth() && skyPos > 0) {
        sky2Pos -= skyVel;
        
        if (sky2Pos < -(sky2.getWidth() - ofGetWidth())) {
            skyPos -= skyVel;
        }
    }
    else if (skyPos >= 0) {
        sky2Pos = ofGetWidth();
        skyPos -= skyVel;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if( blurEnabled && started )
    {
        blur.begin();
    }
    
    ofSetColor(255,255,255);
    sky.draw(skyPos,0);
    sky2.draw(sky2Pos,0);
    
    bg.draw(0,0);

    if( !started )
    {
        font.drawString("SHOW ME YOUR HAND TO START PLAYING", ofGetWidth()/2 - 50, ofGetHeight()/2);
        font.drawString("(press spacebar to restart)", ofGetWidth()/2 - 50, ofGetHeight()/2 + 50);
    }
    else
    {
        ofDrawBitmapString("Left Hand: " + ofToString(lHand.x) + ", " + ofToString(lHand.y), 20,20);
        ofDrawBitmapString("Right Hand: " + ofToString(rHand.x) + ", " + ofToString(rHand.y), 20,40);
        ofDrawBitmapString("FPS: " + ofToString( ofGetFrameRate(), 2 ), 20, 60);
        ofDrawBitmapString("Blur status: " + ofToString( blurEnabled ), 20, 80);
        ofDrawBitmapString("Blur amount: " + ofToString( blurAmount ), 20, 100);
        ofDrawBitmapString("Trail width: " + ofToString( strokeWidth ), 20, 120);

        ofDrawBitmapString("Toggle Fullscreen: 'F'", ofGetWidth() - 200,20);
        ofDrawBitmapString("Toggle Blur: 'B'", ofGetWidth() - 200,40);
        ofDrawBitmapString("Set Blur Amount: 'Q/W'", ofGetWidth() - 200,60);
        ofDrawBitmapString("Set Trail Width: '+/-'", ofGetWidth() - 200,80);
        ofDrawBitmapString("Restart: 'Spacebar'", ofGetWidth() - 200,100);

        //star.draw(lHand.x, lHand.y);

        

        for (int i = 0; i< fireworks.size(); i++)
            fireworks[i].draw();

        if( blurEnabled && started )
        {
            blur.end();
            blur.draw();
        }

        if( lSensor > 0 )
        {
            lSensor = lSensor - 3;
            ofSetColor(255, 255, 255, lSensor);
            ofSetCircleResolution(64);
            ofCircle(ofGetWidth() - 400, ofGetHeight() + 300, 400);
        }
        if( rSensor > 0 )
        {
            rSensor = rSensor - 3;
            ofSetColor(255, 255, 255, rSensor);
            ofSetCircleResolution(64);
            ofCircle(400, ofGetHeight() + 300, 400);
        }

        /*niDepthGenerator.draw(10, 200, 300, 200);
        niImageGenerator.draw(10, 420, 300, 200);
        niUserGenerator.draw(300,200);*/
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if(key == 'f')
		ofToggleFullscreen();

	if(key == 'b')
	{
        if( blurEnabled ) {
            blurEnabled = false;
            blur.setup(ofGetWidth(), ofGetHeight(), blurAmount, .2, 4);
        }
        else {
            blurEnabled = true;
        }
	}

	if(key == '+')
	{
	    if( strokeWidth < 10.0f )
            strokeWidth++;
	}

	if(key == '-')
	{
	    if( strokeWidth > 0.0f )
            strokeWidth--;
	}

	if(key == 'w')
	{
	    if( blurAmount < 10 )
            blurAmount++;
	}

	if(key == 'q')
	{
	    if( blurAmount > 0 )
            blurAmount--;
	}
    
    if(key == 's')
    {
        if( skyVel < 10 )
            skyVel++;
    }
    
    if(key == 'd')
    {
        if( skyVel > 0 )
            skyVel--;
    }

	if(key == 32) //spacebar
	{
	    started = false;
        niHandGenerator->dropHands();

        delete niHandGenerator;
        niHandGenerator = new ofxHandGenerator();
        niHandGenerator->setup(&niContext,1);
        niHandGenerator->setMaxNumHands(2);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
