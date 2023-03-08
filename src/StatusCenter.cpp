//
//  StatusCenter.cpp
//
//  Created by Fish on 2020/12/30.
//

#include "StatusCenter.h"

void StatusCenter::setup(){
    status_messages.push_back("Welcome to BamBoo God.");
    
    bezier_curve_setting.push_back(0.5);
    bezier_curve_setting.push_back(0.0);
    bezier_curve_setting.push_back(0.5);
    bezier_curve_setting.push_back(1.0);
    
    DECADE_logo.load("DECADE_logo.png");
}

void StatusCenter::update(){
    
    if(status_messages.size() > 5)
        status_messages.pop_back();
}

void StatusCenter::draw(){
    
    if(isStatusMessageEnable){
        
        ofPushStyle();
        ofFill();
        ofSetColor(255,25);
        ofPushMatrix();
        
        ofTranslate(10, ofGetHeight()-70);
        
        ofDrawRectangle(glm::vec2(0,-100), 600, 100);
        
        int y_position = 0;
        
        ofSetColor(255);
        for(const auto& m : status_messages)
            ofDrawBitmapString(m,10,y_position-=15);
        
        int ipX = 0;
        int ipY = -150;
        
        ofDrawBitmapString("Motor Boss IP: " + udp_motor_boss_ip + ", ms: "+ ofToString(udp_motor_boss_send_port) + ", mr: " + ofToString(udp_motor_boss_recv_port) + ", ps: " + ofToString(udp_motor_boss_send_port_plc) + ", pr: " + ofToString(udp_motor_boss_recv_port_plc) , ipX, ipY);
        
        ofDrawBitmapString("Light Boss IP: " + udp_light_boss_ip + ", ls: " + ofToString(udp_light_boss_send_port) + ", lr: " + ofToString(udp_light_boss_recv_port) + ", ps: " + ofToString(udp_light_boss_send_port_pixels), ipX, ipY+ 15);
        
        ofDrawBitmapString("Recv QLab IP: " + udp_qlab_ip + ", qr: " + ofToString(udp_qlab_recv_port), ipX, ipY+30);
        
        ofDrawBitmapString("Send Video Boss IP: 127.0.0.1, vs: " + ofToString(udp_video_boss_send_port), ipX, ipY+45);
        
        ofPopMatrix();
        ofPopStyle();
    }
    
    ofPushMatrix();
    ofPushStyle();
//    ofSetColor(255);
    ofTranslate(630 , ofGetHeight() - 80);
    ofFill();
    ofSetColor(255,25);
    ofDrawRectangle(glm::vec2(-10,-90),140,100);
    ofSetColor(255,100);
    DECADE_logo.draw(glm::vec2(-10,-90-55), 140,45);
    drawMotorBezierCurve();
    ofPopStyle();
    ofPopMatrix();
    
    ofPushMatrix();
    //draw background
    ofTranslate(0, ofGetHeight()-50);
    ofPushStyle();
    ofFill();
    ofSetColor(255,50);
    ofDrawRectangle(glm::vec2(0,0),ofGetWidth(),50);
    ofSetColor(255,75);
    ofDrawRectangle(glm::vec2(0,-10),ofGetWidth(),10);
    ofPopStyle();
    
    ofSetColor(255);
    ofDrawLine(130, -10, 130, 50);
    
    // draw BamBooGod Status
    ofPushMatrix();
    ofTranslate(10,20);
    drawBamBooGodStatus();
    ofPopMatrix();
    
    // draw MotorBoss Status
    ofPushMatrix();
    ofTranslate(140,20);
    drawMotorBossStatus();
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawLine(400, -10, 400, 50);
    // draw LightBoss Status
    ofPushMatrix();
    ofTranslate(410,20);
    drawLightBossStatus();
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(255);
    ofDrawLine(600, -10, 600, 50);
    ofTranslate(610,20);
    drawSyphonStatus();
    ofPopMatrix();
    
    ofPopMatrix();
    
    if(isHelperMessageEnable){
        
        int helperX = 250;
        int helperY = 75;
        ofDrawBitmapString("'c' to switch cue/video mode", ofGetWidth()-helperX,ofGetHeight()-helperY-120);
        ofDrawBitmapString("'e/space' to enable/disable motors", ofGetWidth()-helperX,ofGetHeight()-helperY-105);
        ofDrawBitmapString("'d' to un/draw bamboo", ofGetWidth()-helperX,ofGetHeight()-helperY-90);
        ofDrawBitmapString("'n' to switch livestream", ofGetWidth()-helperX,ofGetHeight()-helperY-75);
        ofDrawBitmapString("'r' to auto rotation", ofGetWidth()-helperX,ofGetHeight()-helperY-60);
        ofDrawBitmapString("'o' to original position", ofGetWidth()-helperX,ofGetHeight()-helperY-45);
        ofDrawBitmapString("'g' to open/close grid.", ofGetWidth()-helperX,ofGetHeight()-helperY-30);
        ofDrawBitmapString("'s' to open/close log.", ofGetWidth()-helperX,ofGetHeight()-helperY-15);
        ofDrawBitmapString("'h' to open/close helper.", ofGetWidth()-helperX,ofGetHeight()-helperY);
        
    }
}

void StatusCenter::drawBamBooGodStatus(){
    
    ofPushStyle();
    
    ofSetColor(255);
    ofDrawBitmapString("BamBooGod",0,-20);
    
    ofFill();
    if(isBamBooGodCueInput)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    ofDrawRectangle(60,-10,10,10);
    
    if(!isBamBooGodCueInput)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    
    ofDrawRectangle(60,10,10,10);
    
    ofSetColor(255);
    ofDrawBitmapString("QLab",0, 0);
    ofDrawBitmapString("Video", 0, 20);
    ofDrawBitmapString("FPS : " + ofToString(ofGetFrameRate()), 900, 20);
    ofPopStyle();
}

void StatusCenter::drawMotorBossStatus(){
    ofPushStyle();
    
    ofSetColor(255);
//    if(isMotorBossAllOK)
//        ofSetColor(255);
//    else
//        ofSetColor(255,30);
    ofDrawBitmapString("Motor Boss",0,-20);
    
    ofSetColor(255,0,0);
    ofFill();
    
    ofSetColor(255);
    ofDrawBitmapString("TX", 85, -20);
    if(isMotorBossTx){
        ofSetColor(0,255,0);
        ofDrawRectangle(105,-25,5,5);
        
        ofSetColor(255,0,0);
        ofDrawRectangle(110,-25,5,5);
    }
    else{
        ofSetColor(255,0,0);
        ofDrawRectangle(105,-25,5,5);
        
        ofSetColor(0,255,0);
        ofDrawRectangle(110,-25,5,5);
    }
    
    ofSetColor(255);
    ofDrawBitmapString("RX", 120, -20);
    
    if(isMotorBossRx){
        ofSetColor(0,255,0);
        ofDrawRectangle(140,-25,5,5);
        
        ofSetColor(255,0,0);
        ofDrawRectangle(145,-25,5,5);
    }
    else{
        ofSetColor(255,0,0);
        ofDrawRectangle(140,-25,5,5);
        
        ofSetColor(0,255,0);
        ofDrawRectangle(145,-25,5,5);
    }
    
    ofSetColor(255);
    ofDrawBitmapString("HIMC",0,0);
    ofFill();
    for(uint8_t i = 0 ; i<8 ; i++ ){
        
        if(!himc_status[i]){
            ofSetColor(255,0,0);
        }else{
            ofSetColor(0,255,0);
        }
        ofDrawRectangle(i*15+10+30,-10,10,10);
        ofSetColor(0,0,0);
    }
    
    ofSetColor(255);
    ofDrawBitmapString("PLC",0,20);
    if(isPLCEnable)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    ofDrawRectangle(10+30,10,10,10);
    
    ofSetColor(255);
    ofDrawBitmapString("MOTOR",80,20);
    if(isMotorBossAlarm)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    ofDrawRectangle(100+30,10,10,10);
    
    if(isMotorBossStatus)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    ofDrawRectangle(100+45,10,10,10);
    ofPopStyle();
    
}

void StatusCenter::drawMotorBezierCurve(){
    
    ofPolyline line;
    float curve_x_size = 120;
    float curve_y_size = -80;
    
    line.addVertex(0,0);
    line.bezierTo(bezier_curve_setting[0] * curve_x_size,
                  bezier_curve_setting[1] * curve_y_size,
                  bezier_curve_setting[2] * curve_x_size,
                  bezier_curve_setting[3] * curve_y_size,
                  curve_x_size,
                  curve_y_size
                  );
    
    ofSetColor(200,200,0);
    ofDrawLine(bezier_curve_setting[0] * curve_x_size, bezier_curve_setting[1] * curve_y_size, 0,0);
    ofSetColor(100,100,255);
    ofDrawCircle(bezier_curve_setting[0] * curve_x_size, bezier_curve_setting[1] * curve_y_size, 3);
    
    ofSetColor(200,200,0);
    ofDrawLine(bezier_curve_setting[2] * curve_x_size, bezier_curve_setting[3] * curve_y_size, curve_x_size, curve_y_size);
    ofSetColor(100,100,255);
    ofDrawCircle(bezier_curve_setting[2] * curve_x_size, bezier_curve_setting[3] * curve_y_size, 3);
    
    ofSetColor(200);
    line.draw();
}

void StatusCenter::drawLightBossStatus(){
    
    ofPushStyle();
    ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString("Light Boss",0,-20);
    
    ofFill();
    
    ofSetColor(255);
    ofDrawBitmapString("TX", 85, -20);
    
    if(isLightBossTx){
        ofSetColor(0,255,0);
        ofDrawRectangle(105,-25,5,5);
        
        ofSetColor(255,0,0);
        ofDrawRectangle(110,-25,5,5);
    }
    else{
        ofSetColor(255,0,0);
        ofDrawRectangle(105,-25,5,5);
        
        ofSetColor(0,255,0);
        ofDrawRectangle(110,-25,5,5);
    }
    
    ofSetColor(255);
    ofDrawBitmapString("RX", 120, -20);
    if(isLightBossRx){
        ofSetColor(0,255,0);
        ofDrawRectangle(140,-25,5,5);
        
        ofSetColor(255,0,0);
        ofDrawRectangle(145,-25,5,5);
    }
    else{
        ofSetColor(255,0,0);
        ofDrawRectangle(140,-25,5,5);
        
        ofSetColor(0,255,0);
        ofDrawRectangle(145,-25,5,5);
    }
    
    ofPopStyle();
    
    ofFill();
    ofDrawBitmapString("MADRIX STREAM",0,0);
    if(isNDIEnable)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    ofDrawRectangle(110,-10,10,10);
    
    ofPopStyle();
}

void StatusCenter::drawSyphonStatus(){
    
    ofPushStyle();
    ofPushStyle();
    
    ofSetColor(255);
    ofDrawBitmapString("Video Channel",0,-20);
    
    ofFill();
    
    ofSetColor(255);
    ofDrawBitmapString("Light RGB",0,0);
    if(isSyphonLightOpened)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    ofDrawRectangle(80,-10,10,10);
    
    ofSetColor(255);
    ofDrawBitmapString("Light W",0,20);
    if(isSyphonLightWhiteOpened)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    ofDrawRectangle(80,-10+20,10,10);
    
    ofSetColor(255);
    ofDrawBitmapString("Motor",80+35,0);
    if(isSyphonMotorOpened)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);
    ofDrawRectangle(80+80,-10,10,10);
    
    ofPopStyle();
    ofPopStyle();
}

void StatusCenter::updateStatusMessages(string status_message){
    
    if(status_message.length() > 0){
        
        ofLogNotice("bbgod_log") << status_message;
        status_messages.push_front(status_message);
    }
}

void StatusCenter::enableStatusMessage(){
    isStatusMessageEnable = !isStatusMessageEnable;
}

void StatusCenter::enableHelperMessage(){
    isHelperMessageEnable = !isHelperMessageEnable;
}

void StatusCenter::setBamBooGodStatus(bool isCueInput){
    
    isBamBooGodCueInput = isCueInput;
    
}

void StatusCenter::setMotorBossStatus(bool tx, bool rx, bool motor_power, Json::Value motor_boss_json){
    isMotorBossTx = tx;
    isMotorBossRx = rx;
    
    isPLCEnable = Json_ReadBool(motor_boss_json["PLC_RX"]);
    
    for(int i = 0 ; i < 8 ; i++){
        bool himc_enable = Json_ReadBool(motor_boss_json["HIMC_RX"][i]);
        himc_status[i] = himc_enable;
    }
    
    
    isMotorBossAlarm = Json_ReadBool(motor_boss_json["ALL_M_ALARM"]);
    isMotorBossStatus = Json_ReadBool(motor_boss_json["ALL_M_STATUS"]);
    isMotorBossAllOK = Json_ReadBool(motor_boss_json["ALL_OK"]);
}

void StatusCenter::setMotorBezierCurveSetting(vector<float> setting){
    for(int i = 0 ; i < 4 ; i++)
        bezier_curve_setting[i] = setting[i];
}

void StatusCenter::setLightBossStatus(bool tx, bool rx, bool ndi_status){
    isLightBossTx = tx;
    isLightBossRx = rx;
    isNDIEnable = ndi_status;
    
}

void StatusCenter::setUDPSetting(string _motor_boss_ip, string _light_boss_ip,
                                 string _udp_qlab_ip , int _qlab_port, int _motor_boss_send_port,
                                 int _motor_boss_recv_port, int _motor_boss_send_port_plc,
                                 int _motor_boss_recv_port_plc, int _light_boss_send_port,
                                 int _light_boss_recv_port, int _light_boss_send_port_pixels,
                                 int _video_boss_send_port){
    
    udp_motor_boss_ip = _motor_boss_ip;
    udp_light_boss_ip = _light_boss_ip;
    udp_qlab_ip = _udp_qlab_ip;
    udp_qlab_recv_port = _qlab_port;
    udp_motor_boss_send_port = _motor_boss_send_port;
    udp_motor_boss_recv_port = _motor_boss_recv_port;
    udp_motor_boss_send_port_plc = _motor_boss_send_port_plc;
    udp_motor_boss_recv_port_plc = _motor_boss_recv_port_plc;
    udp_light_boss_send_port = _light_boss_send_port;
    udp_light_boss_recv_port = _light_boss_recv_port;
    udp_light_boss_send_port_pixels = _light_boss_send_port_pixels;
    udp_video_boss_send_port = _video_boss_send_port;
    
}

void StatusCenter::setSyphonStatus(bool _isLightOpened, bool _isLightWhiteOpened, bool isMotorOpened){
    
    isSyphonLightOpened = _isLightOpened;
    isSyphonLightWhiteOpened = _isLightWhiteOpened;
    isSyphonMotorOpened = isMotorOpened;
}
