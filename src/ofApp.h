#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxOsc.h"
#include "ofxLeapMotion2.h"

#define OSC_SEND_PORT 20568
#define OSC_SEND_HOST "localhost"

class ofApp : public ofBaseApp {

public:
    void setup();

    void update();

    void draw();

    void keyPressed(int key);

    void keyReleased(int key);

    void mouseMoved(int x, int y);

    void mouseDragged(int x, int y, int button);

    void mousePressed(int x, int y, int button);

    void mouseReleased(int x, int y, int button);

    void mouseEntered(int x, int y);

    void mouseExited(int x, int y);

    void windowResized(int w, int h);

    void dragEvent(ofDragInfo dragInfo);

    void gotMessage(ofMessage msg);
	void exit();

	ofxImGui::Gui gui;
	ofxLeapMotion leap;
	vector <ofxLeapMotionSimpleHand> simpleHands;

	vector <int> fingersFound;
	ofEasyCam cam;

	ofxOscSender sender;

private:
	void sendOscMessage(std::string address, float value);

};
