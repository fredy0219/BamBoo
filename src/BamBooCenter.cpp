//
//  BamBoo.cpp
//  BamBooGod
//
//  Created by Fish on 2020/12/29.
//

#include <stdio.h>
#include "BamBooCenter.h"

LED_Point::LED_Point(glm::vec3 _position){
    position = _position;
    color_mode = 0;
    color = ofColor(0,0,0);
    start_color = ofColor(0,0,0);
    target_color = ofColor(0,0,0);
    
    white_color = 0;
    start_white_color = 0;
    target_white_color = 0;
}

void LED_Point::draw(){
    
    ofPushStyle();
    ofSetColor(color);
    ofDrawCircle(position, 3);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(white_color);
    ofDrawCircle(position, 2);
    ofPopStyle();
}

void LED_Point::update(){
    if(color_mode == 0){
        if(color != target_color || white_color!= target_white_color){
            if(ofGetElapsedTimeMillis() - start_time < target_time){
                if(ofGetElapsedTimeMillis() - current_time > 1000/40){
                    
                    float increment = ofMap((ofGetElapsedTimeMillis() - start_time)/40,0,target_time/40,0,1);
                    color = start_color.getLerped(target_color, increment);
                    white_color = ofLerp(start_white_color, target_white_color, increment);
                }
            }
            else{
                color = target_color;
                white_color = target_white_color;
            }
        }
    }
    else if(color_mode == 1){
        if(color != ofColor::fromHsb(target_color.r, target_color.g, target_color.b)|| white_color!= target_white_color){
            if(ofGetElapsedTimeMillis() - start_time < target_time){
                if(ofGetElapsedTimeMillis() - current_time > 1000/40){
                    
                    float increment = ofMap((ofGetElapsedTimeMillis() - start_time)/40,0,target_time/40,0,1);
                    
                    ofColor c = start_color.getLerped(target_color,increment);
                    color = ofColor::fromHsb(c.r, c.g, c.b);
                    white_color = ofLerp(start_white_color, target_white_color, increment);
                }
            }
            else{
                color = ofColor::fromHsb(target_color.r, target_color.g, target_color.b);
                white_color = target_white_color;
            }
        }
    }
}

void LED_Point::setLED(uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time){
    color_mode = _color_mode;
//    if(color_mode == 0)
//        color = _start_color;
//    else if(color_mode == 1)
//        color = ofColor::fromHsb(_start_color.r, _start_color.g, _start_color.b);
    start_color = ofColor(_start_color.r, _start_color.g, _start_color.b);
    target_color = ofColor(_target_color.r, _target_color.g, _target_color.b);
    
    start_white_color = _start_color.a;
    target_white_color = _target_color.a;
    
    target_time = _time;
    start_time = ofGetElapsedTimeMillis();
    current_time = ofGetElapsedTimeMillis();
}

void LED_Point::setLEDRGB(ofColor color){
    start_color = color;
    target_color = color;
}

void LED_Point::setLEDWhite(uint8_t white){
//    white_color = white;
    start_white_color = white;
    target_white_color = white;
}

void LED_Point::add2DPoint(glm::vec2 _position_2D){
    position_2D = _position_2D;
}

glm::vec2 LED_Point::get2DPoint(){
    return position_2D;
}

ofColor LED_Point::getColor(){
    return color;
}

uint8_t LED_Point::getColorWhite(){
    return white_color;
}

//
LED_Group::LED_Group(){
}

void LED_Group::draw(){
    
    unordered_map<uint8_t, LED_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++)
        iter->second.draw();
}

void LED_Group::update(){
    
    unordered_map<uint8_t, LED_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++)
        iter->second.update();
}

void LED_Group::addPoint(uint8_t _id, glm::vec3 _position){
    LED_Point new_point(_position);
    points.insert(pair<uint8_t,LED_Point>(_id,new_point));
}

void LED_Group::setLED(uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time){
    
    auto point = points.find(_point_id);
    
    if(point != points.end())
        point->second.setLED(_color_mode, _start_color,_target_color,_time);
}

void LED_Group::add2DPoint(uint8_t _point_id, glm::vec2 _position_2D){
    
    points.find(_point_id)->second.add2DPoint(_position_2D);
}

glm::vec2 LED_Group::get2DPoint(uint8_t _point_id){
    
    auto point = points.find(_point_id);
    
    if(point != points.end())
        return points.find(_point_id)->second.get2DPoint();
    else
        return glm::vec2(0,0);
}

ofColor LED_Group::getColor(uint8_t _point_id){
    
    auto point = points.find(_point_id);
    
    if(point != points.end())
        return points.find(_point_id)->second.getColor();
    else
        return ofColor(0,0,0);
}

uint8_t LED_Group::getColorWhite(uint8_t _point_id){
    
    auto point = points.find(_point_id);
    
    if(point != points.end())
        return points.find(_point_id)->second.getColorWhite();
    else
        return 0;
}

void LED_Group::setLEDFromPixels(ofPixels &pixels){
    unordered_map<uint8_t, LED_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++){
        glm::vec2 tex_position = iter->second.position_2D;
        ofColor c = pixels.getColor(tex_position.x ,tex_position.y);
//        iter->second.setLED(0 ,c, c, 0); //color_mode, start_color, end_color, time
        iter->second.setLEDRGB(c);
    }
}

void LED_Group::setLEDWhiteFromPixels(ofPixels &pixels){
    unordered_map<uint8_t, LED_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++){
        glm::vec2 tex_position = iter->second.position_2D;
        ofColor c = pixels.getColor(tex_position.x ,tex_position.y);
        iter->second.setLEDWhite((uint8_t)(c.getHex()>>16));
    }
}


// LED Layer
LED_Layer::LED_Layer(){
}

void LED_Layer::draw(){
    unordered_map<uint8_t, LED_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.draw();
}

void LED_Layer::update(){
    unordered_map<uint8_t, LED_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.update();
    
}

void LED_Layer::addGroup(uint8_t _group_id){
    
    auto group = groups.find(_group_id);
    if(group == groups.end()){
        LED_Group new_group;
        groups.insert(pair<uint8_t, LED_Group>(_group_id,new_group));
    }
    
}

void LED_Layer::addPoint(uint8_t _group_id, uint8_t _point_id, glm::vec3 _position){
    
    groups.find(_group_id)->second.addPoint(_point_id, _position);
}

void LED_Layer::setLED(uint8_t _group_id, uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time){
    groups.find(_group_id)->second.setLED(_point_id, _color_mode, _start_color, _target_color, _time);
}

void LED_Layer::add2DPoint(uint8_t _group_id, uint8_t _point_id, glm::vec2 _position_2D){
    
    groups.find(_group_id)->second.add2DPoint(_point_id, _position_2D);
}

glm::vec2 LED_Layer::get2DPoint(uint8_t _group_id
                                , uint8_t _point_id){
    return groups.find(_group_id)->second.get2DPoint(_point_id);
}

ofColor LED_Layer::getColor(uint8_t _group_id, uint8_t _point_id){
    return groups.find(_group_id)->second.getColor(_point_id);
}

uint8_t LED_Layer::getColorWhite(uint8_t _group_id, uint8_t _point_id){
    return groups.find(_group_id)->second.getColorWhite(_point_id);
}

void LED_Layer::setLEDFromPixels(ofPixels &pixels){
    
    unordered_map<uint8_t, LED_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.setLEDFromPixels(pixels);
}

void LED_Layer::setLEDWhiteFromPixels(ofPixels &pixels){
    
    unordered_map<uint8_t, LED_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.setLEDWhiteFromPixels(pixels);
}

// LED Wash Point

void LED_Wash_Point::add2DPointFlash(glm::vec2 _position_2D_flash){
    position_2D_flash = _position_2D_flash;
}

void LED_Wash_Point::setFlash(bool _isFlashEnable){
    isFlashEnable = _isFlashEnable;
}

void LED_Wash_Point::setFlashSpeed(uint8_t brightness){
    flashSpeed = (uint64_t)ofMap(brightness,1,255,3000,200);
}

void LED_Wash_Point::draw(){
    
    ofPushStyle();
    if(isFlashEnable){
        if(isFlashTrigger)
            LED_Point::draw();
    }else{
        LED_Point::draw();
    }
    
//    ofDrawCircle(position, 3);
    ofPopStyle();
}

void LED_Wash_Point::update(){
    
    LED_Point::update();
    
    if(isFlashEnable){
        if(ofGetElapsedTimeMillis() - flashTime > flashSpeed){
            isFlashTrigger = !isFlashTrigger;
            flashTime = ofGetElapsedTimeMillis();
        }
    }
}

glm::vec2 LED_Wash_Point::get2DPointFlash(){
    return position_2D_flash;
}

ofColor LED_Wash_Point::getColorFlash(){
    return colorFlash;
}

void LED_Wash_Point::setColorFlash(ofColor c){
    colorFlash = c;
}

// LED Wash Group
LED_Wash_Group::LED_Wash_Group(){
}

void LED_Wash_Group::draw(){
    
    unordered_map<uint8_t, LED_Wash_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++)
        iter->second.draw();
}

void LED_Wash_Group::update(){
    
    unordered_map<uint8_t, LED_Wash_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++)
        iter->second.update();
}

void LED_Wash_Group::addPoint(uint8_t _id, glm::vec3 _position){
    LED_Wash_Point new_point(_position);
    points.insert(pair<uint8_t, LED_Wash_Point>(_id,new_point));
}

void LED_Wash_Group::setLED(uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time){
    points.find(_point_id)->second.setLED(_color_mode, _start_color,_target_color,_time);
}

void LED_Wash_Group::setLEDFlash(uint8_t _point_id
                                 , uint8_t _flash_speed){
    
    
    if(_flash_speed == 0){
        points.find(_point_id)->second.setFlash(false);
        points.find(_point_id)->second.setFlashSpeed(0);
        points.find(_point_id)->second.setColorFlash(ofColor(0,0,0));
    }else{
        points.find(_point_id)->second.setFlash(true);
        points.find(_point_id)->second.setFlashSpeed(_flash_speed);
        points.find(_point_id)->second.setColorFlash(ofColor(_flash_speed,0,0));
    }
}

void LED_Wash_Group::add2DPoint(uint8_t _point_id, glm::vec2 _position_2D){
    points.find(_point_id)->second.add2DPoint(_position_2D);
    
}

void LED_Wash_Group::add2DPointFlash(uint8_t _point_id,glm::vec2 _position_2D_flash){
    
    
    points.find(_point_id)->second.add2DPointFlash(_position_2D_flash);
}
glm::vec2 LED_Wash_Group::get2DPoint(uint8_t _point_id){
    return points.find(_point_id)->second.get2DPoint();
}

glm::vec2 LED_Wash_Group::get2DPointFlash(uint8_t _point_id){
    return points.find(_point_id)->second.get2DPointFlash();
}

ofColor LED_Wash_Group::getColor(uint8_t _point_id){
    return points.find(_point_id)->second.getColor();
}

uint8_t LED_Wash_Group::getColorWhite(uint8_t _point_id){
    return points.find(_point_id)->second.getColorWhite();
}

ofColor LED_Wash_Group::getColorFlash(uint8_t _point_id){
    return points.find(_point_id)->second.getColorFlash();

}

void LED_Wash_Group::setLEDFromPixels(ofPixels &pixels){
    unordered_map<uint8_t, LED_Wash_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++){
        
        ofColor c = pixels.getColor(iter->second.position_2D.x ,iter->second.position_2D.y);
//        iter->second.setLED(0, c, c, 0);
        iter->second.setLEDRGB(c);
        
        // set flash
//        c = pixels.getColor(iter->second.position_2D_flash.x, iter->second.position_2D_flash.y);
        
        //        float brightness = (0.299*c.r)+(0.587*c.g)+(0.114*c.b);
        //        if(brightness > 127)
        //            iter->second.setFlash(true);
        //        else
        //            iter->second.setFlash(false);
        
    }
    
}

void LED_Wash_Group::setLEDWhiteFromPixels(ofPixels &pixels){
    unordered_map<uint8_t, LED_Wash_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++){
        
        ofColor c = pixels.getColor(iter->second.position_2D.x ,iter->second.position_2D.y);
        iter->second.setLEDWhite((uint8_t)(c.getHex()>>16));

        // flash
        c = pixels.getColor(iter->second.position_2D_flash.x, iter->second.position_2D_flash.y);
        uint8_t flash_freqence = (uint8_t)(c.getHex()>>16);
        
        if(flash_freqence != 0){
            iter->second.setFlash(true);
            iter->second.setColorFlash(flash_freqence);
            iter->second.setFlashSpeed(flash_freqence);
        }else{
            iter->second.setFlash(false);
            iter->second.setColorFlash(flash_freqence);
            iter->second.setFlashSpeed(flash_freqence);
        }

    }
    
}

// LED Wash
LED_Wash_Layer::LED_Wash_Layer(){
}

void LED_Wash_Layer::draw(){
    
    unordered_map<uint8_t, LED_Wash_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.draw();
}

void LED_Wash_Layer::update(){
    
    unordered_map<uint8_t, LED_Wash_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.update();

}

void LED_Wash_Layer::addGroup(uint8_t _group_id){
    
    auto group = groups.find(_group_id);
    if(group == groups.end()){
        LED_Wash_Group new_group;
        groups.insert(pair<uint8_t, LED_Wash_Group>(_group_id,new_group));
    }
    
}

void LED_Wash_Layer::addPoint(uint8_t _group_id, uint8_t _point_id, glm::vec3 _position){
    
    groups.find(_group_id)->second.addPoint(_point_id, _position);
}

void LED_Wash_Layer::setLED(uint8_t _group_id, uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time){
    groups.find(_group_id)->second.setLED(_point_id, _color_mode, _start_color, _target_color, _time);
}

void LED_Wash_Layer::setLEDFlash(uint8_t _group_id
                                 , uint8_t _point_id
                                 , uint8_t _flash_speed){
    groups.find(_group_id)->second.setLEDFlash(_point_id, _flash_speed);
}

void LED_Wash_Layer::add2DPoint(uint8_t _group_id, uint8_t _point_id, glm::vec2 _position_2D){

    groups.find(_group_id)->second.add2DPoint(_point_id, _position_2D);
}

glm::vec2 LED_Wash_Layer::get2DPoint(uint8_t _group_id
                                , uint8_t _point_id){
    return groups.find(_group_id)->second.get2DPoint(_point_id);
}

glm::vec2 LED_Wash_Layer::get2DPointFlash(uint8_t _group_id
                                     , uint8_t _point_id){
    return groups.find(_group_id)->second.get2DPointFlash(_point_id);
}

ofColor LED_Wash_Layer::getColor(uint8_t _group_id
                                 , uint8_t _point_id){
    return groups.find(_group_id)->second.getColor(_point_id);
}

uint8_t LED_Wash_Layer::getColorWhite(uint8_t _group_id
                                 , uint8_t _point_id){
    return groups.find(_group_id)->second.getColorWhite(_point_id);
}

ofColor LED_Wash_Layer::getColorFlash(uint8_t _group_id, uint8_t _point_id){
    
    return groups.find(_group_id)->second.getColorFlash(_point_id);
}

void LED_Wash_Layer::add2DPointFlash(uint8_t _group_id, uint8_t _point_id,glm::vec2 _position_2D_flash){
    groups.find(_group_id)->second.add2DPointFlash(_point_id, _position_2D_flash);
}

void LED_Wash_Layer::setLEDFromPixels(ofPixels &pixels){
    
    unordered_map<uint8_t, LED_Wash_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.setLEDFromPixels(pixels);
}

void LED_Wash_Layer::setLEDWhiteFromPixels(ofPixels &pixels){
    
    unordered_map<uint8_t, LED_Wash_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.setLEDWhiteFromPixels(pixels);
}

// BamBoo

BamBoo_Point::BamBoo_Point(glm::vec3 _position){
    position = _position;
    color_mode = 0;
    color = ofColor(0,0,0);
    start_color = ofColor(0,0,0);
    target_color = ofColor(0,0,0);
    
    white_color = 0;
    start_white_color = 0;
    target_white_color = 0;

}

void BamBoo_Point::draw(){
    
    ofPushStyle();
    ofSetColor(color,200);
    ofDrawCircle(position, 3);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(white_color,200);
    ofDrawCircle(position, 2);
    ofPopStyle();
}

void BamBoo_Point::update(){
    
    
    if(color_mode == 0){
        if(color != target_color || white_color!= target_white_color){
            if(ofGetElapsedTimeMillis() - start_time < target_time){
                if(ofGetElapsedTimeMillis() - current_time > 1000/40){
                    
                    float increment = ofMap((ofGetElapsedTimeMillis() - start_time)/40,0,target_time/40,0,1);
                    color = start_color.getLerped(target_color, increment);
                    white_color = ofLerp(start_white_color, target_white_color, increment);
                }
            }
            else{
                color = target_color;
                white_color = target_white_color;
            }
        }
    }
    else if(color_mode == 1){
        if(color != ofColor::fromHsb(target_color.r, target_color.g, target_color.b) || white_color!= target_white_color){
            if(ofGetElapsedTimeMillis() - start_time < target_time){
                if(ofGetElapsedTimeMillis() - current_time > 1000/40){
                    
                    float increment = ofMap((ofGetElapsedTimeMillis() - start_time)/40,0,target_time/40,0,1);
                    
                    ofColor c = start_color.getLerped(target_color,increment);
                    color = ofColor::fromHsb(c.r, c.g, c.b);
                    white_color = ofLerp(start_white_color, target_white_color, increment);
                }
            }
            else{
                color = ofColor::fromHsb(target_color.r, target_color.g, target_color.b);
                white_color = target_white_color;
            }
        }
    }
    
    
}

void BamBoo_Point::setLED(uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time){
    
    color_mode = _color_mode;
    
//    if(color_mode == 0)
//        color = _start_color;
//    else if(color_mode == 1)
//        color = ofColor::fromHsb(_start_color.r, _start_color.g, _start_color.b);
    start_color = ofColor(_start_color.r, _start_color.g, _start_color.b);
    target_color = ofColor(_target_color.r, _target_color.g, _target_color.b);
    
    start_white_color = _start_color.a;
    target_white_color = _target_color.a;
    
    target_time = _time;
    start_time = ofGetElapsedTimeMillis();
    current_time = ofGetElapsedTimeMillis();
}

void BamBoo_Point::setLEDRGB(ofColor color){
    start_color = color;
    target_color = color;
}

void BamBoo_Point::setLEDWhite(uint8_t white){
//    white_color = white;
    start_white_color = white;
    target_white_color = white;
}

void BamBoo_Point::add2DPoint(glm::vec2 _position_2D){
    position_2D = _position_2D;
}

glm::vec2 BamBoo_Point::get2DPoint(){
    return position_2D;
}
ofColor BamBoo_Point::getColor(){
    return color;
}

uint8_t BamBoo_Point::getColorWhite(){
    return white_color;
}

//

BamBoo_Group::BamBoo_Group(){
}

void BamBoo_Group::draw(){
    
    unordered_map<uint8_t, BamBoo_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++)
        iter->second.draw();
}

void BamBoo_Group::update(){
    unordered_map<uint8_t, BamBoo_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++)
        iter->second.update();
}

void BamBoo_Group::addPoint(uint8_t _id, glm::vec3 _position){
    BamBoo_Point new_point(_position);
    points.insert(pair<uint8_t,BamBoo_Point>(_id,new_point));
}


void BamBoo_Group::setLED(uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time){
    
    points.find(_point_id)->second.setLED(_color_mode, _start_color,_target_color,_time);
}

void BamBoo_Group::add2DPoint(uint8_t _point_id, glm::vec2 _position_2D){
    
    points.find(_point_id)->second.add2DPoint(_position_2D);
}

glm::vec2 BamBoo_Group::get2DPoint(uint8_t _point_id){
    return points.find(_point_id)->second.get2DPoint();
}
ofColor BamBoo_Group::getColor(uint8_t _point_id){
    return points.find(_point_id)->second.getColor();
}

uint8_t BamBoo_Group::getColorWhite(uint8_t _point_id){
    return points.find(_point_id)->second.getColorWhite();
}

void BamBoo_Group::setLEDFromPixels(ofPixels &pixels){
    unordered_map<uint8_t, BamBoo_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++){
        glm::vec2 tex_position = iter->second.position_2D;
        ofColor c = pixels.getColor(tex_position.x ,tex_position.y);
//        ofColor cw = pixels.getColor(tex_position.x+470 ,tex_position.y);
//        iter->second.setLED(0, c, c, 0);
        iter->second.setLEDRGB(c);
        
    }
}

void BamBoo_Group::setLEDWhiteFromPixels(ofPixels &pixels){
    unordered_map<uint8_t, BamBoo_Point>::iterator iter;
    for(iter = points.begin(); iter != points.end(); iter++){
        glm::vec2 tex_position = iter->second.position_2D;
        ofColor c = pixels.getColor(tex_position.x ,tex_position.y);
        iter->second.setLEDWhite((uint8_t)(c.getHex()>>16));
    }
}


BamBoo_Layer::BamBoo_Layer(){
}

void BamBoo_Layer::drawLED(){
    
    ofPushMatrix();
    
    ofTranslate(rotation_point);
    ofRotateZDeg(360+degree_origin);
    ofRotateXDeg(-degree_draw);
    unordered_map<uint8_t, BamBoo_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.draw();
    
    ofPopMatrix();
    
}
void BamBoo_Layer::drawBamBoo(int mode){
    
    float rotation_axis_length = 26.0581;
    float stick_start_length = 89.974;
    float stick_end_length = 480.019;
    int direction = isUpBamBoo? 1 : -1;
   
    if(mode == 0 || mode == 1){
        ofPushMatrix();
        ofPushStyle();
        ofSetColor(50);
        
        ofTranslate(rotation_point);
        ofRotateZDeg(360+degree_origin+90);
        
        ofRotateYDeg(degree_draw);
        ofDrawLine(glm::vec3(0,0,0) , glm::vec3(1,0,0) * rotation_axis_length);

        ofDrawLine(glm::vec3(1,0,0) * rotation_axis_length + glm::vec3(0,0,-direction) * stick_start_length, glm::vec3(1,0,0) * rotation_axis_length);
    //
        ofDrawLine(glm::vec3(1,0,0) * rotation_axis_length + glm::vec3(0,0,direction) * stick_end_length, glm::vec3(1,0,0) * rotation_axis_length);
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    if(mode == 0 || mode == 2){
        ofPushMatrix();
        ofPushStyle();
        ofSetColor(82,154,69,150);
        
        ofTranslate(rotation_point);
        ofRotateZDeg(360+degree_origin+90);
        
        ofRotateYDeg(degree_motorboss_draw);
        ofDrawLine(glm::vec3(0,0,0) , glm::vec3(1,0,0) * rotation_axis_length);
        
        ofDrawLine(glm::vec3(1,0,0) * rotation_axis_length + glm::vec3(0,0,-direction) * stick_start_length, glm::vec3(1,0,0) * rotation_axis_length);
        //
        ofDrawLine(glm::vec3(1,0,0) * rotation_axis_length + glm::vec3(0,0,direction) * stick_end_length, glm::vec3(1,0,0) * rotation_axis_length);
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    
    

}

void BamBoo_Layer::update(){
    unordered_map<uint8_t, BamBoo_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.update();
    
    
//    if(degree_motor != degree_target){
//        if(ofGetElapsedTimeMillis() - start_time < target_time){
//            if(ofGetElapsedTimeMillis() - current_time > 1000/40){
//
//                degree_motor = ofLerpDegrees(degree_start, degree_target, ofMap((ofGetElapsedTimeMillis() - start_time),0,target_time,0,1));
//                current_time = ofGetElapsedTimeMillis();
//            }
//        }
//        else{
//            degree_motor = degree_target;
//        }
//    }
    if(degree_motor != degree_target){
        if(ofGetElapsedTimeMillis() - start_time < target_time){
            if(ofGetElapsedTimeMillis() - current_time > 1000/40){
                float precent = ofMap((ofGetElapsedTimeMillis() - start_time),0,target_time,0,1);
//                degree_motor = degree_start + (degree_target - degree_start) * bezier_curve(bezier_p1x,bezier_p1y, bezier_p2x,bezier_p2y, precent);
                
                if(isUpBamBoo){
                    if(degree_motor >= 90)
                        degree_motor = ofMap(degree_start + (degree_target - degree_start) * bezier_curve(bezier_p1x,bezier_p1y, bezier_p2x,bezier_p2y, precent), 90, MOTOR_VALUE_MAX , 90, MOTOR_VALUE_LIMIT_MAX);
                    else if(degree_motor <= 0){
                        degree_motor = ofMap(degree_start + (degree_target - degree_start) * bezier_curve(bezier_p1x,bezier_p1y, bezier_p2x,bezier_p2y, precent),0, MOTOR_VALUE_MIN , 0, MOTOR_VALUE_LIMIT_MIN);
                    }else{
                        degree_motor = degree_start + (degree_target - degree_start) * bezier_curve(bezier_p1x,bezier_p1y, bezier_p2x,bezier_p2y, precent);
                    }
                }else{
                    if(degree_motor <= -90)
                        degree_motor = ofMap(degree_start + (degree_target - degree_start) * bezier_curve(bezier_p1x,bezier_p1y, bezier_p2x,bezier_p2y, precent), -90, -MOTOR_VALUE_MAX , -90, -MOTOR_VALUE_LIMIT_MAX);
                    else if(degree_motor >= 0){
                        degree_motor = ofMap(degree_start + (degree_target - degree_start) * bezier_curve(bezier_p1x,bezier_p1y, bezier_p2x,bezier_p2y, precent),0, -MOTOR_VALUE_MIN , 0, -MOTOR_VALUE_LIMIT_MIN);
                    }else{
                        degree_motor = degree_start + (degree_target - degree_start) * bezier_curve(bezier_p1x,bezier_p1y, bezier_p2x,bezier_p2y, precent);
                    }
                }
                
                current_time = ofGetElapsedTimeMillis();
            }
        }else{
            
            if(isUpBamBoo){
                if(degree_motor >= 90)
                    degree_motor = ofMap(degree_target,90,MOTOR_VALUE_MAX,90,MOTOR_VALUE_LIMIT_MAX);
                else if(degree_motor <= 0){
                    degree_motor = ofMap(degree_target,0,MOTOR_VALUE_MIN, 0 ,MOTOR_VALUE_LIMIT_MIN);
                }else{
                    degree_motor = degree_target;
                }
            }else{
                if(degree_motor <= -90)
                    degree_motor = ofMap(degree_target,-90,-MOTOR_VALUE_MAX,-90,-MOTOR_VALUE_LIMIT_MAX);
                else if(degree_motor >= 0){
                    degree_motor = ofMap(degree_target,0,-MOTOR_VALUE_MIN,0,-MOTOR_VALUE_LIMIT_MIN);
                }else{
                    degree_motor = degree_target;
                }
            }
        }
    }

    
    if(isUpBamBoo){
        degree_draw = 90-degree_motor;
        degree_motorboss_draw = 90 - degree_motorboss;
        
//
    }else{
        degree_draw = -degree_motor-90;
        degree_motorboss_draw = -degree_motorboss-90;
    }
}

void BamBoo_Layer::addGroup(uint8_t _group_id){
    auto group = groups.find(_group_id);
    if(group == groups.end()){
        BamBoo_Group new_group;
        groups.insert(pair<uint8_t, BamBoo_Group>(_group_id,new_group));
    }
}

void BamBoo_Layer::addPoint(uint8_t _group_id
                            , uint8_t _point_id
                            , glm::vec3 _position){
    
    groups.find(_group_id)->second.addPoint(_point_id, _position);
}

void BamBoo_Layer::setLED(uint8_t _group_id
                          , uint8_t _point_id
                          , uint8_t _color_mode
                          , ofColor _start_color
                          , ofColor _target_color
                          , uint16_t _time){

    groups.find(_group_id)->second.setLED(_point_id, _color_mode, _start_color, _target_color, _time);
    
}

void BamBoo_Layer::add2DPoint(uint8_t _group_id
                              , uint8_t _point_id
                              , glm::vec2 _position_2D){
    groups.find(_group_id)->second.add2DPoint(_point_id, _position_2D);
}

glm::vec2 BamBoo_Layer::get2DPoint(uint8_t _group_id, uint8_t _point_id){
    return groups.find(_group_id)->second.get2DPoint(_point_id);
}
ofColor BamBoo_Layer::getColor(uint8_t _group_id, uint8_t _point_id){
    return groups.find(_group_id)->second.getColor(_point_id);
}

uint8_t BamBoo_Layer::getColorWhite(uint8_t _group_id, uint8_t _point_id){
    return groups.find(_group_id)->second.getColorWhite(_point_id);
}



void BamBoo_Layer::setLEDFromPixels(ofPixels &pixels){
    
    unordered_map<uint8_t, BamBoo_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.setLEDFromPixels(pixels);
}

void BamBoo_Layer::setLEDWhiteFromPixels(ofPixels &pixels){
    
    unordered_map<uint8_t, BamBoo_Group>::iterator iter;
    for(iter = groups.begin(); iter != groups.end(); iter++)
        iter->second.setLEDWhiteFromPixels(pixels);
}

void BamBoo_Layer::setMotor2DPoint(glm::vec2 _position_2D){
    position_motor_2D = _position_2D;
}



void BamBoo_Layer::setRotationPoint(glm::vec3 _rotation_point){
    rotation_point = _rotation_point;
}

void BamBoo_Layer::setMOTOR(float _degree_motor, int _time){
    
    if(isUpBamBoo){
        if(_degree_motor < -MOTOR_VALUE_MIN)
            _degree_motor = -MOTOR_VALUE_MIN;
        else if(_degree_motor > MOTOR_VALUE_MAX)
            _degree_motor = MOTOR_VALUE_MAX;
        
        if( degree_motor >= 90)
            degree_start = ofMap(degree_motor, 90, MOTOR_VALUE_LIMIT_MAX , 90, MOTOR_VALUE_MAX);
        else if(degree_motor <= 0)
            degree_start = ofMap(degree_motor, 0 , MOTOR_VALUE_LIMIT_MIN , 0, MOTOR_VALUE_MIN);
        
    }else{
        if(_degree_motor > MOTOR_VALUE_MIN)
            _degree_motor = MOTOR_VALUE_MIN;
        else if(_degree_motor < -MOTOR_VALUE_MAX)
            _degree_motor = -MOTOR_VALUE_MAX;
        
        if( degree_motor <= -90)
            degree_start = ofMap(degree_motor, -90, -MOTOR_VALUE_LIMIT_MAX , -90, -MOTOR_VALUE_MAX);
        else if(degree_motor >= 0)
            degree_start = ofMap(degree_motor, 0 , -MOTOR_VALUE_LIMIT_MIN , 0, -MOTOR_VALUE_MIN);
    }
    
    degree_target = _degree_motor;
//    degree_start = degree_motor; //record current degree;
//    degree_start = ofMap(degree_motor)

    target_time = _time;
    start_time = ofGetElapsedTimeMillis();
    current_time = ofGetElapsedTimeMillis();
}

void BamBoo_Layer::setMotorBezierCurveSetting(float p1x, float p1y, float p2x, float p2y){
    bezier_p1x = p1x;
    bezier_p1y = p1y;
    bezier_p2x = p2x;
    bezier_p2y = p2y;
}

void BamBoo_Layer::setDegreeOrigin(float _degree_origin){
    degree_origin = _degree_origin;
}

void BamBoo_Layer::setBamBooUpOrDown(bool _isUpBammBoo){
    isUpBamBoo = _isUpBammBoo;
    
    if(isUpBamBoo){
        degree_motor = 90;
        degree_motorboss = 90;
        degree_target = 90;
    }else{
        degree_motor = -90;
        degree_motorboss = -90;
        degree_target = -90;
    }
}

// BamBooCenter Methods
BamBooCenter::BamBooCenter(){
    
}

BamBooCenter::~BamBooCenter(){
    
}

void BamBooCenter::setup(){

    loadMotor3DCsv();
    loadMotor2DCsv();
    loadLED3DCsv();
    loadLED2DCsv();
    
    initGroupsMaxIndex();
    
    NDI_pixels.allocate(470,650,OF_IMAGE_COLOR);
    NDI_pixels.setColor(ofColor(0,0,0));
}

void BamBooCenter::update(){
    
    unordered_map<uint8_t, LED_Layer>::iterator iter;
    for(iter = led_layers.begin(); iter != led_layers.end(); iter++)
        iter->second.update();
    
    unordered_map<uint8_t, LED_Wash_Layer>::iterator iter_wash;
    for(iter_wash = led_layers_wash.begin(); iter_wash != led_layers_wash.end(); iter_wash++)
        iter_wash->second.update();
    
    unordered_map<uint8_t, BamBoo_Layer>::iterator iter_bamboo;
    for(iter_bamboo = bamboos.begin(); iter_bamboo != bamboos.end(); iter_bamboo++)
        iter_bamboo->second.update();
    
    //updateLED2DPiexls();
    updateLED2UDPFormat();
}

void BamBooCenter::draw(){
    
    unordered_map<uint8_t, LED_Layer>::iterator iter;
    for(iter = led_layers.begin(); iter != led_layers.end(); iter++)
        iter->second.draw();
    
    unordered_map<uint8_t, LED_Wash_Layer>::iterator iter_wash;
    for(iter_wash = led_layers_wash.begin(); iter_wash != led_layers_wash.end(); iter_wash++)
        iter_wash->second.draw();
    
    unordered_map<uint8_t, BamBoo_Layer>::iterator iter_bamboo;
    for(iter_bamboo = bamboos.begin(); iter_bamboo != bamboos.end(); iter_bamboo++){
        iter_bamboo->second.drawLED();
        iter_bamboo->second.drawBamBoo(bamBooDrewMode);
    }
    
}

void BamBooCenter::setBamBooDrew(){
//    isBamBooDrew = !isBamBooDrew;
    
    bamBooDrewMode++;
    bamBooDrewMode = bamBooDrewMode > 3 ? 0 : bamBooDrewMode;
}

void BamBooCenter::setLED(uint8_t _layer_id, uint8_t _group_id, uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time){

    if(getGroupsMaxIndex(_layer_id) < _group_id){
        cout<< "groups out of range" <<endl;
        return;
    }
    
    if(getPointsMaxIndex(_layer_id, _group_id) < _point_id){
        cout<< "points out of range" <<endl;
        return;
    }
    
    if(_layer_id == CENTER04_WASH || _layer_id == CENTER05_WASH || _layer_id==CENTER06_WASH || _layer_id == CENTER08_WASH)
        led_layers_wash.find(_layer_id)->second.setLED(_group_id, _point_id, _color_mode, _start_color, _target_color, _time);
    else if(_layer_id == BBTUBE120_14){
        uint8_t bamboo_layer_id;
        
        if(_group_id % 2 == 0)
            bamboo_layer_id = _group_id/2;
        else
            bamboo_layer_id = _group_id/2 +1;
        
        bamboos.find(bamboo_layer_id)->second.setLED(_group_id, _point_id, _color_mode, _start_color, _target_color, _time);
    }else if(_layer_id == BBTUBE120_09){
        uint8_t bamboo_layer_id;
        
        if(_group_id % 2 == 0)
            bamboo_layer_id = _group_id/2 +54;
        else
            bamboo_layer_id = _group_id/2 +1 +54;
        
        bamboos.find(bamboo_layer_id)->second.setLED(_group_id, _point_id, _color_mode, _start_color, _target_color, _time);
    }else if(_layer_id == BBDOT_13){
        
        bamboos.find(_group_id)->second.setLED(_group_id+54+1, _point_id, _color_mode, _start_color, _target_color, _time);
        
    }else if(_layer_id == BBDOT_10){
        
        bamboos.find(_group_id+54)->second.setLED(_group_id+54+1, _point_id, _color_mode, _start_color, _target_color, _time);
    }else
        led_layers.find(_layer_id)->second.setLED(_group_id, _point_id, _color_mode, _start_color, _target_color, _time);
}

void BamBooCenter::setLEDFlash(uint8_t _layer_id, uint8_t _group_id, uint8_t _point_id, uint8_t _flash_speed){
    
    if(getGroupsMaxIndex(_layer_id) < _group_id){
        cout<< "groups out of range" <<endl;
        return;
    }
    
    if(getPointsMaxIndex(_layer_id, _group_id) < _point_id){
        cout<< "points out of range" <<endl;
        return;
    }
    
    if(_layer_id == CENTER04_WASH || _layer_id == CENTER05_WASH || _layer_id==CENTER06_WASH || _layer_id == CENTER08_WASH){
        
        led_layers_wash.find(_layer_id)->second.setLEDFlash(_group_id, _point_id, _flash_speed);
    }
    
}



void BamBooCenter::loadLED3DCsv(){
    
    
    ofxCsv csv;
    if (csv.load("BBLight3D_20210503.CSV"))
    {
        int csv_rows = csv.getNumRows() - 1;
        int csv_cols = csv.getNumCols();
        
        for (int i = 1; i <= csv_rows; i++)
        {
            uint8_t layerID = layerName2Enum(csv[i][1]);
            
            vector<string> sub_position = ofSplitString(csv[i][4], ",");
            glm::vec3 position = glm::vec3(-stof(sub_position[0]) / size_scale, stof(sub_position[1]) / size_scale, stof(sub_position[2]) / size_scale - 532.5);
            
            vector<string> sub_attributeText = ofSplitString(csv[i][5],"_");

            uint8_t attribute_1 = stoi(sub_attributeText[0]);
            uint8_t attribute_2 = stoi(sub_attributeText[1]);
            
            if(layerID == CENTER04_WASH || layerID == CENTER05_WASH || layerID==CENTER06_WASH || layerID == CENTER08_WASH){
                
                addLayerWash(layerID);
                
                auto layer = led_layers_wash.find(layerID);
                layer->second.addGroup(attribute_1);
                layer->second.addPoint(attribute_1,attribute_2,position);
                
            }else if(layerID == BBTUBE120_14){
                
                addLayerBambooLED(layerID);
                
                uint8_t bamboo_layer_id;
                glm::vec3 position_o;
                
                if(attribute_1 % 2 == 0){
                    bamboo_layer_id = attribute_1/2;
                    position_o = glm::vec3(-9.00107, 17.4418, 52 + 5.3392 *attribute_2);
//                    position_o = glm::vec3(10.0651, 16.2789, 54.8 + 5.2175 *attribute_2);
                }else{
                    bamboo_layer_id = attribute_1/2+1;
//                    position_o = glm::vec3(-9.00107, 17.4418, 52 + 5.3392 *attribute_2);
                    position_o = glm::vec3(10.0651, 16.2789, 54.8 + 5.2175 *attribute_2);
                }

                auto layer = bamboos.find(bamboo_layer_id);

                layer->second.addGroup(attribute_1);
                layer->second.addPoint(attribute_1,attribute_2,position_o);
                
            }else if(layerID == BBTUBE120_09){
                
                addLayerBambooLED(layerID);
                
                uint8_t bamboo_layer_id;
                glm::vec3 position_o;
                
                if(attribute_1 % 2 == 0){
                    bamboo_layer_id = attribute_1/2 +54;
//                    position_o = glm::vec3(10.9651, 16.2789, -174.8 + 5.2175 *attribute_2);
                    position_o = glm::vec3(-9.00107, 17.4418, -174.8 + 5.2175 *attribute_2);
                    
                }
                else{
                    bamboo_layer_id = attribute_1/2 +1 +54;
//                    position_o = glm::vec3(-9.00107, 17.4418, -174.8 + 5.2175 *attribute_2);
                    position_o = glm::vec3(10.9651, 16.2789, -174.8 + 5.2175 *attribute_2);
                }
                auto layer = bamboos.find(bamboo_layer_id);

                layer->second.addGroup(attribute_1);
                layer->second.addPoint(attribute_1,attribute_2,position_o);
                
            }else if(layerID == BBDOT_13){
                
                addLayerBambooLED(layerID);
                
                glm::vec3 position_o;
                position_o = glm::vec3(1.37101,23.5393,474.021);
                
                auto layer = bamboos.find(attribute_1);
                layer->second.addGroup(attribute_1+54+1);
                layer->second.addPoint(attribute_1+54+1,attribute_2, position_o);
                
            }else if(layerID == BBDOT_10){
                
                addLayerBambooLED(layerID);
                
                glm::vec3 position_o;
                position_o = glm::vec3(1.37101,23.5393,-474.021);
                
                auto layer = bamboos.find(attribute_1+54);
                
                layer->second.addGroup(attribute_1+54+1);
                layer->second.addPoint(attribute_1+54+1,attribute_2, position_o);
                
            }else{

                addLayer(layerID);
                
                auto layer = led_layers.find(layerID);
                layer->second.addGroup(attribute_1);
                layer->second.addPoint(attribute_1,attribute_2,position);
            
            }
            
            if(cue_max_index.find(layerID)->second[0] < attribute_1)
                cue_max_index.find(layerID)->second[0] = attribute_1;
            
            if(cue_max_index.find(layerID)->second[1] < attribute_2)
                cue_max_index.find(layerID)->second[1] = attribute_2;

        }
    }
}

void BamBooCenter::loadLED2DCsv(){
    ofxCsv csv;
    if (csv.load("BBLight_20210504.CSV")){
        int csv_rows = csv.getNumRows() - 1;
        int csv_cols = csv.getNumCols();
        
        for (int i = 1; i <= csv_rows; i++)
        {
            uint8_t layerID = layerName2Enum(csv[i][1]);
            vector<string> sub_position = ofSplitString(csv[i][4], ",");
            glm::vec2 position_2D = glm::vec2(stof(sub_position[0]), -stof(sub_position[1]));
            
            vector<string> sub_attributeText = ofSplitString(csv[i][5],"_");
            uint8_t attribute_1 = stoi(sub_attributeText[0]);
            uint8_t attribute_2 = stoi(sub_attributeText[1]);
            
//            cout<< +layerID<< ", " << +attribute_1<< ", "<<+attribute_2 << ", " << position_2D.x << ", " << position_2D.y<< ", "<< csv[i][5]<<endl;

            if(layerID == NONE){
                cout<< "cant found 2d layer "<< csv[i][1] <<endl;
            }else if(layerID == CENTER04_WASH || layerID == CENTER05_WASH || layerID==CENTER06_WASH || layerID == CENTER08_WASH){
                auto layer = led_layers_wash.find(layerID);
                layer->second.add2DPoint(attribute_1, attribute_2, position_2D);
                
            }else if(layerID == CENTER04_WASH_FLASH){
                auto layer = led_layers_wash.find(CENTER04_WASH);
                layer->second.add2DPointFlash(attribute_1, attribute_2, position_2D);
                
            }else if(layerID == CENTER05_WASH_FLASH){
                auto layer = led_layers_wash.find(CENTER05_WASH);
                layer->second.add2DPointFlash(attribute_1, attribute_2, position_2D);
                
            }else if(layerID == CENTER06_WASH_FLASH){
                auto layer = led_layers_wash.find(CENTER06_WASH);
                layer->second.add2DPointFlash(attribute_1, attribute_2, position_2D);
                
            }else if(layerID == CENTER08_WASH_FLASH){
                auto layer = led_layers_wash.find(CENTER08_WASH);
                
                for(uint8_t i = 1 ; i <= 6 ; i++){
                    layer->second.add2DPointFlash(attribute_1, i+(attribute_2-1)*6, position_2D);
                }
                
            }else if(layerID == BBTUBE120_14){
                uint8_t bamboo_layer_id;
                if(attribute_1 % 2 == 0)
                    bamboo_layer_id = attribute_1/2;
                else
                    bamboo_layer_id = attribute_1/2 +1;
                
                auto layer = bamboos.find(bamboo_layer_id);
                layer->second.add2DPoint(attribute_1, attribute_2, position_2D);
                
            }else if(layerID == BBTUBE120_09){
                uint8_t bamboo_layer_id;
                if(attribute_1 % 2 == 0)
                    bamboo_layer_id = attribute_1/2 +54;
                else
                    bamboo_layer_id = attribute_1/2 +1 +54;
                
                auto layer = bamboos.find(bamboo_layer_id);
                layer->second.add2DPoint(attribute_1,attribute_2, position_2D);
            }else if(layerID == BBDOT_13){

                auto layer = bamboos.find(attribute_1);
                layer->second.add2DPoint(attribute_1+54+1,attribute_2, position_2D);
                
            }else if(layerID == BBDOT_10){

                auto layer = bamboos.find(attribute_1+54);
                layer->second.add2DPoint(attribute_1+54+1,attribute_2, position_2D);
                
            }else{
                auto layer = led_layers.find(layerID);
                layer->second.add2DPoint(attribute_1, attribute_2, position_2D);
            }
        }
    }
}

void BamBooCenter::loadMotor3DCsv(){
    ofxCsv csv;
    if (csv.load("BBMotor3D_20210114.CSV")){
        
        int csv_rows = csv.getNumRows() - 1;
        int csv_cols = csv.getNumCols();
        
        for (int i = 1; i <= csv_rows; i++){
            string bambooLayerID = csv[i][1];
            vector<string> sub_position = ofSplitString(csv[i][4], ",");
            glm::vec3 position = glm::vec3(-stof(sub_position[0])/ size_scale, stof(sub_position[1])/ size_scale,stof(sub_position[2])/ size_scale - 532.5);
            
            vector<string> sub_attributeText = ofSplitString(csv[i][6],"_");
            uint8_t attribute_1 = stoi(sub_attributeText[0]);
            uint8_t attribute_2 = stoi(sub_attributeText[1]);
            
            
            if(bambooLayerID.compare("MotorUp") == 0){
                
                addLayerBamboo(attribute_1);
                
                auto layer = bamboos.find(attribute_1);
                layer->second.setBamBooUpOrDown(true);
                layer->second.setDegreeOrigin(float(6.66*attribute_1));
                if(attribute_2 == 1)
                    layer->second.setRotationPoint(position);

            }else if( bambooLayerID.length() == 9){
                
                addLayerBamboo(attribute_1 + 54);
                
                auto layer = bamboos.find(attribute_1 + 54);
                layer->second.setBamBooUpOrDown(false);
                layer->second.setDegreeOrigin(float(6.66*attribute_1));
                if(attribute_2 == 1)
                    layer->second.setRotationPoint(position);

            }
        }
    }
    
    // sort
    vector<pair<uint8_t, BamBoo_Layer>> sorted_bamboos(bamboos.begin(),bamboos.end());
    sort(sorted_bamboos.begin(), sorted_bamboos.end(), compare_map_order);
    bamboos.clear();
    for(int i = 0 ; i < sorted_bamboos.size() ; i++){
        bamboos.insert(pair<uint8_t, BamBoo_Layer>(sorted_bamboos[i].first, sorted_bamboos[i].second));
    }
    

}

void BamBooCenter::loadMotor2DCsv(){
    ofxCsv csv;
    if (csv.load("BBMotor_20210503.CSV")){

        int csv_rows = csv.getNumRows() - 1;
        int csv_cols = csv.getNumCols();

        for (int i = 1; i <= csv_rows; i++){

            string bambooLayerID = csv[i][1];

            vector<string> sub_position = ofSplitString(csv[i][4], ",");
            glm::vec2 position_2D = glm::vec2(stof(sub_position[0]), -stof(sub_position[1]));

            uint8_t attribute_1 = stoi(csv[i][5]);
            
            if(bambooLayerID.compare("MotorUp") == 0){
                
                auto layer = bamboos.find(attribute_1);
                layer->second.setMotor2DPoint(position_2D);
                
            }else if( bambooLayerID.length() == 9){
                
                auto layer = bamboos.find(attribute_1+54);
                layer->second.setMotor2DPoint(position_2D);
            }
        }

    }
}

void BamBooCenter::setMOTOR(bool _isUpBamBoo, uint8_t _id, float _angle, int _time){
    
    if(_isUpBamBoo){
        auto layer = bamboos.find(_id);
        layer->second.setMOTOR(_angle, _time);
    }else{
        auto layer = bamboos.find(_id+54);
        layer->second.setMOTOR(_angle, _time);
    }
}

void BamBooCenter::setMotorBezierCurveSetting(vector<float> p){
    
    
    unordered_map<uint8_t, BamBoo_Layer>::iterator iter_bamboo;
    for(iter_bamboo = bamboos.begin(); iter_bamboo != bamboos.end(); iter_bamboo++){
        iter_bamboo->second.setMotorBezierCurveSetting(p[0], p[1], p[2], p[3]);
    }
    
}

void BamBooCenter::addLayerBambooLED(uint8_t _id){
    
    auto layer = cue_max_index.find(_id);
    
    if(layer == cue_max_index.end()){
     
        vector<uint8_t> index_vector;
        index_vector.push_back(0);
        index_vector.push_back(0);
        cue_max_index.insert(pair<uint8_t, vector<uint8_t>>(_id, index_vector));

    }
}

void BamBooCenter::addLayerBamboo(uint8_t _id){
    auto layer = bamboos.find(_id);
    
    if(layer == bamboos.end()){
        
        BamBoo_Layer new_led_layer;
        bamboos.insert(pair<uint8_t, BamBoo_Layer>(_id, new_led_layer));
    }
}

void BamBooCenter::addLayerWash(uint8_t _id){
    auto layer = led_layers_wash.find(_id);
    
    if(layer == led_layers_wash.end()){
        
        LED_Wash_Layer new_led_layer;
        led_layers_wash.insert(pair<uint8_t, LED_Wash_Layer>(_id, new_led_layer));
        
        vector<uint8_t> index_vector;
        index_vector.push_back(0);
        index_vector.push_back(0);
        cue_max_index.insert(pair<uint8_t, vector<uint8_t>>(_id, index_vector));
        
    }
}

void BamBooCenter::addLayer(uint8_t _id){
    
    auto layer = led_layers.find(_id);
    
    if(layer == led_layers.end()){
        
        LED_Layer new_led_layer;
        led_layers.insert(pair<uint8_t, LED_Layer>(_id, new_led_layer));
        
        
        vector<uint8_t> index_vector;
        index_vector.push_back(0);
        index_vector.push_back(0);
        cue_max_index.insert(pair<uint8_t, vector<uint8_t>>(_id, index_vector));
        
    }
    
}

void BamBooCenter::initGroupsMaxIndex(){
    vector<uint8_t> groups_index{27,9,54,108,54,108,18,27,27,6,3,18,27,27,27,18,27,108,27,27,27,54,27,27,18,6,6,1,6,9};
    
    vector<uint8_t> points_index{12,12,1,24,1,24,8,1,1,14,57,1,12,12,12,24,12,1,12,1,1,1,12,12,15,1,1,1,1,12};
    
    
    for(uint8_t i = 0 ; i < 30 ; i++){
        LayerGroupMaxIndex newLayerGroupMaxIndex;
        groups_max_index.insert(pair<uint8_t, LayerGroupMaxIndex>(i, newLayerGroupMaxIndex));
        groups_max_index.find(i)->second.setGroupsIndex(groups_index[i]);
        groups_max_index.find(i)->second.setPointsIndex(groups_index[i], points_index[i]);
    }
    
}

uint8_t BamBooCenter::getGroupsMaxIndex(uint8_t _layerID){
    return groups_max_index.find(_layerID)->second.getMaxGroupsIndex();
}

uint8_t BamBooCenter::getPointsMaxIndex(uint8_t _layerID , uint8_t _groupID){
    return groups_max_index.find(_layerID)->second.getMaxPointsIndex(_groupID);
}

unordered_map<uint8_t, vector<uint8_t>> BamBooCenter::getLayerMaxIndex(){
    return cue_max_index;
}

void BamBooCenter::setLEDFromPixels( ofPixels &pixels){
    
    unordered_map<uint8_t, LED_Layer>::iterator iter;
    for(iter = led_layers.begin(); iter != led_layers.end(); iter++)
        iter->second.setLEDFromPixels(pixels);
    
    unordered_map<uint8_t, LED_Wash_Layer>::iterator iter_wash;
    for(iter_wash = led_layers_wash.begin(); iter_wash != led_layers_wash.end(); iter_wash++)
        iter_wash->second.setLEDFromPixels(pixels);
    
    unordered_map<uint8_t, BamBoo_Layer>::iterator iter_bamboo;
    for(iter_bamboo = bamboos.begin(); iter_bamboo != bamboos.end(); iter_bamboo++)
        iter_bamboo->second.setLEDFromPixels(pixels);
    
}

void BamBooCenter::setLEDWhiteFromPixels( ofPixels &pixels){
    
    unordered_map<uint8_t, LED_Layer>::iterator iter;
    for(iter = led_layers.begin(); iter != led_layers.end(); iter++)
        iter->second.setLEDWhiteFromPixels(pixels);

    unordered_map<uint8_t, LED_Wash_Layer>::iterator iter_wash;
    for(iter_wash = led_layers_wash.begin(); iter_wash != led_layers_wash.end(); iter_wash++)
        iter_wash->second.setLEDWhiteFromPixels(pixels);
    
    unordered_map<uint8_t, BamBoo_Layer>::iterator iter_bamboo;
    for(iter_bamboo = bamboos.begin(); iter_bamboo != bamboos.end(); iter_bamboo++)
        iter_bamboo->second.setLEDWhiteFromPixels(pixels);
    
}

void BamBooCenter::setMotorFromPixels(ofPixels &pixels){
    
    unordered_map<uint8_t, BamBoo_Layer>::iterator iter_bamboo;
    for(iter_bamboo = bamboos.begin(); iter_bamboo != bamboos.end(); iter_bamboo++){
//        iter_bamboo->second.setMotorFromPixels(pixels);
        
        ofColor c = pixels.getColor(iter_bamboo->second.position_motor_2D.x,iter_bamboo->second.position_motor_2D.y);
        
        float brightness = c.r;
        
        float angle;
        if(iter_bamboo->second.isUpBamBoo)
            angle = ofMap(brightness,0,224,-MOTOR_VALUE_MIN,MOTOR_VALUE_MAX);
        else
            angle = ofMap(brightness,0,224,MOTOR_VALUE_MIN,-MOTOR_VALUE_MAX);
        iter_bamboo->second.setMOTOR(angle, 0);
    }
}

vector<float> BamBooCenter::getMotorsDegrees(){
    vector<float> degrees;
    unordered_map<uint8_t, BamBoo_Layer>::iterator iter_bamboo;
    for(iter_bamboo = bamboos.begin(); iter_bamboo != bamboos.end();iter_bamboo++){
        degrees.push_back(iter_bamboo->second.degree_motor);
    }

    return degrees;
}

void BamBooCenter::setMotorBossDegree(Json::Value motor_boss_json){
    

//    cout<<motor_boss_json.toStyledString()<<endl;
    for(int i = 1 ; i <= 108 ; i++){

        if(Json_ReadDouble(motor_boss_json["HIMC_M_DEG"][i-1]) != -1000.0){
            
            float _degree_motor = Json_ReadDouble(motor_boss_json["HIMC_M_DEG"][i-1]);
            
//            cout << i << ":"<<_degree_motor<<endl;
            
            if( i <= 54){
//                if(_degree_motor < -MOTOR_VALUE_MIN)
//                    _degree_motor = -MOTOR_VALUE_MIN;
//                else if(_degree_motor > MOTOR_VALUE_MAX)
//                    _degree_motor = MOTOR_VALUE_MAX;
                
                if(_degree_motor > MOTOR_VALUE_MIN)
                    _degree_motor = MOTOR_VALUE_MIN;
                else if(_degree_motor < -MOTOR_VALUE_MAX)
                    _degree_motor = -MOTOR_VALUE_MAX;
                
//                cout<< "up"<<(i+54) <<" : "<<_degree_motor<<endl;
                
                bamboos.find((uint8_t)(i+54))->second.degree_motorboss = _degree_motor;
            }else{
//                if(_degree_motor > MOTOR_VALUE_MIN)
//                    _degree_motor = MOTOR_VALUE_MIN;
//                else if(_degree_motor < -MOTOR_VALUE_MAX)
//                    _degree_motor = -MOTOR_VALUE_MAX;
                
                if(_degree_motor < -MOTOR_VALUE_MIN)
                    _degree_motor = -MOTOR_VALUE_MIN;
                else if(_degree_motor > MOTOR_VALUE_MAX)
                    _degree_motor = MOTOR_VALUE_MAX;
                
//                cout<< "down"<<(i-54)<<" : "<<_degree_motor<<endl;
                
                bamboos.find((uint8_t)(i-54))->second.degree_motorboss = _degree_motor;
            }
            
            
//            bamboos.find((uint8_t)i)->second.degree_motorboss = _degree_motor;
        }
    }
}

void BamBooCenter::updateLED2DPiexls(){
    NDI_pixels.setColor(ofColor(0,0,0));
    
    unordered_map<uint8_t, LayerGroupMaxIndex>::iterator iter;
    for(iter = groups_max_index.begin(); iter != groups_max_index.end(); iter++){
        auto led_layer = led_layers.find(iter->first);
        auto led_layer_wash = led_layers_wash.find(iter->first);
        
        
        if(led_layer != led_layers.end()){
            for(uint8_t i = 1 ; i <= iter->second.groups_max_index ; i++){
                for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){
                    
                    glm::vec2 pos = led_layer->second.get2DPoint(i, j);
                    ofColor c = led_layer->second.getColor(i,j);
                    
                    if(pos.x != 0 && pos.y != 0){
                        NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                        NDI_pixels.setColor(pos.x-1, pos.y, c);
                        NDI_pixels.setColor(pos.x-1, pos.y+1, c);
                    
                        NDI_pixels.setColor(pos.x, pos.y-1, c);
                        NDI_pixels.setColor(pos.x, pos.y, c);
                        NDI_pixels.setColor(pos.x, pos.y+1, c);
                    
                        NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                        NDI_pixels.setColor(pos.x+1, pos.y, c);
                        NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                    }
                }
            }
        }else if(led_layer_wash != led_layers_wash.end()){
            
            for(uint8_t i = 1 ; i <= iter->second.groups_max_index ; i++){
                for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){
                    
                    glm::vec2 pos = led_layer_wash->second.get2DPoint(i, j);
                    ofColor c = led_layer_wash->second.getColor(i,j);
                    if(pos.x != 0 && pos.y != 0){
                        NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                        NDI_pixels.setColor(pos.x-1, pos.y, c);
                        NDI_pixels.setColor(pos.x-1, pos.y+1, c);
                        
                        NDI_pixels.setColor(pos.x, pos.y-1, c);
                        NDI_pixels.setColor(pos.x, pos.y, c);
                        NDI_pixels.setColor(pos.x, pos.y+1, c);
                        
                        NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                        NDI_pixels.setColor(pos.x+1, pos.y, c);
                        NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                    }
                    
                    pos = led_layer_wash->second.get2DPointFlash(i, j);
                    c = led_layer_wash->second.getColor(i,j);
                    if(pos.x != 0 && pos.y != 0){
                        NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                        NDI_pixels.setColor(pos.x-1, pos.y, c);
                        NDI_pixels.setColor(pos.x-1, pos.y+1, c);
                        
                        NDI_pixels.setColor(pos.x, pos.y-1, c);
                        NDI_pixels.setColor(pos.x, pos.y, c);
                        NDI_pixels.setColor(pos.x, pos.y+1, c);
                        
                        NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                        NDI_pixels.setColor(pos.x+1, pos.y, c);
                        NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                    }
                }
            }
        }else{
            if(iter->first == BBDOT_13){
                for(uint8_t i = 1 ; i <= iter->second.groups_max_index ; i++){
                    for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){

                        glm::vec2 pos = bamboos.find(i)->second.get2DPoint(i+54+1, j);
                        ofColor c = bamboos.find(i)->second.getColor(i+54+1,j);
                        if(pos.x != 0 && pos.y != 0){
                            NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x-1, pos.y, c);
                            NDI_pixels.setColor(pos.x-1, pos.y+1, c);

                            NDI_pixels.setColor(pos.x, pos.y-1, c);
                            NDI_pixels.setColor(pos.x, pos.y, c);
                            NDI_pixels.setColor(pos.x, pos.y+1, c);

                            NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x+1, pos.y, c);
                            NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                        }
                    }
                }

            }else if(iter->first == BBDOT_10){
                for(uint8_t i = 1 ; i <= iter->second.groups_max_index ; i++){
                    for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){

                        glm::vec2 pos = bamboos.find(i+54)->second.get2DPoint(i+54+1, j);
                        ofColor c = bamboos.find(i+54)->second.getColor(i+54+1,j);
                        if(pos.x != 0 && pos.y != 0){
                            NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x-1, pos.y, c);
                            NDI_pixels.setColor(pos.x-1, pos.y+1, c);

                            NDI_pixels.setColor(pos.x, pos.y-1, c);
                            NDI_pixels.setColor(pos.x, pos.y, c);
                            NDI_pixels.setColor(pos.x, pos.y+1, c);

                            NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x+1, pos.y, c);
                            NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                        }
                    }
                }
            }else if(iter->first == BBTUBE120_14){
                for(uint8_t i = 1 ; i <= 54 ; i++){
                    for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){

                        glm::vec2 pos = bamboos.find(i)->second.get2DPoint(i*2-1, j);
                        ofColor c = bamboos.find(i)->second.getColor(i*2-1,j);
                        
                        
                        if(pos.x != 0 && pos.y != 0){
                            NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x-1, pos.y, c);
                            NDI_pixels.setColor(pos.x-1, pos.y+1, c);

                            NDI_pixels.setColor(pos.x, pos.y-1, c);
                            NDI_pixels.setColor(pos.x, pos.y, c);
                            NDI_pixels.setColor(pos.x, pos.y+1, c);
                            
                            NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x+1, pos.y, c);
                            NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                        }

                        pos = bamboos.find(i)->second.get2DPoint(i*2, j);
                        c = bamboos.find(i)->second.getColor(i*2,j);

                        if(pos.x != 0 && pos.y != 0){
                            NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x-1, pos.y, c);
                            NDI_pixels.setColor(pos.x-1, pos.y+1, c);

                            NDI_pixels.setColor(pos.x, pos.y-1, c);
                            NDI_pixels.setColor(pos.x, pos.y, c);
                            NDI_pixels.setColor(pos.x, pos.y+1, c);

                            NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x+1, pos.y, c);
                            NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                        }
                    }
                }
                
            }else if(iter->first == BBTUBE120_09){
                for(uint8_t i = 1 ; i <= 54 ; i++){
                    for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){

                        glm::vec2 pos = bamboos.find(i+54)->second.get2DPoint(i*2-1, j);
                        ofColor c = bamboos.find(i+54)->second.getColor(i*2-1,j);
                        if(pos.x != 0 && pos.y != 0){
                            NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x-1, pos.y, c);
                            NDI_pixels.setColor(pos.x-1, pos.y+1, c);

                            NDI_pixels.setColor(pos.x, pos.y-1, c);
                            NDI_pixels.setColor(pos.x, pos.y, c);
                            NDI_pixels.setColor(pos.x, pos.y+1, c);

                            NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x+1, pos.y, c);
                            NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                        }

                        pos = bamboos.find(i+54)->second.get2DPoint(i*2, j);
                        c = bamboos.find(i+54)->second.getColor(i*2,j);

                        if(pos.x != 0 && pos.y != 0){
                            NDI_pixels.setColor(pos.x-1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x-1, pos.y, c);
                            NDI_pixels.setColor(pos.x-1, pos.y+1, c);

                            NDI_pixels.setColor(pos.x, pos.y-1, c);
                            NDI_pixels.setColor(pos.x, pos.y, c);
                            NDI_pixels.setColor(pos.x, pos.y+1, c);

                            NDI_pixels.setColor(pos.x+1, pos.y-1, c);
                            NDI_pixels.setColor(pos.x+1, pos.y, c);
                            NDI_pixels.setColor(pos.x+1, pos.y+1, c);
                        }
                    }
                }
            }
        }
    }
}

void BamBooCenter::updateLED2UDPFormat(){

    int count = 0;
    unordered_map<uint8_t, LayerGroupMaxIndex>::iterator iter;
    for(iter = groups_max_index.begin(); iter != groups_max_index.end(); iter++){
        auto led_layer = led_layers.find(iter->first);
        auto led_layer_wash = led_layers_wash.find(iter->first);
        
        if(led_layer != led_layers.end()){
            for(uint8_t i = 1 ; i <= iter->second.groups_max_index ; i++){
                for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){
                    glm::vec2 pos = led_layer->second.get2DPoint(i, j);
                    int c = led_layer->second.getColor(i,j).getHex();
                    uint8_t cw = led_layer->second.getColorWhite(i,j);
                    if(pos.x != 0 && pos.y != 0){
                        led_udp_format[count++] = c >> 16 & 0xff;
                        led_udp_format[count++] = c >> 8 & 0xff;
                        led_udp_format[count++] = c & 0xff;
                        led_udp_format[count++] = cw;
                    }
                }
            }
        }else if(led_layer_wash != led_layers_wash.end()){
            
            for(uint8_t i = 1 ; i <= iter->second.groups_max_index ; i++){
                for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){
                    
                    glm::vec2 pos = led_layer_wash->second.get2DPoint(i, j);
                    int c = led_layer_wash->second.getColor(i,j).getHex();
                    uint8_t cw = led_layer_wash->second.getColorWhite(i,j);
                    if(pos.x != 0 && pos.y != 0){
                        led_udp_format[count++] = c >> 16 & 0xff;
                        led_udp_format[count++] = c >> 8 & 0xff;
                        led_udp_format[count++] = c & 0xff;
                        led_udp_format[count++] = cw;
                    }
                    
                    pos = led_layer_wash->second.get2DPointFlash(i, j);
//                    c = led_layer_wash->second.getColor(i,j).getHex();
                    c = led_layer_wash->second.getColorFlash(i, j).getHex();
//                    cw = led_layer_wash->second.getColorWhite(i,j);
                    
                    if(pos.x != 0 && pos.y != 0){
                        led_udp_format[count++] = c >> 16 & 0xff;
                        led_udp_format[count++] = c >> 8 & 0xff;
                        led_udp_format[count++] = c & 0xff;
//                        led_udp_format[count++] = cw;
                        count++;
                    }
                }
            }
        }else{
            if(iter->first == BBDOT_13){
                for(uint8_t i = 1 ; i <= iter->second.groups_max_index ; i++){
                    for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){
                        
                        glm::vec2 pos = bamboos.find(i)->second.get2DPoint(i+54+1, j);
                        int c = bamboos.find(i)->second.getColor(i+54+1,j).getHex();
                        uint8_t cw = bamboos.find(i)->second.getColorWhite(i+54+1,j);
                        if(pos.x != 0 && pos.y != 0){
                            led_udp_format[count++] = c >> 16 & 0xff;
                            led_udp_format[count++] = c >> 8 & 0xff;
                            led_udp_format[count++] = c & 0xff;
                            led_udp_format[count++] = cw;
                        }
                    }
                }
                
            }else if(iter->first == BBDOT_10){
                for(uint8_t i = 1 ; i <= iter->second.groups_max_index ; i++){
                    for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){
//                        cout<< +iter->first << " , "<< +i  << " , " << +j << endl;
                        glm::vec2 pos = bamboos.find(i+54)->second.get2DPoint(i+54+1, j);
                        int c = bamboos.find(i+54)->second.getColor(i+54+1,j).getHex();
                        uint8_t cw = bamboos.find(i+54)->second.getColorWhite(i+54+1,j);
                        if(pos.x != 0 && pos.y != 0){
                            led_udp_format[count++] = c >> 16 & 0xff;
                            led_udp_format[count++] = c >> 8 & 0xff;
                            led_udp_format[count++] = c & 0xff;
                            led_udp_format[count++] = cw;
                        }
                    }
                }
            }else if(iter->first == BBTUBE120_14){
                for(uint8_t i = 1 ; i <= 54 ; i++){
                    for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){
                        
                        glm::vec2 pos = bamboos.find(i)->second.get2DPoint(i*2-1, j);
                        int c = bamboos.find(i)->second.getColor(i*2-1,j).getHex();
                        uint8_t cw = bamboos.find(i)->second.getColorWhite(i*2-1,j);
                        if(pos.x != 0 && pos.y != 0){
                            led_udp_format[count++] = c >> 16 & 0xff;
                            led_udp_format[count++] = c >> 8 & 0xff;
                            led_udp_format[count++] = c & 0xff;
                            led_udp_format[count++] = cw;
                        }
                        
                        pos = bamboos.find(i)->second.get2DPoint(i*2, j);
                        c = bamboos.find(i)->second.getColor(i*2,j).getHex();
                        cw = bamboos.find(i)->second.getColorWhite(i*2,j);
                        
                        if(pos.x != 0 && pos.y != 0){
                            led_udp_format[count++] = c >> 16 & 0xff;
                            led_udp_format[count++] = c >> 8 & 0xff;
                            led_udp_format[count++] = c & 0xff;
                            led_udp_format[count++] = cw;
                        }
                    }
                }
                
            }else if(iter->first == BBTUBE120_09){
                for(uint8_t i = 1 ; i <= 54 ; i++){
                    for(uint8_t j = 1 ; j <= iter->second.points_max_index ; j++){
                        
                        glm::vec2 pos = bamboos.find(i+54)->second.get2DPoint(i*2-1, j);
                        int c = bamboos.find(i+54)->second.getColor(i*2-1,j).getHex();
                        uint8_t cw = bamboos.find(i+54)->second.getColorWhite(i*2-1,j);
                        if(pos.x != 0 && pos.y != 0){
                            led_udp_format[count++] = c >> 16 & 0xff;
                            led_udp_format[count++] = c >> 8 & 0xff;
                            led_udp_format[count++] = c & 0xff;
                            led_udp_format[count++] = cw;
                        }
                        
                        pos = bamboos.find(i+54)->second.get2DPoint(i*2, j);
                        c = bamboos.find(i+54)->second.getColor(i*2,j).getHex();
                        cw = bamboos.find(i+54)->second.getColorWhite(i*2,j);
                        
                        if(pos.x != 0 && pos.y != 0){
                            led_udp_format[count++] = c >> 16 & 0xff;
                            led_udp_format[count++] = c >> 8 & 0xff;
                            led_udp_format[count++] = c & 0xff;
                            led_udp_format[count++] = cw;
                        }
                    }
                }
            }
        }
    }
//    cout<<"totoal pixels : " << count<<endl;
}


int compare_map_order(pair<uint8_t, BamBoo_Layer> a, pair<uint8_t, BamBoo_Layer> b){
    return a.first > b.first;
}
