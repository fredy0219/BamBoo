//
//  BamBoo.h
//  BamBooGod
//
//  Created by Fish on 2020/12/29.
//
using namespace std;
#include "ofMain.h"
#include "ofxCsv.h"
#include "ofxJSON.h"
#include "Utils.h"

#ifndef BamBooCenter_h
#define BamBooCenter_h

class LED_Point{
    
    public:
    LED_Point(glm::vec3 _position);
    virtual void draw();
    virtual void update();
    
    void setLED(uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void setLEDRGB(ofColor color);
    void setLEDWhite(uint8_t white);
    
    void add2DPoint(glm::vec2 _position_2D);
    glm::vec2 get2DPoint();
    ofColor getColor();
    uint8_t getColorWhite();
    
    uint8_t color_mode;
    
    ofColor color;
    ofColor start_color;
    ofColor target_color;
    
    uint8_t white_color;
    uint8_t start_white_color;
    uint8_t target_white_color;
    
    glm::vec3 position;
    glm::vec2 position_2D;
    uint64_t target_time;
    uint64_t start_time;
    uint64_t current_time;
};

class LED_Group{
    public:
    LED_Group();
    void draw();
    void addPoint(uint8_t _id, glm::vec3 _position);
    void update();
    
    void setLED(uint8_t _color_mode, uint8_t _point_id, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void add2DPoint(uint8_t _point_id, glm::vec2 _position_2D);
    glm::vec2 get2DPoint(uint8_t _point_id);
    
    ofColor getColor(uint8_t _point_id);
    uint8_t getColorWhite(uint8_t _point_id);
    
    unordered_map<uint8_t, LED_Point> points;
    
    void setLEDFromPixels( ofPixels &pixels);
    void setLEDWhiteFromPixels(ofPixels &pixels);
};

class LED_Layer{
    
public:
    LED_Layer();
    virtual void draw();
    virtual void update();
    void addGroup(uint8_t _group_id);
    void addPoint(uint8_t _group_id, uint8_t _point_id, glm::vec3 _position);
    
    void setLED(uint8_t _group_id,uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void add2DPoint(uint8_t _group_id, uint8_t _point_id, glm::vec2 _position_2D);
    glm::vec2 get2DPoint(uint8_t _group_id, uint8_t _point_id);
    
    ofColor getColor(uint8_t _group_id, uint8_t _point_id);
    uint8_t getColorWhite(uint8_t _group_id, uint8_t _point_id);
    unordered_map<uint8_t, LED_Group> groups;
    
    void setLEDFromPixels( ofPixels &pixels);
    void setLEDWhiteFromPixels(ofPixels &pixels);
};

/////////////////////////////////
class LED_Wash_Point : public LED_Point{
    
public:
    LED_Wash_Point(glm::vec3 _position) : LED_Point(_position){
        isFlashEnable = false;
        isFlashTrigger = false;
        flashTime = 0;
        flashSpeed = 150;
        colorFlash = ofColor(0,0,0);
    }
    
    void update() override;
    
    void draw() override;
    void setFlash(bool _isFlashEnable);
    
    void add2DPointFlash(glm::vec2 _position_2D_flash);
    glm::vec2 get2DPointFlash();
    ofColor getColorFlash();
    
    void setFlashSpeed(uint8_t brightness);
    void setColorFlash(ofColor);
    ofColor colorFlash;
    bool isFlashEnable = false;
    bool isFlashTrigger = false;
    uint64_t flashTime = 0;
    uint64_t flashSpeed = 0;
    glm::vec2 position_2D_flash;

};

class LED_Wash_Group{
public:
    LED_Wash_Group();
    void draw();
    void addPoint(uint8_t _id, glm::vec3 _position);
    void update();
    
    void setLED(uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void setLEDFlash(uint8_t _point_id, uint8_t _flash_speed);
    
    void add2DPoint(uint8_t _point_id, glm::vec2 _position_2D);
    void add2DPointFlash(uint8_t _point_id, glm::vec2 _position_2D_flash);
    
    glm::vec2 get2DPoint(uint8_t _point_id);
    glm::vec2 get2DPointFlash(uint8_t _point_id);
    
    ofColor getColor(uint8_t _point_id);
    uint8_t getColorWhite(uint8_t _point_id);
    ofColor getColorFlash(uint8_t _point_id);
    
    unordered_map<uint8_t, LED_Wash_Point> points;
    
    void setLEDFromPixels( ofPixels &pixels);
    void setLEDWhiteFromPixels(ofPixels &pixels);
};

class LED_Wash_Layer{
    public:
    LED_Wash_Layer();
    void draw();
    void update();
    void addGroup(uint8_t _group_id);
    void addPoint(uint8_t _group_id, uint8_t _point_id, glm::vec3 _position);
    
    void setLED(uint8_t _group_id,uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void setLEDFlash(uint8_t _group_id, uint8_t _point_id, uint8_t _flash_speed);
    
    void add2DPoint(uint8_t _group_id, uint8_t _point_id, glm::vec2 _position_2D);
    
    void add2DPointFlash(uint8_t _group_id, uint8_t _point_id, glm::vec2 _position_2D_flash);
    
    glm::vec2 get2DPoint(uint8_t _group_id, uint8_t _point_id);
    glm::vec2 get2DPointFlash(uint8_t _group_id, uint8_t _point_id);
    ofColor getColor(uint8_t _group_id, uint8_t _point_id);
    uint8_t getColorWhite(uint8_t _group_id, uint8_t _point_id);
    ofColor getColorFlash(uint8_t _group_id, uint8_t _point_id);
    
    unordered_map<uint8_t, LED_Wash_Group> groups;
    
    void setLEDFromPixels( ofPixels &pixels);
    void setLEDWhiteFromPixels(ofPixels &pixels);
    
    
};
/*
 o-------o-----------------------------------o
 ｜
 o
 */

class BamBoo_Point{
    
public:
    BamBoo_Point(glm::vec3 _position);
    virtual void draw();
    virtual void update();
    
    void setLED(uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void setLEDRGB(ofColor color);
    void setLEDWhite(uint8_t white);
    
    void add2DPoint(glm::vec2 _position_2D);
    
    glm::vec2 get2DPoint();
    ofColor getColor();
    uint8_t getColorWhite();
    
    uint8_t color_mode;
    ofColor color;
    ofColor start_color;
    ofColor target_color;
    
    uint8_t white_color;
    uint8_t start_white_color;
    uint8_t target_white_color;
    
    glm::vec3 position;
    glm::vec2 position_2D;
    uint64_t target_time;
    uint64_t start_time;
    uint64_t current_time;
};

class BamBoo_Group{
public:
    BamBoo_Group();
    void draw();
    void addPoint(uint8_t _id, glm::vec3 _position);
    void update();
    
    void setLED(uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void add2DPoint(uint8_t _point_id, glm::vec2 _position_2D);
    
    glm::vec2 get2DPoint(uint8_t _point_id);
    ofColor getColor(uint8_t _point_id);
    uint8_t getColorWhite(uint8_t _point_id);
    
    unordered_map<uint8_t, BamBoo_Point> points;
    
    void setLEDFromPixels( ofPixels &pixels);
    void setLEDWhiteFromPixels(ofPixels &pixels);
};

class BamBoo_Layer{
public:
    
    BamBoo_Layer();
    void drawLED();
    void drawBamBoo(int mode = 0);
    void update();
    void addGroup(uint8_t _group_id);
    void addPoint(uint8_t _group_id, uint8_t _point_id, glm::vec3 _position);
    
    void setLED(uint8_t _group_id,uint8_t _point_id, uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void add2DPoint(uint8_t _group_id, uint8_t _point_id, glm::vec2 _position_2D);
    
    glm::vec2 get2DPoint(uint8_t _group_id, uint8_t _point_id);
    ofColor getColor(uint8_t _group_id, uint8_t _point_id);
    uint8_t getColorWhite(uint8_t _group_id, uint8_t _point_id);
    
    unordered_map<uint8_t, BamBoo_Group> groups;
    
    void setLEDFromPixels( ofPixels &pixels);
    void setLEDWhiteFromPixels( ofPixels &pixels);
    //
    
    glm::vec3 rotation_point;
    glm::vec2 position_motor_2D;
    
    void setRotationPoint(glm::vec3 _rotation_point);
    void setMotor2DPoint(glm::vec2 _rotation_point);
    
    void setMOTOR(float _angle, int _time);
    void setMotorBezierCurveSetting(float p1x, float p1y, float p2x, float p2y);
    void setBamBooUpOrDown(bool _isUpBammBoo);
    bool isUpBamBoo;
    
    void setDegreeOrigin(float _degree_origin);
    
    float degree_origin = 0;
    float degree_draw = 0;
    float degree_motor;
    float degree_target;
    float degree_start;
    
    float degree_motorboss_draw = 0;
    float degree_motorboss;
    
    uint64_t start_time;
    uint64_t target_time;
    uint64_t current_time;
    
    float bezier_p1x = 0.5;
    float bezier_p1y = 0.0;
    float bezier_p2x = 0.5;
    float bezier_p2y = 1.0;
};



class BamBooCenter{
  
public:
    BamBooCenter();
    ~BamBooCenter();
    
    void setup();
    void loadLED2DCsv();
    void loadLED3DCsv();
    void loadMotor2DCsv();
    void loadMotor3DCsv();
    void update();
    void draw();
    
    
    unordered_map<uint8_t, BamBoo_Layer> bamboos;
    void setMOTOR(bool _isUpBamBoo, uint8_t _id, float _angle, int _time);
    void setMotorBezierCurveSetting(vector<float> p);
    int bamBooDrewMode = 1;
    void setBamBooDrew();
    
    void addLayerBamboo(uint8_t _id);
    void addLayerBambooLED(uint8_t _id);
    
    unordered_map<uint8_t,LED_Wash_Layer> led_layers_wash;
    void addLayerWash(uint8_t _id);
    
    unordered_map<uint8_t, LED_Layer> led_layers;
    void addLayer(uint8_t _id);
    
    void setLED(uint8_t _layer_id, uint8_t _group_id, uint8_t _point_id,uint8_t _color_mode, ofColor _start_color, ofColor _target_color, uint16_t _time);
    
    void setLEDFlash(uint8_t _layer_id, uint8_t _group_id, uint8_t _point_id, uint8_t flash_speed);
    
    
    unordered_map<uint8_t, LayerGroupMaxIndex> groups_max_index;
    void initGroupsMaxIndex();
    uint8_t getGroupsMaxIndex(uint8_t);
    uint8_t getPointsMaxIndex(uint8_t,uint8_t);
    
    unordered_map<uint8_t, vector<uint8_t>> cue_max_index;
    unordered_map<uint8_t, vector<uint8_t>> getLayerMaxIndex();
    
    float size_scale = 10;
    
    void setLEDFromPixels( ofPixels &pixels);
    void setLEDWhiteFromPixels( ofPixels &pixels);
    void setMotorFromPixels( ofPixels &pixels);
    
//    void readLEDColor();
    vector<float> getMotorsDegrees();
//    void readMotorDegree(string &);
    void setMotorBossDegree(Json::Value motor_boss_json);
    
    ofPixels NDI_pixels;
    void updateLED2DPiexls();
    
    unsigned char led_udp_format[38480];
    void updateLED2UDPFormat();
    
    
};

int compare_map_order(pair<uint8_t, BamBoo_Layer> a, pair<uint8_t, BamBoo_Layer> b);
#endif /* BamBoo_h */
