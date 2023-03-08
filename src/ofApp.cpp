#include "ofApp.h"
#include <string>

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("BamBoo God");
    
//    ofLogToFile("bbgod_log.txt", true);
    ofSetLogLevel("bbgod_log", OF_LOG_VERBOSE);
    
    system("say 'Bamboo God Online' &");
    
    bamboo.setup();
    statusCenter.setup();
    udpCenter.setup();
    cueCenter.setup(bamboo.getLayerMaxIndex());
    
    // Syphon
    dir.setup();
    led_client.setup();
    led_white_client.setup();
    motor_client.setup();
    
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    
    led_pixels.allocate(470, 650,OF_IMAGE_COLOR);
    motor_pixels.allocate(470, 650,OF_IMAGE_COLOR);
    
    fbo.allocate(470, 650);
    white_fbo.allocate(470, 650);
    motor_fbo.allocate(470, 650);
    
    ofNoFill();
    
    curve_setting.push_back(0.5);
    curve_setting.push_back(0.0);
    curve_setting.push_back(0.5);
    curve_setting.push_back(1.0);
    statusCenter.setMotorBezierCurveSetting(curve_setting);
    bamboo.setMotorBezierCurveSetting(curve_setting);
    
    ofLogVerbose("bbgod_log") << "BamBoo God wake up.";
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    // *** Update HIWIN Motor Boss Degree ***
    bamboo.update();
    bamboo.setMotorBossDegree(udpCenter.motor_boss_json);
    
    udpCenter.update();
    udpCenter.sendMotorBossStatus(bamboo.getMotorsDegrees(), isMotorOn, isHIMCRebootTrigger);
//    isMotorOn = true;
    
    // *** Update Cue Message ***
    udpCenter.popCueMessage(cue_message);
    cueCenter.update(cue_message, isCueEnable);
    
//    if(cueCenter.popCuePLCStatus(cue_plc_status)){
//        isMotorOn = cue_plc_status;
//        udpCenter.sendMotorBossPLCStatus(cue_plc_status);
//    }
    
    if(cueCenter.popCuePLCMessage(cue_plc_message)){
        udpCenter.sendMotorBossPLCMessage(cue_plc_message);
    }
    
    if(cueCenter.popCueMADRIXSMessage(cue_madrix_message)){
        udpCenter.sendLightBossMARDIXMessage(cue_madrix_message);
    }
    
    if(cueCenter.popCueBBInput(isCueEnable)){
        if(isCueEnable)
            system("say 'Input from Q Lab' &");
        else
            system("say 'Input from Video' &");
        
        ofLogVerbose("bbgod_log") << "Cue bb input from -> " + ofToString(isCueEnable);
    }
    
    if(cueCenter.popCueBBRealtime(isPixelsSendEnable)){
        if(isPixelsSendEnable)
            system("say 'Stream open' &");
        else
            system("say 'Stream close' &");
        
        ofLogVerbose("bbgod_log") << "Cue livestream -> " + ofToString(isPixelsSendEnable);
    }
    
    if(cueCenter.popCueBBMotorEnable(isMotorOn)){
        if(isMotorOn)
            system("say 'Open Motor' &");
        else
            system("say 'Close Motor' &");
        
        ofLogVerbose("bbgod_log") << "Cue motor enable -> " + ofToString(isMotorOn);
    }
    
    if(cueCenter.popCueBBCurveSetting(curve_setting)){
        
        statusCenter.setMotorBezierCurveSetting(curve_setting);
        bamboo.setMotorBezierCurveSetting(curve_setting);
//        for(int i = 0 ; i < 4 ; i++){
//            cout<< curve_setting[i] << endl;
        
    }
    
    if(cueCenter.popCueVideoMessage(cue_video_message)){
        udpCenter.sendVideoBossMessage(cue_video_message);
        
        ofLogVerbose("bbgod_log") << "Cue video -> " + cue_video_message;
    }
    
    
    // *** Update Bamboo from Cue Message ***
    if(isCueEnable){
        
        if(cueCenter.popCueLEDStruct(cue_led_struct)){
            
            if(cue_led_struct.isFlashCue == false){
                for(size_t i = 0 ; i < cue_led_struct.layers_id.size() ; i++){
                    for(size_t j = 0 ; j < cue_led_struct.groups_id.size() ; j++){
                        if(bamboo.getGroupsMaxIndex(cue_led_struct.layers_id[i]) >= cue_led_struct.groups_id[j]){
                            for(size_t k = 0 ; k < cue_led_struct.points_id.size() ; k++){
                                if(bamboo.getPointsMaxIndex(cue_led_struct.layers_id[i], cue_led_struct.groups_id[j]) >= cue_led_struct.points_id[k]){
                                    
                                    bamboo.setLED(cue_led_struct.layers_id[i],
                                                  cue_led_struct.groups_id[j],
                                                  cue_led_struct.points_id[k],
                                                  cue_led_struct.color_mode,
                                                  cue_led_struct.colors[0],
                                                  cue_led_struct.colors[1],
                                                  cue_led_struct.time);
                                    
                                }else{
                                    break;
                                }
                            }
                        }else{
                            break;
                        }
                    }
                }
            }else{
                for(size_t i = 0 ; i < cue_led_struct.layers_id.size() ; i++){
                    for(size_t j = 0 ; j < cue_led_struct.groups_id.size() ; j++){
                        if(bamboo.getGroupsMaxIndex(cue_led_struct.layers_id[i]) >= cue_led_struct.groups_id[j]){
                            for(size_t k = 0 ; k < cue_led_struct.points_id.size() ; k++){
                                if(bamboo.getPointsMaxIndex(cue_led_struct.layers_id[i], cue_led_struct.groups_id[j]) >= cue_led_struct.points_id[k]){
                                    
                                    
                                    bamboo.setLEDFlash(cue_led_struct.layers_id[i],
                                                  cue_led_struct.groups_id[j],
                                                  cue_led_struct.points_id[k],
                                                  cue_led_struct.flash_speed);
                                    
                                }else{
                                    break;
                                }
                            }
                        }else{
                            break;
                        }
                    }
                }
            }
        }
        
        if(cueCenter.popCueMotorStruct(cue_motor_struct)){
            
            if(cue_motor_struct.effectMode == STILL){
                effectCenter.effectMode_U = cue_motor_struct.isUpBamBoo ? STILL : effectCenter.effectMode_U;
                effectCenter.effectMode_D = cue_motor_struct.isUpBamBoo ? effectCenter.effectMode_D : STILL;
                for(size_t i = 0 ; i < cue_motor_struct.id.size() ; i++){
                    bamboo.setMOTOR(cue_motor_struct.isUpBamBoo,
                                    cue_motor_struct.id[i],
                                    cue_motor_struct.angle,
                                    cue_motor_struct.time);
                }
            }else if(cue_motor_struct.effectMode == WAVE){
                effectCenter.setMotorEffectWAVE(cue_motor_struct.group_select, cue_motor_struct.id, cue_motor_struct.crest_size, cue_motor_struct.from_degree, cue_motor_struct.to_degree, cue_motor_struct.sin_period);
            }else if(cue_motor_struct.effectMode == STAIR){
                effectCenter.setMotorEffectSTAIR(cue_motor_struct.group_select, cue_motor_struct.id, cue_motor_struct.from_degree, cue_motor_struct.step_size);
            }
        }
        
        if(effectCenter.effectMode_U == STILL || effectCenter.effectMode_D == STILL){
            vector<float> bamboo_degrees = bamboo.getMotorsDegrees();
            float* motors_degree = effectCenter.getMotorsDegree();
            for(int i = 0 ; i < 54 ; i++){
                if(effectCenter.effectMode_U == STILL)
                    motors_degree[i] = bamboo_degrees[i];
                if(effectCenter.effectMode_D == STILL)
                    motors_degree[i+54] = bamboo_degrees[i+54];
            }
        }
        
        if(effectCenter.effectMode_U != STILL || effectCenter.effectMode_D != STILL){
            effectCenter.update();
            float* motors_degree = effectCenter.getMotorsDegree();
            for(int i = 0 ; i < 54 ; i++){
                
                if(effectCenter.effectMode_U != STILL)
                    bamboo.setMOTOR(true, i+1, motors_degree[i], 0);
                if(effectCenter.effectMode_D != STILL)
                    bamboo.setMOTOR(false, i+1, motors_degree[i+54], 0);
            }
        }
    }
    
    // *** Update BamBoo from VideoBoss ***
    if(!isCueEnable){
        if(dir.size() > 0){
            
//            if(ofGetElapsedTimeMillis() - syphon_timer > (1000/30.0)){
//                fbo.begin();
//                if(dir.serverExists("LED Server", "VideoBoss")){
//
//                    led_client.bind();
//                    led_client.draw(0, 0);
//                    led_client.unbind();
//                }
//
//                if(dir.serverExists("LED White Server", "VideoBoss")){
//
//                    led_white_client.bind();
//                    led_white_client.draw(470, 0);
//                    led_white_client.unbind();
//                }
//                fbo.end();
//
//                if(dir.serverExists("LED Server", "VideoBoss") || dir.serverExists("LED White Server", "VideoBoss")){
//                    led_texture = fbo.getTexture();
//                    led_texture.readToPixels(led_pixels);
//                    bamboo.setLEDFromPixels(led_pixels);
//                }
//
//            }
            
            ofPushStyle();
            ofSetColor(255);
            if(dir.serverExists("LED Server", "VideoBoss"))
                if(ofGetElapsedTimeMillis() - syphon_timer > (1000/30.0)){
//                    led_client.bind();
                    fbo.begin();
                    led_client.draw(0, 0);
                    fbo.end();
//                    led_client.unbind();

                    led_texture = fbo.getTexture();
                    led_texture.readToPixels(led_pixels);
                    bamboo.setLEDFromPixels(led_pixels);

                    syphon_timer = ofGetElapsedTimeMillis();

                }

            if(dir.serverExists("LED White Server", "VideoBoss"))
                if(ofGetElapsedTimeMillis() - syphon_white_timer > (1000/30.0)){
                    
//                    led_white_client.bind();
                    white_fbo.begin();
                    led_white_client.draw(0, 0);
                    white_fbo.end();
//                    led_white_client.unbind();

                    led_white_texture = white_fbo.getTexture();
                    led_white_texture.readToPixels(led_white_pixels);
                    bamboo.setLEDWhiteFromPixels(led_white_pixels);

                    syphon_white_timer = ofGetElapsedTimeMillis();
                }
            
            
            if(dir.serverExists("Motor Server", "VideoBoss"))
                if(ofGetElapsedTimeMillis() - syphon_motor_timer > (1000/30.0)){
//                    motor_client.bind();
                    motor_fbo.begin();
                    motor_client.draw(0, 0);
                    motor_fbo.end();
//                    motor_client.unbind();
                    
                    motor_texture = motor_fbo.getTexture();
                    motor_texture.readToPixels(motor_pixels);
                    bamboo.setMotorFromPixels(motor_pixels);
                    
                    syphon_motor_timer = ofGetElapsedTimeMillis();
                    
                }
            
            
            if(dir.serverExists("Processing Syphon", "Bamboo_motor_video"))
                if(ofGetElapsedTimeMillis() - syphon_motor_timer > (1000/30.0)){
//                    motor_client.bind();
                    motor_fbo.begin();
                    motor_client.draw(0, 0);
                    motor_fbo.end();
//                    motor_client.unbind();
                    
                    motor_texture = motor_fbo.getTexture();
                    motor_texture.readToPixels(motor_pixels);
                    bamboo.setMotorFromPixels(motor_pixels);
                    
                    syphon_motor_timer = ofGetElapsedTimeMillis();
                    
                }
            ofPopStyle();
        }
    }
    
    // *** Update status messages ***
    string message;
    cueCenter.popStatusMessage(message);
    statusCenter.update();
    statusCenter.updateStatusMessages(message);
    statusCenter.setBamBooGodStatus(isCueEnable);
    statusCenter.setMotorBossStatus(udpCenter.isMotorBossTx, udpCenter.isMotorBossRx,isMotorOn,udpCenter.motor_boss_json);
    statusCenter.setLightBossStatus(udpCenter.isLightBossTx,udpCenter.isLightBossRx, isPixelsSendEnable);
    statusCenter.setUDPSetting(udpCenter.udp_motor_boss_ip,
                                udpCenter.udp_light_boss_ip,
                                udpCenter.udp_qlab_ip,
                                udpCenter.udp_qlab_recv_port,
                                udpCenter.udp_motor_boss_send_port,
                                udpCenter.udp_motor_boss_recv_port,
                                udpCenter.udp_motor_boss_send_port_plc,
                                udpCenter.udp_motor_boss_recv_port_plc,
                                udpCenter.udp_light_boss_send_port,
                                udpCenter.udp_light_boss_recv_port,
                                udpCenter.udp_light_boss_send_port_pixels,
                                udpCenter.udp_video_boss_send_port);
    
    statusCenter.setSyphonStatus(isSyphonLightOpened, isSyphonLightWhiteOpened, isSyphonMotorOpened);

    
    // *** Update pixels to LightBoss ***
    if(isPixelsSendEnable){
        udpCenter.sendLightBossPixels(bamboo.led_udp_format);
    }

    
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(ofColor(0, 0, 0));
    
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, -1500);
    
    ofPushMatrix();
    
    if(isAutoRotationEnable)
        perspective.x += TWO_PI/100.0;
    
    ofRotateXDeg(-perspective.y);
    ofRotateYDeg(perspective.x);
    
    ofPushStyle();
    
    if(isGridEnable){
        ofSetColor(255,30);
        for(int i = 0 ; i < 21 ; i++){
            
            ofDrawLine(-1000 + i * 100, -500, -500,
                       -1000 + i * 100 , 500, -500);
            
            ofDrawLine(-1000+i * 100, 500, -500,
                       -1000+i * 100, 500, 500);
            
            if(i <11){
                ofDrawLine(-1000 , -500 + i * 100, -500,
                           1000 , -500 + i * 100, -500);
                
                ofDrawLine(-1000, 500, -500+i * 100,
                           1000, 500,  -500+i * 100);
            }
        }
        
        
        for(int i = 0 ; i < 12 ; i++){
            if(i == 0 || i == 3)
                ofSetColor(255,0,0);
            else
                ofSetColor(255,100);
            ofDrawRectangle(glm::vec3(sin(TWO_PI/12*i)*-500,500,cos(TWO_PI/12*i)*500), 5,10);
        }
    }
    
    ofPopStyle();
    
    ofRotateXDeg(90);

    bamboo.draw();
    
    ofPopMatrix();
    
    ofTranslate(-500, 535 ,0);
    //ofDrawAxis(100);
    ofPopMatrix();
    
    ofSetColor(255,255,255);
    
    statusCenter.draw();
    
    
    ofSetColor(150);
    if(isMotorCurrentMessageShow){
        vector<float> bamboo_degree = bamboo.getMotorsDegrees();
        for(int i = 0 ; i < bamboo_degree.size() ; i++)
            if(i < 54)
                ofDrawBitmapString(ofToString(i) + " : " + ofToString(bamboo_degree[i]), 10, 10 + 10*i);
            else
                ofDrawBitmapString(ofToString(i) + " : " + ofToString(bamboo_degree[i]), ofGetWidth()-100, 10 + 10*(i-54));
    }

}

void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& adir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<adir.serverName <<" | App Name: "<<adir.appName;
        
        if(adir.serverName == "LED Server"){
            led_client.set(adir);
            isSyphonLightOpened = true;
        }else if(adir.serverName == "Motor Server"){
            motor_client.set(adir);
            isSyphonMotorOpened = true;
        }else if(adir.serverName == "LED White Server"){
            led_white_client.set(adir);
            isSyphonLightWhiteOpened = true;
        }else if(adir.serverName == "Processing Syphon"){
            motor_client.set(adir);
        }
    }
    dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        
        if(dir.serverName == "LED Server"){
            isSyphonLightOpened = false;
        }else if(dir.serverName == "Motor Server"){
            isSyphonMotorOpened = false;
        }else if(dir.serverName == "LED White Server"){
            isSyphonLightWhiteOpened = false;
        }else if(dir.serverName == "Processing Syphon"){
//            motor_client.set(adir);
        }
    }
    dirIdx = 0;
}

void ofApp::exit()
{
    // videoThread.stopThread();
    udpCenter.exit();
    cueCenter.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
//    cout<<key <<endl;
    switch(key){
        case 32:{
            isMotorOn = false;
            //            udpCenter.sendMotorBossPLCStatus(isMotorOn);
            string message = "[K] : Close Motors.";
            statusCenter.updateStatusMessages(message);
            
            system("say 'Motor CLOSE' &");
            
            ofLog(OF_LOG_VERBOSE, "Pressed ctrl + x");
            ofLogVerbose("bbgod_log") << "Pressed ctrl + x";
            break;
        }
        case 5:{ //ctrl + e
            isMotorOn = true;
            //            udpCenter.sendMotorBossPLCStatus(isMotorOn);
            string message = "[K] : Open Motors.";
            statusCenter.updateStatusMessages(message);
            
            system("say 'Motors ON' &");
            
            ofLogVerbose("bbgod_log") << "Pressed ctrl + z";
            break;
        }
        case 3:{ // ctrl + c
            isCueEnable = !isCueEnable;
            string message = isCueEnable ? "[K] : Input from cue." : "[K] : Input from video.";
            statusCenter.updateStatusMessages(message);
            
            if(isCueEnable)
                system("say 'Input from Q Lab' &");
            else
                system("say 'Input from Video' &");
            
            ofLogVerbose("bbgod_log") << "Pressed ctrl + c ->" + ofToString(isCueEnable);
            break;
        }
        case 7:{ // ctrl + g
            isGridEnable = !isGridEnable;
            ofLogVerbose("bbgod_log") << "Pressed ctrl + g ->" + ofToString(isGridEnable);
            break;
        }
        case 19:{ // ctrl + s
            statusCenter.enableStatusMessage();
            ofLogVerbose("bbgod_log") << "Pressed ctrl + s";
            break;
        }
        case 8:{ // ctrl + h
            statusCenter.enableHelperMessage();
            break;
        }
        case 4:{ // ctrl + d
            bamboo.setBamBooDrew();
            break;
        }
        case 18:{ // ctrl + r
            isAutoRotationEnable = !isAutoRotationEnable;
            break;
        }
        case 14:{ // crtl + n
            isPixelsSendEnable = !isPixelsSendEnable;
            string message = isPixelsSendEnable ? "[K] : MADRIX livestream enable." : "[K] : MADRIX livestream disable.";
            statusCenter.updateStatusMessages(message);
            
            if(isPixelsSendEnable)
                system("say 'Stream on' &");
            else
                system("say 'Stream off' &");
            
            ofLogVerbose("bbgod_log") << "Pressed ctrl + n ->" + ofToString(isPixelsSendEnable);
            break;
        }
        case 15:{ // ctrl+o
            isAutoRotationEnable = false;
            perspective.x = 0;
            perspective.y = 0;
            break;
        }
        case 13:{
            isMotorCurrentMessageShow = !isMotorCurrentMessageShow;
            break;
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    end_position = ofVec3f(mouseX, mouseY, 0);
    ofVec3f result = end_position - start_position;
    result.x = ofMap(result.x, 0, 1024, 0, TWO_PI * 5);
    result.y = ofMap(result.y, 0, 768, 0, TWO_PI * 5);
    
    perspective = perspective + result - last_rotation;
    
    last_rotation = result;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    start_position = ofVec3f(x, y, 0);
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    last_rotation = ofVec3f(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
