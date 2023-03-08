//
//  UDPCenter.hpp
//  BamBooGod
//
//  Created by Fish on 2020/12/30.
//

#ifndef UDPCenter_h
#define UDPCenter_h

#include "ofxNetwork.h"
#include "ofxJSON.h"
#include "Utils.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <queue>

using namespace std;

#define NUM_BYTES_IN_IMG (30057)

enum serverState{
    NON, READY, SENDING, COMPLETE
};

class BBLightSender{
    
public:
    
    BBLightSender(){
        state        = NON;
        totalBytes    = NUM_BYTES_IN_IMG;
    }
    
    void setup(){
        
        UDP.Create();
        UDP.SetSendBufferSize(65535);
        UDP.Connect("127.0.0.1", 11999);
        UDP.SetNonBlocking(true);
        
        state= READY;
    }
    
    
    void setPixels(unsigned char * pixelData){
        
        if( state == NON ) return;
        
        memcpy(pixels, pixelData, totalBytes);
        state            = SENDING;
        numSentBytes    = 0;
    }
    
    
    void update(int numToSend = 1024){
        
        UDP.Send((char *)&pixels[0],numToSend);
        
//        if( state == SENDING && numSentBytes < totalBytes ){
//
//            if( numToSend + numSentBytes > totalBytes ){
//                numToSend -= ( (numToSend + numSentBytes) - totalBytes );
//            }
//
//            UDP.Send((char *)&pixels[numSentBytes],numToSend);
//            //            TCP.sendRawBytesToAll( (char *)&pixels[numSentBytes], numToSend);
//            numSentBytes += numToSend;
//        }
//
//        if( numSentBytes >= totalBytes && state != COMPLETE){
//            state = COMPLETE;
//            tmpstartTimer =startTimer;
//            endTimer =ofGetElapsedTimeMillis();
//        }
        
    }
    
    serverState state;
    int numSentBytes;
    int totalBytes;
    
    ofxUDPManager UDP;
    
    int width = 470;
    int height = 650;
    ofTexture tex;
    
    unsigned char pixels[NUM_BYTES_IN_IMG];
    
    uint64 startTimer = 1;
    uint64 tmpstartTimer = 1;
    uint64 endTimer = 2;
    
};

class UDPCenter
{

public:
    void setup();
    void update();
    void exit();
    
    
    uint64_t udp_start_time;

    ofxUDPManager udp_cue;
    queue<string> cue_queue;
    void popCueMessage(string &current_cue_message);
    
    string udp_qlab_ip = "127.0.0.1";
    int udp_qlab_recv_port = 53089;
    
    string udp_motor_boss_ip = "10.10.10.66";
    int udp_motor_boss_send_port = 53066;
    int udp_motor_boss_recv_port = 53088;
    
    int udp_motor_boss_send_port_plc = 53502;
    int udp_motor_boss_recv_port_plc = 53501;
    
    string udp_light_boss_ip = "10.10.10.77";
//    string udp_light_boss_ip = "127.0.0.1";
    int udp_light_boss_send_port = 53077;
    int udp_light_boss_recv_port = 53087;
    
    int udp_light_boss_send_port_pixels = 53097;
    
    string udp_video_boss_ip = "127.0.0.1";
    int udp_video_boss_send_port = 53099;
    
    
    //Motor udp parameters
    ofxUDPManager udp_motor_boss;
    bool isMotorBossTx = false;
    bool isMotorBossRx = false;
    
    void sendMotorBossStatus(vector<float>,bool _isMotorOn, bool isMotorRebootTrigger);
    uint64_t current_motor_boss_send_timer = 0;
    
    Json::Value motor_boss_json;
    
    ofxUDPManager udp_motor_boss_plc;
//    void sendMotorBossPLCStatus(bool _isPLCOn);
    void sendMotorBossPLCMessage(string _cue_string);
    
    //Light udp parameters
    ofxUDPManager udp_light_boss;
    bool isLightBossTx = false;
    bool isLightBossRx = false;
    void sendLightBossMARDIXMessage(string _cue_string);
    
    
    ofxUDPManager udp_light_boss_pixels;
    void sendLightBossPixels(unsigned char * pixelData);
    uint64_t current_light_boss_send_timer = 0;
    
    ofxUDPManager udp_video_boss;
    void sendVideoBossMessage(string _cue_string);
    
//    void sendLightBossMessage();
};


#endif /* UDPCenter_h */
