//
//  Utils.hpp
//  BamBooGod
//
//  Created by Fish on 2020/12/30.
//

#ifndef Utils_h
#define Utils_h

#include "ofMain.h"
#include "ofxJSON.h"
#include <stdio.h>
#include <boost/lexical_cast.hpp>

#define MOTOR_VALUE_MAX 108
#define MOTOR_VALUE_MIN 4

#define MOTOR_VALUE_LIMIT_MAX 108
#define MOTOR_VALUE_LIMIT_MIN 4

enum ColorMode{
    RGB,
    HSB
};

enum LightEffectMode{
    
};

enum MotorEffectMode{
    WAVE,
    STILL,
    STAIR
    
};

enum LayerName{
    TUBE60_11,
    TUBE60_18,
    BBDOT_10,
    BBTUBE120_09,
    BBDOT_13,
    BBTUBE120_14,
    CIRCLE_16,
    LANTERN_07,
    LANTERN_05,
    CENTER03_STRIP,
    CENTER02_STRIP,
    DOT_22,
    TUBE60_21,
    TUBE60_20,
    TUBE60_19,
    TUBE120_17,
    TUBE60_15,
    DOT_12,
    TUBE60_08,
    GLASS_06,
    GLASS_04,
    GLASS_03,
    TUBE60_02,
    TUBE60_01,
    CENTER01_GLASS,
    CENTER04_WASH,
    CENTER05_WASH,
    CENTER06_WASH,
    CENTER07_BEAM,
    CENTER08_WASH,
    CENTER04_WASH_FLASH,
    CENTER05_WASH_FLASH,
    CENTER06_WASH_FLASH,
    CENTER08_WASH_FLASH,
    NONE
    
    
};

int layerName2Enum(string layerName);
int layerNumber2Enum(uint8_t number);
uint8_t groupMaxIndex(vector<uint8_t> id, unordered_map<uint8_t, vector<uint8_t>>);
uint8_t pointMaxIndex(vector<uint8_t> id, unordered_map<uint8_t, vector<uint8_t>>);

class LayerGroupMaxIndex{
    
public:
    void addGroupsIndex(uint8_t);
    void setGroupsIndex(uint8_t);
    void setPointsIndex(uint8_t, uint8_t);
    
    uint8_t getMaxGroupsIndex();
    uint8_t getMaxPointsIndex(uint8_t g_index);
    
    uint8_t groups_max_index;
    uint8_t points_max_index;
//    unordered_map<uint8_t, uint8_t> points_max_index;
    
};

int Json_ReadInt(Json::Value JV, int ori_value = -1000.0);
double Json_ReadDouble(Json::Value JV, double ori_value = -1000.0);
string Json_ReadString(Json::Value JV, string ori_value = "");
bool Json_ReadBool(Json::Value JV, bool ori_value = false);

int parsingMotorLayer(string string_layer);
vector<uint8_t> parsingMotorIndex(vector<string> vector_string_id);

//int compare_map_order(pair<uint8_t, BamBoo_Layer> a, pair<uint8_t, BamBoo_Layer> b);

float lerp(float n1, float n2, float percent);
float bezier_curve(float p1x, float p1y, float p2x, float p2y, float percent);

#endif /* Utils_hpp */
