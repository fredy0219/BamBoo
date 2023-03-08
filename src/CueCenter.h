//
//  CueCenter.hpp
//  BamBooGod
//
//  Created by Fish on 2021/1/6.
//

#ifndef CueCenter_h
#define CueCenter_h

#include "ofMain.h"
#include <iostream>
#include <stdio.h>
#include <regex>
#include <queue>
#include <algorithm>
#include <Utils.h>

using namespace std;

struct CueLEDStruct
{
    vector<uint8_t> layers_id;
    vector<uint8_t> groups_id;
    vector<uint8_t> points_id;
    vector<ofColor> colors;
    int color_mode;
    int time;
    
    //for flash
    bool isFlashCue = false;
    uint8_t flash_speed;
};

struct CueMotorStruct
{
    bool isUpBamBoo;
    vector<uint8_t> id;
    float angle;
    int time;
    
    bool isEffectMode = false;
    MotorEffectMode effectMode = STILL;
    
    //WAVE Effect
    int group_select = 0;
    int crest_size;
    float from_degree;
    float to_degree;
    int sin_period;
    
    //STAIR
    float step_size = 0;
    
    
};

class CueLEDParserThread : public ofThread
{

public:
    void setup(unordered_map<uint8_t, vector<uint8_t>> _led_layers_max_index);
    void update();
    void threadedFunction();

    smatch cue_match;
    unordered_map<uint8_t, vector<uint8_t>> led_layers_max_index;

    void parsingLED();
    void parsingLEDFlash();
    
    vector<uint8_t> parsingLayerIndex(vector<string> vector_string_id);
    vector<uint8_t> parsingGroupIndex(vector<string> vector_string_id, int group_max_index);
    vector<uint8_t> parsingPointIndex(vector<string> vector_string_id, int point_max_index);
    vector<ofColor> parsingColor(vector<string> vector_string_color);
    int parsingColorMode(string string_color_mode);
    int parsingTime(string string_time);
    
    int parsingFlash(string string_flash);

    CueLEDStruct cue_led_struct;

    queue<smatch> cue_match_container;
    queue<CueLEDStruct> cue_finish_container;
    queue<string> cue_status_messages;

    void addCueMatch(smatch _cue_match);
    bool getCueMatch(CueLEDStruct &_cue_led_struct);
    void addStatusMessage(string _cue_status);
    bool getStatusMessage(string &_cue_status_message);
};

class CueMotorParserThread : public ofThread
{

public:
    void setup();
    void update();

    void threadedFunction();

    smatch cue_match;

    void parsingMotor();
    bool parsingUpOrDown(string _updown_string);
    vector<uint8_t> parsingMotorIndex(string layer, vector<string> vector_string_id);
    float parsingDegree(string layer, string string_angle);
    int parsingTime(string string_time);

    queue<smatch> cue_match_container;
    queue<CueMotorStruct> cue_finish_container;
    queue<string> cue_status_messages;

    void addCueMatch(smatch _cue_match);
    bool getCueMatch(CueMotorStruct &_cue_motor_struct);
    void addStatusMessage(string _cue_status);
    bool getStatusMessage(string &_cue_status_message);
};

class CueCenter
{

public:
    CueCenter();

    void setup(unordered_map<uint8_t, vector<uint8_t>>);
    void update(string cue, bool isCueEnable);
    void exit();

    bool isNewCue = false;
    string current_cue_message;

    regex reg_standard;
    void parsingCue(bool isCueEnable);

    // LED parser function
    queue<CueLEDStruct> cue_led_queue;
    CueLEDParserThread cue_led_parser_thread;
    unordered_map<uint8_t, vector<uint8_t>> led_layers_max_index;

    void parsingLEDFlash(smatch _cue_match);
    
    queue<CueMotorStruct> cue_motor_queue;
    CueMotorParserThread cue_motor_parser_thread;
    
    void parsingMotorEffect(smatch _cue_match);

    // PLC parser function
    void parsingPLC(smatch _cue_match);
//    queue<bool> cue_plc_queue;
    queue<string> cue_plc_queue;
    
    // Matrix parser function
    void parsingMadrix(smatch _cue_match);
    queue<string> cue_madrix_queue;
    
    void parsingBB(smatch _cue_match);
    queue<bool> cue_bb_input_queue;
    queue<bool> cue_bb_realtime_queue;
    queue<bool> cue_bb_motor_enable_queue;
    void parsingBBCurveSetting(smatch _cue_match);
    queue<float> cue_bb_curve_setting_queue;
    
    void parsingVideo(smatch _cue_match);
    queue<string> cue_video_input_queue;

    bool popCueLEDStruct(CueLEDStruct &cue_led_struct);
    bool popCueMotorStruct(CueMotorStruct &cue_motor_struct);
    bool popCueMotorEffect();
//    bool popCuePLCStatus(bool &cue_plc_status);
    bool popCuePLCMessage(string &cue_plc_message);
    bool popCueMADRIXSMessage(string &cue_madrix_message);
    bool popCueBBInput(bool &cue_bb_input);
    bool popCueBBRealtime(bool &cue_bb_realtime);
    bool popCueBBMotorEnable(bool &cue_bb_motor_enable);
    bool popCueBBCurveSetting(vector<float> &cue_bb_curve_setting);
    bool popCueVideoMessage(string &cue_video_message);
    

    // Status
    queue<string> status_messages;
    void addStatusMessage(string message);
    bool popStatusMessage(string &message);
};

#endif /* CueCenter_h */
