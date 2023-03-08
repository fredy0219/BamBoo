//
//  CueCenter.cpp
//  BamBooGod
//
//  Created by Fish on 2021/1/6.
//

#include "ofMain.h"
#include "CueCenter.h"
#include <boost/lexical_cast.hpp>
using namespace std;

void CueLEDParserThread::setup(unordered_map<uint8_t, vector<uint8_t>> _led_layers_max_index)
{

    this->led_layers_max_index = _led_layers_max_index;
}

void CueLEDParserThread::threadedFunction()
{

    while (isThreadRunning())
        if (cue_match_container.size() > 0)
        {
            cue_match = cue_match_container.front();
            cue_match_container.pop();
            
            if(cue_match[42] == "FLASH")
                parsingLEDFlash();
            else
                parsingLED();
        }
}

void CueLEDParserThread::addCueMatch(smatch _cue_match)
{
    cue_match_container.push(_cue_match);
}

bool CueLEDParserThread::getCueMatch(CueLEDStruct &_cue_led_struct)
{

    if (cue_finish_container.size() > 0)
    {

        _cue_led_struct = cue_finish_container.front();
        cue_finish_container.pop();
        return true;
    }

    return false;
}

void CueLEDParserThread::addStatusMessage(string _cue_status){
    cue_status_messages.push(_cue_status);
}

bool CueLEDParserThread::getStatusMessage(string &_cue_status_message){
    
    if(cue_status_messages.size() > 0){
        _cue_status_message = cue_status_messages.front();
        cue_status_messages.pop();
        return true;
    }else{
        
        return false;
    }
}

void CueLEDParserThread::parsingLED()
{

    // layer
    //int layer_id = layerName2Enum(cue_match[4].str());
    
    vector<string> layer_c = ofSplitString(cue_match[4], ",");
    vector<uint8_t> layers_id = parsingLayerIndex(layer_c);
    
    
    int group_max_index = groupMaxIndex(layers_id, led_layers_max_index);
    int point_max_index = pointMaxIndex(layers_id, led_layers_max_index);;
    
    //groups
    vector<string> group_c = ofSplitString(cue_match[5], ",");
    vector<uint8_t> groups_id = parsingGroupIndex(group_c, group_max_index);
    
    // points
    vector<string> point_c = ofSplitString(cue_match[6], ",");
    vector<uint8_t> points_id = parsingPointIndex(point_c, point_max_index);
    

    // color mode
    int color_mode = parsingColorMode(cue_match[7]);

    // colors
    vector<string> color_c = ofSplitString(cue_match[8], "|");
    vector<ofColor> colors = parsingColor(color_c);
    

    // time
    int time = 0;
    if (cue_match[9] != "")
        time = parsingTime(cue_match[9]);

    if (layers_id.size() >= 0 && groups_id.size() > 0 && points_id.size() > 0 && color_mode != 100 && colors.size() > 0 && time >= 0)
    {
        
        CueLEDStruct _cue_led_struct;
        _cue_led_struct.layers_id = layers_id;
        _cue_led_struct.groups_id = groups_id;
        _cue_led_struct.points_id = points_id;
        _cue_led_struct.color_mode = color_mode;
        _cue_led_struct.colors = colors;
        _cue_led_struct.time = time;

        cue_finish_container.push(_cue_led_struct);
    }
    else
    {
        addStatusMessage("[Q] : cue LED sytax parser error!");
    }
}

void CueLEDParserThread::parsingLEDFlash(){
    
    int flash_index = 42;
    
    vector<string> layer_c = ofSplitString(cue_match[flash_index+1], ",");
    vector<uint8_t> layers_id = parsingLayerIndex(layer_c);
    
    for(int i = 0 ; i < layers_id.size() ; i++)
        if(layers_id[i] == 25 || layers_id[i] == 26 || layers_id[i] == 27 || layers_id[i] == 29){
            
            
        }else{
            addStatusMessage("[Q] : cue LED Flash index sytax parser error!");
            return;
        }
    
    int group_max_index = groupMaxIndex(layers_id, led_layers_max_index);
    int point_max_index = pointMaxIndex(layers_id, led_layers_max_index);;
    
    //groups
    vector<string> group_c = ofSplitString(cue_match[flash_index+2], ",");
    vector<uint8_t> groups_id = parsingGroupIndex(group_c, group_max_index);
    
    // points
    vector<string> point_c = ofSplitString(cue_match[flash_index+3], ",");
    vector<uint8_t> points_id = parsingPointIndex(point_c, point_max_index);
    
    int flash_speed= 0;
    flash_speed = parsingFlash(cue_match[flash_index+4]);
    
    if (layers_id.size() >= 0 && groups_id.size() > 0 && points_id.size() > 0 && flash_speed >= 0)
    {
        
        CueLEDStruct _cue_led_struct;
        _cue_led_struct.layers_id = layers_id;
        _cue_led_struct.groups_id = groups_id;
        _cue_led_struct.points_id = points_id;
        _cue_led_struct.flash_speed = flash_speed;
        _cue_led_struct.isFlashCue = true;
        cue_finish_container.push(_cue_led_struct);
    }
    else
    {
        addStatusMessage("[Q] : cue LED sytax parser error!");
    }
    
}

vector<uint8_t> CueLEDParserThread::parsingLayerIndex(vector<string> vector_string_id){
    
    vector<uint8_t> id;
    for(size_t i = 0 ; i < vector_string_id.size() ;i++){
        vector<string> layer_c = ofSplitString(vector_string_id[i], "-");
        
        switch(layer_c.size()){
            case 1:{
                int from_id;
                if (boost::conversion::try_lexical_convert<int>(layer_c[0], from_id) && from_id >= 0 && from_id <=30)
                {
                    if(find( id.begin(), id.end(), from_id) == id.end())
                        id.push_back((uint8_t)layerNumber2Enum(from_id));
                }
                else if (layer_c[0] == "ALL")
                {
                    for (uint8_t i = 1; i <= 30; i++){
                        uint8_t layer_id = layerNumber2Enum(i);
                        if(find( id.begin(), id.end(), layer_id) == id.end())
                            id.push_back(layer_id);
                    }
                }
                else
                {
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                }
                break;
            }
            case 2:{
                int from_id;
                int to_id;
                
                
                if ((!boost::conversion::try_lexical_convert<int>(layer_c[0], from_id) ||
                    !boost::conversion::try_lexical_convert<int>(layer_c[1], to_id)) ||
                    from_id == 0 || from_id > 30 || to_id == 0 || to_id > 30 || from_id >= to_id){
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                    break;
                }
                
                
                for (uint8_t i = from_id; i <= to_id; i++){
                    uint8_t layer_id = layerNumber2Enum(i);
                    if( find( id.begin(), id.end(), layer_id) == id.end())
                        id.push_back(layer_id);
                }
                
                break;
            }
            case 3:{
                int from_id;
                int to_id;
                int devide_number = -1;
                if ((!boost::conversion::try_lexical_convert<int>(layer_c[0], from_id) ||
                    !boost::conversion::try_lexical_convert<int>(layer_c[1], to_id)) ||
                    from_id == 0 || from_id > 30 || to_id == 0 || to_id > 30 || from_id >= to_id){
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                    break;
                }
            
                if (boost::conversion::try_lexical_convert<int>(layer_c[2], devide_number) &&
                    devide_number > 0 && devide_number < to_id){
                    
                    for (uint8_t i = from_id; i <= to_id; i++){
                        uint8_t layer_id = layerNumber2Enum(i);
                        if (i % devide_number == 0 && find( id.begin(), id.end(), layer_id) == id.end())
                            id.push_back(layer_id);
                    }
                }
                else if (layer_c[2] == "o")
                {
                    for (uint8_t i = from_id; i <= to_id; i++){
                        uint8_t layer_id = layerNumber2Enum(i);
                        if (i % 2 == 1 && find( id.begin(), id.end(), layer_id) == id.end())
                            id.push_back(layer_id);
                    }
                }
                else if (layer_c[2] == "e")
                {
                    for (uint8_t i = from_id; i <= to_id; i++){
                        uint8_t layer_id = layerNumber2Enum(i);
                        if (i % 2 == 0 && find( id.begin(), id.end(), layer_id) == id.end())
                            id.push_back(layer_id);
                    }
                }
                else
                {
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                }
                break;
            }
            default:
            {
                addStatusMessage("[Q] : cue layer index sytax parser error!");
                break;
            }
        }
        
    }
    sort(id.begin(), id.end());
    return id;
}

vector<uint8_t> CueLEDParserThread::parsingGroupIndex(vector<string> vector_string_id, int group_max_index)
{
    
    vector<uint8_t> id;
    for(size_t i = 0 ; i < vector_string_id.size() ;i++){
        vector<string> group_c = ofSplitString(vector_string_id[i], "-");
        
        switch(group_c.size()){
            case 1:{
                int from_id;
                if (boost::conversion::try_lexical_convert<int>(group_c[0], from_id) && from_id >= 0 && from_id <=group_max_index)
                {
                    if(find( id.begin(), id.end(), from_id) == id.end())
                        id.push_back(from_id);
                }
                else if (group_c[0] == "ALL")
                {
                    for (uint8_t i = 1; i <= group_max_index; i++)
                        if(find( id.begin(), id.end(), i) == id.end())
                            id.push_back(i);
                }
                else
                {
                    addStatusMessage("[Q] : cue group index sytax parser error!");
                }
                break;
            }
            case 2:{
                int from_id;
                int to_id;
                
                
                if ((!boost::conversion::try_lexical_convert<int>(group_c[0], from_id) ||
                     !boost::conversion::try_lexical_convert<int>(group_c[1], to_id)) ||
                    from_id == 0 || from_id > group_max_index || to_id == 0 || to_id > group_max_index || from_id >= to_id){
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                    break;
                }
                
                
                for (uint8_t i = from_id; i <= to_id; i++){
                    if( find( id.begin(), id.end(), i) == id.end())
                        id.push_back(i);
                }
                
                break;
            }
            case 3:{
                int from_id;
                int to_id;
                int devide_number = -1;
                if ((!boost::conversion::try_lexical_convert<int>(group_c[0], from_id) ||
                     !boost::conversion::try_lexical_convert<int>(group_c[1], to_id)) ||
                    from_id == 0 || from_id > group_max_index || to_id == 0 || to_id > group_max_index || from_id >= to_id){
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                    break;
                }
                
                if (boost::conversion::try_lexical_convert<int>(group_c[2], devide_number) &&
                    devide_number > 0 && devide_number < to_id){
                    
                    for (uint8_t i = from_id; i <= to_id; i++)
                        if (i % devide_number == 0 && find( id.begin(), id.end(), i) == id.end())
                            id.push_back(i);
                }
                else if (group_c[2] == "o")
                {
                    for (uint8_t i = from_id; i <= to_id; i++)
                        if (i % 2 == 1 && find( id.begin(), id.end(), i) == id.end())
                            id.push_back(i);
                }
                else if (group_c[2] == "e")
                {
                    for (uint8_t i = from_id; i <= to_id; i++)
                        if (i % 2 == 0 && find( id.begin(), id.end(), i) == id.end())
                            id.push_back(i);
                }
                else
                {
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                }
                break;
            }
            default:
            {
                addStatusMessage("[Q] : cue layer index sytax parser error!");
                break;
            }
        }
    }
    
    sort(id.begin(), id.end());
    return id;
}

vector<uint8_t> CueLEDParserThread::parsingPointIndex(vector<string> vector_string_id, int point_max_index)
{

    vector<uint8_t> id;
    for(size_t i = 0 ; i < vector_string_id.size() ;i++){
        vector<string> point_c = ofSplitString(vector_string_id[i], "-");
        
        switch(point_c.size()){
            case 1:{
                int from_id;
                if (boost::conversion::try_lexical_convert<int>(point_c[0], from_id) && from_id >= 0 && from_id <=point_max_index)
                {
                    if(find( id.begin(), id.end(), from_id) == id.end())
                        id.push_back(from_id);
                }
                else if (point_c[0] == "ALL")
                {
                    for (uint8_t i = 1; i <= point_max_index; i++)
                        if(find( id.begin(), id.end(), i) == id.end())
                            id.push_back(i);
                }
                else
                {
                    addStatusMessage("[Q] : cue group index sytax parser error!");
                }
                break;
            }
            case 2:{
                int from_id;
                int to_id;
                
                
                if ((!boost::conversion::try_lexical_convert<int>(point_c[0], from_id) ||
                     !boost::conversion::try_lexical_convert<int>(point_c[1], to_id)) ||
                    from_id == 0 || from_id > point_max_index || to_id == 0 || to_id > point_max_index || from_id >= to_id){
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                    break;
                }
                
                
                for (uint8_t i = from_id; i <= to_id; i++){
                    if( find( id.begin(), id.end(), i) == id.end())
                        id.push_back(i);
                }
                
                break;
            }
            case 3:{
                int from_id;
                int to_id;
                int devide_number = -1;
                if ((!boost::conversion::try_lexical_convert<int>(point_c[0], from_id) ||
                     !boost::conversion::try_lexical_convert<int>(point_c[1], to_id)) ||
                    from_id == 0 || from_id > point_max_index || to_id == 0 || to_id > point_max_index || from_id >= to_id){
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                    break;
                }
                
                if (boost::conversion::try_lexical_convert<int>(point_c[2], devide_number) &&
                    devide_number > 0 && devide_number < to_id){
                    
                    for (uint8_t i = from_id; i <= to_id; i++)
                        if (i % devide_number == 0 && find( id.begin(), id.end(), i) == id.end())
                            id.push_back(i);
                }
                else if (point_c[2] == "o")
                {
                    for (uint8_t i = from_id; i <= to_id; i++)
                        if (i % 2 == 1 && find( id.begin(), id.end(), i) == id.end())
                            id.push_back(i);
                }
                else if (point_c[2] == "e")
                {
                    for (uint8_t i = from_id; i <= to_id; i++)
                        if (i % 2 == 0 && find( id.begin(), id.end(), i) == id.end())
                            id.push_back(i);
                }
                else
                {
                    addStatusMessage("[Q] : cue layer index sytax parser error!");
                }
                break;
            }
            default:
            {
                addStatusMessage("[Q] : cue layer index sytax parser error!");
                break;
            }
        }
    }
    sort(id.begin(), id.end());
    return id;
}

int CueLEDParserThread::parsingColorMode(string _cue_match)
{

    uint8_t color_mode = 100;

    if (_cue_match == "RGB")
        color_mode = 0;
    else if (_cue_match == "HSB")
        color_mode = 1;
    else
        addStatusMessage("[Q] : cue color mode sytax parser error!");

    return color_mode;
}

vector<ofColor> CueLEDParserThread::parsingColor(vector<string> vector_string_color)
{

    vector<ofColor> colors;
    for (int i = 0; i < vector_string_color.size(); i++)
    {
        vector<string> split_code = ofSplitString(vector_string_color[i], ",");
        if (split_code.size() == 4){
            
            int code;
            int tmpCode[4] = {-1, -1, -1, -1};
            for (int j = 0; j < 4; j++)
                if (boost::conversion::try_lexical_convert<int>(split_code[j], code))
                {
                    if (code >= 0 && code <= 255)
                        tmpCode[j] = code;
                    
                }
            if (tmpCode[0] != -1 && tmpCode[1] != -1 && tmpCode[2] != -1 && tmpCode[3] != -1){
                
                colors.push_back(ofColor(tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3]));
//                cout<<tmpCode[3]<<endl;
            }
            else
            {
                addStatusMessage("[Q] : cue color sytax parser error!");
                return colors;
            }
            
        }else if (split_code.size() == 3){
            int code;
            int tmpCode[3] = {-1, -1, -1};
            for (int j = 0; j < 3; j++)
                if (boost::conversion::try_lexical_convert<int>(split_code[j], code))
                {
                    if (code >= 0 && code <= 255)
                        tmpCode[j] = code;
                }
            if (tmpCode[0] != -1 && tmpCode[1] != -1 && tmpCode[2] != -1){
                
                colors.push_back(ofColor(tmpCode[0], tmpCode[1], tmpCode[2], 0));
            }
            else
            {
                addStatusMessage("[Q] : cue color sytax parser error!");
                return colors;
            }
        }else{
            
            addStatusMessage("[Q] : cue color sytax parser error!");
            return colors;
        }
    }

    if (vector_string_color.size() == 1){
        colors.push_back(colors[0]);
    }

    return colors;
}

int CueLEDParserThread::parsingTime(string string_time)
{
    int time = -1;

    int tmp_time;
    if (boost::conversion::try_lexical_convert<int>(string_time, tmp_time))
        time = tmp_time;
    else
        addStatusMessage("[Q] : cue time sytax parser error!");

    return time;
}

int CueLEDParserThread::parsingFlash(string string_flash)
{
    int flash = -1;
    
    int tmp_flash;
    if (boost::conversion::try_lexical_convert<int>(string_flash, tmp_flash) && tmp_flash >= 0 && tmp_flash <=255)
        flash = tmp_flash;
    else
        addStatusMessage("[Q] : cue flash sytax parser error!");
    
    return flash;
}

////////////////////////
void CueMotorParserThread::setup()
{
}

void CueMotorParserThread::threadedFunction()
{

    while (isThreadRunning())
        if (cue_match_container.size() > 0)
        {
            cue_match = cue_match_container.front();
            cue_match_container.pop();
            parsingMotor();
        }
}

void CueMotorParserThread::addCueMatch(smatch _cue_match)
{
    cue_match_container.push(_cue_match);
}

bool CueMotorParserThread::getCueMatch(CueMotorStruct &_cue_motor_struct)
{

    if (cue_finish_container.size() > 0)
    {

        _cue_motor_struct = cue_finish_container.front();
        cue_finish_container.pop();
        return true;
    }

    return false;
}

void CueMotorParserThread::addStatusMessage(string _cue_status){
    cue_status_messages.push(_cue_status);
}

bool CueMotorParserThread::getStatusMessage(string &_cue_status_message){
    
    if(cue_status_messages.size() > 0){
        _cue_status_message = cue_status_messages.front();
        cue_status_messages.pop();
        return true;
    }else{
        return false;
    }
}

void CueMotorParserThread::parsingMotor()
{
    
    bool isUpBamboo = parsingUpOrDown(cue_match[10]);

    vector<uint8_t> motor_ids;
    vector<string> motor_ids_c = ofSplitString(cue_match[11], "-");
    motor_ids = parsingMotorIndex(cue_match[10], motor_ids_c);

    float angle;
    angle = parsingDegree(cue_match[10], cue_match[12]);

    int time;
    time = parsingTime(cue_match[13]);

    if (motor_ids_c.size() > 0 && angle != -1000.0 && time >= 0)
    {
        CueMotorStruct motorStruct;
        motorStruct.isUpBamBoo = isUpBamboo;
        motorStruct.id = motor_ids;
        motorStruct.angle = angle;
        motorStruct.time = time;

        cue_finish_container.push(motorStruct);
    }
    else
    {
        addStatusMessage("[Q] : cue MOTOR sytax parser error!");
    }
}

bool CueMotorParserThread::parsingUpOrDown(string _updown_string)
{
    if (_updown_string == "U")
        return true;
    else
        return false;
}

vector<uint8_t> CueMotorParserThread::parsingMotorIndex(string layer, vector<string> vector_string_id)
{
    vector<uint8_t> id;
    switch (vector_string_id.size())
    {
    case 1:
    {
        int from_id;
        if (boost::conversion::try_lexical_convert<int>(vector_string_id[0], from_id) && from_id>=1 && from_id<=54)
        {
            id.push_back((uint8_t)from_id);
        }
        else if (vector_string_id[0] == "ALL")
        {
            for (uint8_t i = 1; i <= 54; i++)
                id.push_back(i);
        }
        else
        {
            addStatusMessage("[Q] : cue motor index sytax parser error!");
        }
        break;
    }
    case 2:
    {
        int from_id;
        int to_id;
        if (!boost::conversion::try_lexical_convert<int>(vector_string_id[0], from_id) ||
            !boost::conversion::try_lexical_convert<int>(vector_string_id[1], to_id))
        {
            addStatusMessage("[Q] : cue motor index sytax parser error!");
            break;
        }

        to_id = 54 < to_id ? 54 : to_id;

        for (uint8_t i = from_id; i <= to_id; i++)
        {
            id.push_back(i);
        }

        break;
    }

    case 3:
    {
        int from_id;
        int to_id;
        int devide_number = -1;
        if (!boost::conversion::try_lexical_convert<int>(vector_string_id[0], from_id) ||
            !boost::conversion::try_lexical_convert<int>(vector_string_id[1], to_id))
        {
            addStatusMessage("[Q] : cue point index sytax parser error!");
            break;
        }
        
        to_id = 54 < to_id ? 54 : to_id;

        if (boost::conversion::try_lexical_convert<int>(vector_string_id[2], devide_number) &&
            devide_number > 0)
        {

            for (uint8_t i = from_id; i <= to_id; i++)
                if (i % devide_number == 0)
                    id.push_back(i);
        }
        else if (vector_string_id[2] == "o")
        {
            for (uint8_t i = from_id; i <= to_id; i++)
                if (i % 2 == 1)
                    id.push_back(i);
        }
        else if (vector_string_id[2] == "e")
        {
            for (uint8_t i = from_id; i <= to_id; i++)
                if (i % 2 == 0)
                    id.push_back(i);
        }
        else
        {
            addStatusMessage("[Q] : cue motor index syntax parser error!");
        }

        break;
    }
    default:
    {
        addStatusMessage("[Q] : d cue motor index syntax parser error!");
    }
    }

    return id;
}

float CueMotorParserThread::parsingDegree(string layer, string string_degree)
{
    float angle = -1000;

    float tmp_angle;
    if (!boost::conversion::try_lexical_convert<float>(string_degree, tmp_angle))
    {
        addStatusMessage("[Q] : cue angle syntax parser error!");
    }
    else
    {
        if (layer == "U" && tmp_angle <= MOTOR_VALUE_MAX && tmp_angle >= -MOTOR_VALUE_MIN)
        {
            angle = tmp_angle;
        }
        else if (layer == "D" && tmp_angle >= -MOTOR_VALUE_MAX && tmp_angle <= MOTOR_VALUE_MIN)
        {
            angle = tmp_angle;
        }
        else
        {
            addStatusMessage("[Q] : cue angle syntax parser error!");
        }
    }
    return angle;
}

int CueMotorParserThread::parsingTime(string string_time)
{
    int time = -1;

    int tmp_time;
    if (boost::conversion::try_lexical_convert<int>(string_time, tmp_time))
        time = tmp_time;
    else
        addStatusMessage("[Q] : cue time sytax parser error!");

    return time;
}


CueCenter::CueCenter()
{
    
    /*
        /cue/LED/([0-9\,-oe]+|ALL)\/([0-9\,-oe]+|ALL)\/([0-9\,-oe]+|ALL)\/([A-Z\_]+)\/([0-9\,\|]+)\/([0-9]*)\/
        /cue/MOTOR/([U|D]{1})\/([0-9\-oe]+|ALL)\/([0-9\-\.]+)\/([0-9]+)\/
        /cue/PLC/(ALL_HIMC)\/(ON|OFF)\/
        /cue/MADRIX/(LOAD)\/([A-B])\/([0-9]+)\/
        /cue/MADRIX/(PLAY|PAUSE|DISPLAY)\/([A-B])\/
        /cue/MADRIX/(RGBA)\/([0-9,]+)\/
        /cue/BB/(INPUT)\/(CUE|VIDEO)\/
        /cue/BB/(NDI)\/(ON|OFF)\/
        /cue/BB/(MOTOR)\/(OM|OFF)
        ** /cue/MOTOR/\/
            -> (WAVE)\/([U|D]{1}|ALL)\/([0-9]+)\/([0-9\-]+)\/([0-9]+)\/
     -> (STAIR)\/([U|D]{1}|ALL)\/([0-9\-oe]+|ALL])\/([0-9\-\~\.]+)\/([0-9\.]+)\/
     
     
    */

    reg_standard = regex("\/(cue)\/(LIGHT|MOTOR|PLC|MADRIX|BB|VIDEO|LIGHTBOSS)\/(([0-9\,-oe]+|ALL)\/([0-9\,-oe]+|ALL)\/([0-9\,-oe]+|ALL)\/([A-Z\_]+)\/([0-9\,\|]+)\/([0-9]*)\/|([U|D]{1})\/([0-9\-oe]+|ALL)\/([0-9\-\.]+)\/([0-9]+)\/|(MOTOR)\/(ON|OFF)\/|(LOAD)\/([LR])\/([0-9]+)\/|(PLAY|PAUSE|DISPLAY)\/([LR])\/|(RGBA)\/([0-9,]+)\/|(INPUT)\/(QLAB|VIDEO)\/|(REALTIME)\/(ON|OFF)\/|(LIGHT|LIGHT_WHITE|MOTOR)\/([a-zA-Z0-9\/-_. ]+)\/|(WAVE)\/([U|D]{1}|ALL)\/([0-9\,-oe]+|ALL)\/([0-9\-\~]+)\/([0-9\-\~\.]+)\/([0-9]+)\/|(STAIR)\/([U|D]{1}|ALL)\/([0-9\,-oe]+|ALL)\/([0-9\.]+)\/([0-9\.-]+)\/|(CURVE)\/([0-9\.,]+)\/|(FLASH)\/([0-9\,-]+|ALL)\/([0-9\,-]+|ALL)\/([0-9\,-]+|ALL)\/([0-9]*)\/|(FILTER)\/([0-9\,]+)\/([0-9\,.]+)\/)");
    
}

void CueCenter::setup(unordered_map<uint8_t, vector<uint8_t>> _led_layers_max_index)
{
    led_layers_max_index = _led_layers_max_index;

    cue_led_parser_thread.setup(led_layers_max_index);
    cue_led_parser_thread.startThread();

    cue_motor_parser_thread.setup();
    cue_motor_parser_thread.startThread();
}

void CueCenter::update(string cue, bool isCueEnable)
{

    if (cue.size() > 0)
        if (current_cue_message != cue)
        {
            isNewCue = true;
            current_cue_message = cue;
        }

    if (isNewCue)
    {
        parsingCue( isCueEnable );
        isNewCue = false;
    }

    CueLEDStruct new_cue_led_struct;
    if(cue_led_parser_thread.getCueMatch(new_cue_led_struct))
        cue_led_queue.push(new_cue_led_struct);
    
    string new_status_message;
    if(cue_led_parser_thread.getStatusMessage(new_status_message))
        status_messages.push(new_status_message);

    CueMotorStruct new_cue_motor_struct;
    if (cue_motor_parser_thread.getCueMatch(new_cue_motor_struct))
        cue_motor_queue.push(new_cue_motor_struct);

    if(cue_motor_parser_thread.getStatusMessage(new_status_message))
        status_messages.push(new_status_message);
    
}

void CueCenter::exit()
{
    cue_led_parser_thread.waitForThread(true);
    cue_motor_parser_thread.waitForThread(true);
}

void CueCenter::parsingCue(bool isCueEnable)
{

    smatch cue_match;
    
    
    if (regex_match(current_cue_message, cue_match, reg_standard))
    {
        
//        for(int i = 0 ; i < cue_match.size() ;i++)
//            cout<< i << " : " << cue_match[i].str() <<endl;

        if (cue_match[1].str() == "cue")
        {
            addStatusMessage("[Q] : received " + current_cue_message);
            
            if(cue_match[2].str() == "LIGHT" && cue_match[42] == "FLASH" && isCueEnable == true){
                cue_led_parser_thread.addCueMatch(cue_match);
                
            }else if (cue_match[2].str() == "LIGHT" && cue_match[11] == "" && cue_match[29] == "" && isCueEnable == true)
            {
                cue_led_parser_thread.addCueMatch(cue_match);
            }else if(cue_match[2].str() == "MOTOR" &&
                     (cue_match[29].str() == "WAVE" || cue_match[35].str() == "STAIR")){
                parsingMotorEffect(cue_match);
            }else if (cue_match[2].str() == "MOTOR" && cue_match[11] != "" && cue_match[29] == "" && isCueEnable == true)
            {
                cue_motor_parser_thread.addCueMatch(cue_match);
            }else if(cue_match[2].str() == "PLC" && cue_match[29] != ""){
                parsingPLC(cue_match);
            }else if(cue_match[2].str() == "MADRIX" || cue_match[2].str() == "LIGHTBOSS"){
                parsingMadrix(cue_match);
            }else if(cue_match[2].str() == "BB"){
                parsingBB(cue_match);
            }else if(cue_match[2].str() == "VIDEO"){
                parsingVideo(cue_match);
            }else{
                addStatusMessage("[Q] : cue regex parser error!");
            }
        }
    }
    else
    {
        addStatusMessage("[Q] : cue regex parser error!");
    }
}

void CueCenter::parsingMotorEffect(smatch _cue_match){
    
    int wave_index = 29;
    int stair_index = wave_index + 5 + 1;
    
    CueMotorStruct cue_motor_struct;
    if(_cue_match[wave_index].str() == "WAVE"){
        
        cue_motor_struct.effectMode = WAVE;
        
        // -- Group select
        int group_select = parsingMotorLayer(_cue_match[wave_index+1].str());
        
        if(group_select > 0 && group_select < 4)
            cue_motor_struct.group_select = group_select;
        else
            addStatusMessage("[Q] : cue motor group_select syntax parser error!");
        
        // -- IDs
        vector<string> id_string = ofSplitString(_cue_match[wave_index+2].str(), "-");
        vector<uint8_t> ids = parsingMotorIndex(id_string);
        
        if(ids.size() > 0)
            cue_motor_struct.id = ids;
        else
            addStatusMessage("[Q] : cue motor ids syntax parser error!");
            
        // -- Crest size
        int crest_size = 0;
        if (boost::conversion::try_lexical_convert<int>(_cue_match[wave_index+3].str(), crest_size))
            cue_motor_struct.crest_size = crest_size;
        else
            addStatusMessage("[Q] : cue motor effect crest index sytax parser error!");
        
        // -- Motor degree
        vector<string> motor_degree = ofSplitString(_cue_match[wave_index+4], "~");
        float from_degree = -1000;
        float to_degree = -1000;
        
        if(motor_degree.size() == 2){
            if(boost::conversion::try_lexical_convert<float>(motor_degree[0], from_degree) && boost::conversion::try_lexical_convert<float>(motor_degree[1], to_degree)){
                cue_motor_struct.from_degree = from_degree;
                cue_motor_struct.to_degree = to_degree;
            }else{
                addStatusMessage("[Q] : cue motor effect degree sytax parser error!");
            }
        }else{
            addStatusMessage("[Q] : cue motor effect degree sytax parser error!");
        }
        
        // -- SIN period
        int sin_period = -1;
        if(boost::conversion::try_lexical_convert<int>(_cue_match[wave_index+5].str(),sin_period)){
            cue_motor_struct.sin_period = sin_period;
        }else{
            addStatusMessage("[Q] : cue motor effect sin period sytax parser error!");
        }
        
        // -- ALL Check
        if( group_select != 0 && crest_size != 0 && from_degree != -1000 && to_degree != -1000 && sin_period != -1 ){
            cue_motor_queue.push(cue_motor_struct);
        }else{
            addStatusMessage("[Q] : cue motor effect WAVE parser error!");
        }
        
        //////////////////////////////////////////////////////////
    }else if(_cue_match[stair_index].str() == "STAIR"/*34*/){
        
        cue_motor_struct.effectMode = STAIR;
        
        // -- Group select
        int group_select = parsingMotorLayer(_cue_match[stair_index+1].str());
        if(group_select > 0 && group_select < 3)
            cue_motor_struct.group_select = group_select;
        else
            addStatusMessage("[Q] : cue motor group_select syntax parser error!");
        
        // -- IDs
        vector<string> id_string = ofSplitString(_cue_match[stair_index+2].str(), "-");
        vector<uint8_t> ids = parsingMotorIndex(id_string);
        if(ids.size() > 0)
            cue_motor_struct.id = ids;
        else
            addStatusMessage("[Q] : cue motor ids syntax parser error!");
        
        // -- From Degree
        float from_degree = -1000;
        if(boost::conversion::try_lexical_convert<float>(_cue_match[stair_index+3].str(), from_degree))
            cue_motor_struct.from_degree = from_degree;
        else
            addStatusMessage("[Q] : cue motor effect degree sytax parser error!");
        
        // -- Step size
        float step_size = -1000;
        if(boost::conversion::try_lexical_convert<float>(_cue_match[stair_index+4].str(), step_size))
            cue_motor_struct.step_size = step_size;
        else
            addStatusMessage("[Q] : cue motor effect step_size sytax parser error!");
        
        
        // -- All Check
        if(group_select != 0 && ids.size() > 0 && from_degree != -1000 && step_size != -1000){
            cue_motor_queue.push(cue_motor_struct);
        }else{
            addStatusMessage("[Q] : cue motor effect STAIR parser error!");
        }
        
        
    }
    
}

void CueCenter::parsingBBCurveSetting(smatch _cue_match){
    
    vector<string> parameter_string = ofSplitString(_cue_match[41].str(), ",");
    
    float x2,y2,x3,y3;
    
    if (!boost::conversion::try_lexical_convert<float>(parameter_string[0], x2) || !boost::conversion::try_lexical_convert<float>(parameter_string[1], y2) || !boost::conversion::try_lexical_convert<float>(parameter_string[2], x3) || !boost::conversion::try_lexical_convert<float>(parameter_string[3], y3)){
        
        addStatusMessage("[Q] : cue BB Curve setting parameter parser error!");
    }else{
        cue_bb_curve_setting_queue.push(x2);
        cue_bb_curve_setting_queue.push(y2);
        cue_bb_curve_setting_queue.push(x3);
        cue_bb_curve_setting_queue.push(y3);
    }
    
}

void CueCenter::parsingPLC(smatch _cue_match)
{
    cue_plc_queue.push(current_cue_message);
//    if(_cue_match[14].str() == "ALL_HIMC"){
//        if(_cue_match[15].str() == "ON")
//            cue_plc_queue.push(true);
//        else if(_cue_match[15].str() == "OFF")
//            cue_plc_queue.push(false);
//        else{
//            addStatusMessage("[Q] : PLC regex parser error !");
//        }
//    }else{
//
//        addStatusMessage("[Q] : PLC regex parser error !");
//    }
    
}

void CueCenter::parsingMadrix(smatch _cue_match){
    
    if(_cue_match[47].str() == "FILTER"){
        cue_madrix_queue.push(current_cue_message);
    }else if(_cue_match[16].str() == "LOAD"||
       (_cue_match[19].str() == "PLAY" ||_cue_match[19].str() == "PAUSE"||_cue_match[19].str() == "DISPLAY" )||_cue_match[21].str() == "RGBA"){
        cue_madrix_queue.push(current_cue_message);
    }else{
        
        addStatusMessage("[Q] : MADRIX regex parser error !");
    }
    
}

void CueCenter::parsingBB(smatch _cue_match){
    
    if(_cue_match[23].str() == "INPUT"){
        
        if(_cue_match[24].str() == "QLAB")
            cue_bb_input_queue.push(true);
        else if(_cue_match[24].str() == "VIDEO")
            cue_bb_input_queue.push(false);
        
    }else if(_cue_match[25].str() == "REALTIME"){
        
        if(_cue_match[26].str() == "ON")
            cue_bb_realtime_queue.push(true);
        else if(_cue_match[26].str() == "OFF")
            cue_bb_realtime_queue.push(false);
        
    }else if(_cue_match[14].str() == "MOTOR"){
        
        if(_cue_match[15].str() == "ON")
            cue_bb_motor_enable_queue.push(true);
        else if(_cue_match[15].str() == "OFF")
            cue_bb_motor_enable_queue.push(false);
    }else if(_cue_match[40].str() == "CURVE"){
        
        parsingBBCurveSetting(_cue_match);
        
    }else{
        addStatusMessage("[Q] : BB regex parser error !");
    }
    
}

void CueCenter::parsingVideo(smatch _cue_match){
    cue_video_input_queue.push(current_cue_message);
}

bool CueCenter::popCueLEDStruct(CueLEDStruct &cue_led_struct)
{

    if (cue_led_queue.size() > 0)
    {
        cue_led_struct = cue_led_queue.front();
        cue_led_queue.pop();
        return true;
    }
    else
        return false;
}

bool CueCenter::popCueMotorStruct(CueMotorStruct &cue_motor_struct)
{

    if (cue_motor_queue.size() > 0)
    {
        cue_motor_struct = cue_motor_queue.front();
        cue_motor_queue.pop();
        return true;
    }
    else
        return false;
}

bool CueCenter::popCuePLCMessage(string &cue_plc_message){
    if(cue_plc_queue.size() > 0){
        cue_plc_message = cue_plc_queue.front();
        cue_plc_queue.pop();
        return true;
    }else{
        return false;
    }
}

//bool CueCenter::popCuePLCStatus(bool &cue_plc_status){
//
//    if(cue_plc_queue.size() > 0){
//        cue_plc_status = cue_plc_queue.front();
//        cue_plc_queue.pop();
//        return true;
//    }else
//        return false;
//
//}

bool CueCenter::popCueMADRIXSMessage(string &cue_madrix_message){
    if(cue_madrix_queue.size() > 0){
        cue_madrix_message = cue_madrix_queue.front();
        cue_madrix_queue.pop();
        return true;
    }else{
        return false;
    }
}

bool CueCenter::popCueBBInput(bool &cue_bb_input){
    if(cue_bb_input_queue.size() > 0){
        cue_bb_input = cue_bb_input_queue.front();
        cue_bb_input_queue.pop();
        return true;
    }else{
        return false;
    }
}

bool CueCenter::popCueBBRealtime(bool &cue_bb_realtime){
    if(cue_bb_realtime_queue.size() > 0){
        cue_bb_realtime = cue_bb_realtime_queue.front();
        cue_bb_realtime_queue.pop();
        return true;
    }else{
        return false;
    }
}

bool CueCenter::popCueBBMotorEnable(bool &cue_bb_motor_enable){
    if(cue_bb_motor_enable_queue.size() > 0){
        cue_bb_motor_enable = cue_bb_motor_enable_queue.front();
        cue_bb_motor_enable_queue.pop();
        return true;
    }else{
        return false;
    }
}

bool CueCenter::popCueBBCurveSetting(vector<float> &cue_bb_curve_setting){
    if(cue_bb_curve_setting_queue.size() % 4 == 0 && cue_bb_curve_setting_queue.size()!=0){
        for(int i = 0 ; i < 4 ; i++){
            cue_bb_curve_setting[i] = cue_bb_curve_setting_queue.front();
            cue_bb_curve_setting_queue.pop();
        }
        return true;
    }else{
        return false;
    }
}

bool CueCenter::popCueVideoMessage(string &cue_video_message){
    if(cue_video_input_queue.size() > 0){
        cue_video_message = cue_video_input_queue.front();
        cue_video_input_queue.pop();
        return true;
    }else{
        return false;
    }
}

void CueCenter::addStatusMessage(string message)
{
    status_messages.push(message);
}

bool CueCenter::popStatusMessage(string &message)
{

    if (status_messages.size() > 0)
    {
        message = status_messages.front();
        status_messages.pop();
        return true;
    }
    else
        return false;
}
