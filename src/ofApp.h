#pragma once
#include "ofMain.h"
#include "ofxSyphon.h"
//#include "ofxNDISender.h"
//#include "ofxNDISendStream.h"

#include "BamBooCenter.h"
#include "StatusCenter.h"
#include "UDPCenter.h"
#include "CueCenter.h"
#include "EffectCenter.h"

using namespace std;

class ofApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
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
    
    // Mouse drug setting
    ofVec3f start_position = ofVec3f(0, 0, 0);
    ofVec3f end_position = ofVec3f(0, 0, 0);
    ofVec3f last_rotation = ofVec3f(0, 0, 0);
    ofVec3f perspective = ofVec3f(0, 0, 0);
    bool isGridEnable = true;
    
    //
    BamBooCenter bamboo;
    bool isMotorOn = false;
    bool isMotorCurrentMessageShow = true;
    bool isHIMCRebootTrigger = false;
    vector<float> curve_setting;
    
    ofTexture led_texture;
    ofPixels led_pixels;
    ofTexture led_white_texture;
    ofPixels led_white_pixels;
    ofTexture motor_texture;
    ofPixels motor_pixels;
    
    
    ofFbo fbo;
    ofFbo white_fbo;
    ofFbo motor_fbo;
    
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    ofxSyphonServerDirectory dir;
    ofxSyphonClient led_client;
    ofxSyphonClient led_white_client;
    ofxSyphonClient motor_client;
    
    bool isSyphonLightOpened = false;
    bool isSyphonLightWhiteOpened = false;
    bool isSyphonMotorOpened = false;
    
    int dirIdx = 0;
    uint64_t syphon_timer;
    uint64_t syphon_white_timer;
    uint64_t syphon_motor_timer;
    
    StatusCenter statusCenter;
    
    //
    
    UDPCenter udpCenter;
    string cue_message;
    
    CueCenter cueCenter;
    CueLEDStruct cue_led_struct;
    CueMotorStruct cue_motor_struct;
//    bool cue_plc_status;
    string cue_plc_message;
    string cue_madrix_message;
    string cue_video_message;
    
    
    bool isCueEnable = false;
    bool isAutoRotationEnable = false;
    
    // Light Boss
    
    bool isPixelsSendEnable = false;
//    ofxNDISender NDI_sender;
//    ofxNDISendVideo NDI_video;
//    bool isNDIEnable = false;
    
    EffectCenter effectCenter;
};
