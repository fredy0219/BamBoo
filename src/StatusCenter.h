//
//  StatusCenter.hpp
//  BamBooGod
//
//  Created by Fish on 2020/12/30.
//

#ifndef StatusCenter_h
#define StatusCenter_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxJSON.h"
#include "Utils.h"

class StatusCenter
{

public:
    void setup();
    void update();
    void draw();
    
    bool isBamBooGodCueInput;
    void drawBamBooGodStatus();

    bool himc_status[8] = {true, false, true, false, true, true, true, true};
    bool isMotorBossTx = false;
    bool isMotorBossRx = false;

    bool isMotorBossAlarm = false;
    bool isMotorBossStatus = false;
    bool isMotorBossAllOK = false;
    vector<float> bezier_curve_setting;
    
    bool isPLCEnable = false;
    void drawMotorBossStatus();
    void drawMotorBezierCurve();
    
    bool isLightBossTx = false;
    bool isLightBossRx = false;
    bool isLightBossAlive = false;
    bool isNDIEnable = false;
    void drawLightBossStatus();

    list<string> status_messages;
    void updateStatusMessages(string status_message);
    void drawStatusMessages();
    bool isStatusMessageEnable = true;
    bool isHelperMessageEnable = true;
    
    void enableStatusMessage();
    void enableHelperMessage();
    
    void setBamBooGodStatus(bool isCueInput);
    
    void setMotorBossStatus(bool tx, bool rx, bool motor_power, Json::Value motorBossJson);
    
    void setMotorBezierCurveSetting(vector<float> setting);
    
    void setLightBossStatus(bool tx, bool rx, bool ndi_status);
    
    void setUDPSetting(string, string, string, int, int, int, int, int, int, int, int, int);
    
    bool isSyphonLightOpened;
    bool isSyphonLightWhiteOpened;
    bool isSyphonMotorOpened;
    
    void setSyphonStatus(bool, bool, bool);
    void drawSyphonStatus();
    
    string udp_qlab_ip = "127.0.0.1";
    int udp_qlab_recv_port = 53089;
    
    string udp_motor_boss_ip = "10.10.10.66";
    int udp_motor_boss_send_port = 53066;
    int udp_motor_boss_recv_port = 53088;
    
    int udp_motor_boss_send_port_plc = 53502;
    int udp_motor_boss_recv_port_plc = 53501;
    
    string udp_light_boss_ip = "10.10.10.77";
    int udp_light_boss_send_port = 53077;
    int udp_light_boss_recv_port = 53087;
    
    int udp_light_boss_send_port_pixels = 11999;
    
    int udp_video_boss_send_port = 53099;
    
    ofImage DECADE_logo;
    ofImage HIWIN_logo;
    ofImage LL_logo;
};

#endif /* StatusCenter_hpp */
