#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	if( ! bg.loadImage("london-eye.png")) printf("Bg loading failed");
    if( ! sky.loadImage("sky.jpg")) printf("Sky loading failed");
    if( ! sky2.loadImage("sky.jpg")) printf("Sky loading failed");
    if( ! cloud.loadImage("cloud.png")) printf("Cloud loading failed");
    if( ! cloud2.loadImage("cloud2.png")) printf("Cloud loading failed");
    if( ! cloud3.loadImage("cloud3.png")) printf("Cloud loading failed");
    if( ! logo.loadImage("logo.png")) printf("Logo loading failed");
    if( ! hands.loadImage("hands.png")) printf("Hands loading failed");
    
    logo.resize(150, 150);
    hands.resize(1390, 742);

	font.loadFont("bebasneue-webfont.ttf", 20);
    
    finale.loadSound("finale.mp3");
    crowd.loadSound("crowd.mp3");
    crowd.setLoop(true);
    crowd.play();
    
    sky2.mirror(false, true);
    skyPos = 0;
    cloudPos = ofGetWidth() + 300;
    cloud2Pos = ofGetWidth() + 3000;
    cloud3Pos = ofGetWidth() + 5200;
    sky2Pos = ofGetWidth();
    skyVel = 1;
    cloudVel = 2;

	nearThreshold = 100;
    farThreshold = 1300;
    strokeWidth = 8.0f;
    blurAmount = 20;
    totalParticles = 0;
    currentTime = 0;
    maxIdleTime = 600;

    started = false;
    ready = false;
    blurEnabled = true;
    showInfo = false;

	niContext.setup();
	niContext.toggleMirror();

    niHandGenerator = new ofxHandGenerator();
	niHandGenerator->setup(&niContext, 2);
	niHandGenerator->setSmoothing(0.0f);
    //niHandGenerator->startTrackHands();
    //niHandGenerator->dropHands();
	//niHandGenerator->setFilterFactors(filterFactor); // custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with
    
    blur.setup(ofGetWidth(), ofGetHeight(), blurAmount, .2, 4);
    
    //ofEnableAlphaBlending();
    ofSetCircleResolution(64);
}

//--------------------------------------------------------------
void ofApp::update()
{
    niContext.update();

    tracked = niHandGenerator->getHand(0);
    tracked2 = niHandGenerator->getHand(1);
    
    //cout << tracked->projectPos.y << ", " << tracked2->projectPos.y << ", " << startTime << "/n";

    if( tracked->projectPos.y || tracked2->projectPos.y )
    {
        ready = true;
        ofSetLineWidth( ofRandom(15) );

        lHandOld.x = lHand.x;
        lHandOld.y = lHand.y;
        lHand.x = tracked->projectPos.x * (ofGetWidth() / 640);
        lHand.y = tracked->projectPos.y * (ofGetHeight() / 480);

        rHandOld.x = rHand.x;
        rHandOld.y = rHand.y;
        rHand.x = tracked2->projectPos.x * (ofGetWidth() / 640);
        rHand.y = tracked2->projectPos.y * (ofGetHeight() / 480);
        
        totalParticles = 0;

        for( int i = fireworks.size()-1; i >=0; i-- )
        {
            if( fireworks[i].finish() )
                fireworks.erase( fireworks.begin() + i );
            else
                fireworks[i].update();
            
            totalParticles += fireworks[i].getTotalParticles();
        }

        if( fireworks.size() < 40 && lHand.y + 50 < lHandOld.y )
        {
            Firework newFirework;
            newFirework.init( lHand.x, lHand.y - 100 );
            fireworks.push_back( newFirework );

            lSensor = 200;
            started = true;
            currentTime = 0;
        }

        if( fireworks.size() < 40 && rHand.y + 50 < rHandOld.y )
        {
            Firework newFirework;
            newFirework.init( rHand.x, rHand.y - 100 );
            fireworks.push_back( newFirework );

            rSensor = 200;
            started = true;
            currentTime = 0;
        }
        
        if (fireworks.size() > 5 && !finale.getIsPlaying()) {
            finale.play();
        }
        
        if (fireworks.size() < 2 && !crowd.getIsPlaying()) {
            crowd.play();
        }
        
        if( started) {
            blurAmount = 0;
            blurEnabled = false;
            currentTime++;
            
            if (currentTime > maxIdleTime) {
                currentTime = 0;
                started = false;
                blurAmount = 20;
                blurEnabled = true;
            }
        }
    }
    else {
        ready = false;
    }
    
    if( blurEnabled )
    {
        blur.setScale(ofMap(200, 0, ofGetWidth(), 0, blurAmount));
        blur.setRotation(ofMap(200, 0, ofGetHeight(), -PI, PI));
    }
    
    updateSky();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if( blurEnabled ) {
        blur.begin();
    }
    
    ofSetColor(255,255,255);
    
    sky.draw(skyPos,0);
    sky2.draw(sky2Pos,0);
    cloud.draw(cloudPos, 200);
    cloud2.draw(cloud2Pos, 500);
    cloud3.draw(cloud3Pos, 50);
    bg.draw(0,0);
    
    ofSetColor(255,255,255,70);
    logo.draw((ofGetWidth()/2 - logo.width/2), 50);
    ofSetColor(255,255,255,255);
    
    if( blurEnabled )
    {
        blur.end();
        blur.draw();
    }

    if( started && ready )
    {
        if (showInfo) {
            ofDrawBitmapString("Left Hand: " + ofToString(lHand.x) + ", " + ofToString(lHand.y), 20,20);
            ofDrawBitmapString("Right Hand: " + ofToString(rHand.x) + ", " + ofToString(rHand.y), 20,40);
            ofDrawBitmapString("FPS: " + ofToString( ofGetFrameRate(), 2 ), 20, 60);
            ofDrawBitmapString("Blur status: " + ofToString( blurEnabled ), 20, 80);
            ofDrawBitmapString("Blur amount: " + ofToString( blurAmount ), 20, 100);
            ofDrawBitmapString("Trail width: " + ofToString( strokeWidth ), 20, 120);
            ofDrawBitmapString("Fireworks: " + ofToString( fireworks.size() ), 20, 140);
            ofDrawBitmapString("Particles: " + ofToString( totalParticles ), 20, 160);

            ofDrawBitmapString("Toggle Fullscreen: 'F'", ofGetWidth() - 200,20);
            ofDrawBitmapString("Toggle Blur: 'B'", ofGetWidth() - 200,40);
            ofDrawBitmapString("Set Blur Amount: 'Q/W'", ofGetWidth() - 200,60);
            ofDrawBitmapString("Set Trail Width: '+/-'", ofGetWidth() - 200,80);
            ofDrawBitmapString("Sky velocity: 'S/D'", ofGetWidth() - 200,100);
            ofDrawBitmapString("Info: 'Spacebar'", ofGetWidth() - 200,120);
        }
        
        for (int i = 0; i< fireworks.size(); i++) {
            fireworks[i].draw();
        }

        if( lSensor > 0 )
        {
            lSensor = lSensor - 3;
            ofSetColor(255, 255, 255, lSensor);
            ofCircle(ofGetWidth() - 400, ofGetHeight() + 300, 400);
            font.drawString("Right hand", ofGetWidth() - 440, ofGetHeight() - 40);
        }
        if( rSensor > 0 )
        {
            rSensor = rSensor - 3;
            ofSetColor(255, 255, 255, rSensor);
            ofCircle(400, ofGetHeight() + 300, 400);
            font.drawString("Left hand", 360, ofGetHeight() - 40);
        }
    }
    
    ofSetColor(255,255,255);
    
    if( !started )
    {
        logo.draw((ofGetWidth()/2 - logo.width/2), 50);
        hands.draw((ofGetWidth()/2 - hands.width/2), 300);
        
        font.drawString("Sapient Fireworks", ofGetWidth()/2 - 85, 270);
        font.drawString("A Kinect hand tracking demo", ofGetWidth()/2 - 137, 310);
        font.drawString("Show your hands until you can move both green dots, then lift your hands to start your fireworks show!", ofGetWidth()/2 - 550, ofGetHeight() - 80);
        
        ofFill();
        ofSetColor(0, 255, 0);
        ofCircle(lHand.x, lHand.y, 15);
        ofCircle(rHand.x, rHand.y, 15);
    }
}

void ofApp::updateSky()
{
    if (skyPos > -sky.getWidth() && sky2Pos > 0) {
        skyPos -= skyVel;
        cloudPos -= cloudVel;
        cloud2Pos -= cloudVel;
        cloud3Pos -= cloudVel;
        
        if (skyPos < -(sky.getWidth() - ofGetWidth())) {
            sky2Pos -= skyVel;
        }
    }
    else if (sky2Pos >= 0) {
        skyPos = ofGetWidth();
        cloudPos = ofGetWidth() + 300;
        cloud2Pos = ofGetWidth() + 3000;
        cloud3Pos = ofGetWidth() + 5200;
        sky2Pos -= skyVel;
    }
    
    if (sky2Pos > -sky2.getWidth() && skyPos > 0) {
        sky2Pos -= skyVel;
        cloudPos -= cloudVel;
        cloud2Pos -= cloudVel;
        cloud3Pos -= cloudVel;
        
        if (sky2Pos < -(sky2.getWidth() - ofGetWidth())) {
            skyPos -= skyVel;
        }
    }
    else if (skyPos >= 0) {
        sky2Pos = ofGetWidth();
        cloudPos = ofGetWidth() + 300;
        cloud2Pos = ofGetWidth() + 2500;
        cloud3Pos = ofGetWidth() + 5200;
        skyPos -= skyVel;
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
	    if( blurAmount < 30 )
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
            skyVel += 0.5;
    }
    
    if(key == 'd')
    {
        if( skyVel > 0 )
            skyVel -= 0.5;
    }

	if(key == 32) //spacebar
	{
        showInfo = !showInfo;
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
