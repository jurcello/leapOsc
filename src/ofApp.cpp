#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_WARNING);

    leap.open();

    // keep app receiving data from leap motion even when it's in the background
    leap.setReceiveBackgroundFrames(true);

    cam.setOrientation(ofPoint(-20, 0, 0));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    gui.setup();
    sender.setup(OSC_SEND_HOST, OSC_SEND_PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    fingersFound.clear();

    simpleHands = leap.getSimpleHands();

    if( leap.isFrameNew() && simpleHands.size() ){

        leap.setMappingX(-230, 230, 0, 1);
        leap.setMappingY(90, 490, 0, 1);
        leap.setMappingZ(-150, 150, 0, 1);

        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};

        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                fingersFound.push_back(id);
            }
        }

        bool hasMessages = false;
        for (auto hand: simpleHands) {
            if (hand.isLeft) {
                this->thereminVolume = ofClamp((hand.handPos.y + ((float) this->smoothen - 1.f) * this->thereminVolume) / (float) smoothen, 0.f, 1.f);
            }
            else {
                this->thereminPitch = ofClamp(((1.f - hand.handPos.z) + ((float) this->smoothen - 1.f) * this->thereminPitch) / (float) smoothen, 0.f, 1.f);
            }
            hasMessages = true;
        }
        if (hasMessages) {
            ofxOscBundle bundle;
            this->addOscMessageToBundle(bundle, "/theremin/volume", thereminVolume);
            this->addOscMessageToBundle(bundle, "/theremin/pitch", thereminPitch);
            sender.sendBundle(bundle);
        }

    }
    leap.markFrameAsOld();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);

    ofSetColor(200);
    ofDrawBitmapString("ofxLeapMotion - Osc sender\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);

    cam.begin();

    ofPushMatrix();
    ofRotate(90, 0, 0, 1);
    ofSetColor(20);
    ofDrawGridPlane(800, 20, false);
    ofPopMatrix();


    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};

    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;

        ofSetColor(0, 0, 255);
        ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
        ofSetColor(255, 255, 0);
        ofDrawArrow(handPos, handPos + 100*handNormal);

        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip

            ofSetColor(0, 255, 0);
            ofDrawSphere(mcp.x, mcp.y, mcp.z, 12);
            ofDrawSphere(pip.x, pip.y, pip.z, 12);
            ofDrawSphere(dip.x, dip.y, dip.z, 12);
            ofDrawSphere(tip.x, tip.y, tip.z, 12);

            ofSetColor(255, 0, 0);
            ofSetLineWidth(20);
            ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
        }
    }
    cam.end();

    this->drawInterface();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

void ofApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}

void ofApp::addOscMessageToBundle(ofxOscBundle &bundle, std::string address, float value) {
    ofxOscMessage m;
    m.setAddress(address);
    m.addFloatArg(value);
    bundle.addMessage(m);
}

void ofApp::drawInterface() {
    gui.begin();

    ofxOscBundle bundle;

    {
        ImGui::Begin("Values");
        ImGui::Text("Sending on port %d", OSC_SEND_PORT);
        ImGui::Value("Theremin volume", thereminVolume);
        ImGui::Value("Theremin pitch", thereminPitch);
        ImGui::InputInt("Smoothen", &this->smoothen);
        ImGui::End();
    }
    {
        ImGui::Begin("Triggers for learning");
        ImGui::Text("Sending on port %d", OSC_SEND_PORT);
        if (ImGui::Button("Theremin volume")) {
            this->addOscMessageToBundle(bundle, "/theremin/volume", 0.5f);
        }
        if (ImGui::Button("Theremin pitch")) {
            this->addOscMessageToBundle(bundle, "/theremin/pitch", 0.5f);
        }
        ImGui::End();
    }

    gui.end();
    sender.sendBundle(bundle);
}

