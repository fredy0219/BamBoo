//
//  Utils.cpp
//  BamBooGod
//
//  Created by Fish on 2020/12/30.
//


#include "Utils.h"
int layerName2Enum(string layerName){
    string l = layerName;
    if(l == "11_Tube60"){
        return TUBE60_11;
    }else if(l == "18_Tube60")
        return TUBE60_18;
    else if(l == "10_BBDot")
        return BBDOT_10;
    else if(l == "09_BBTube120")
        return BBTUBE120_09;
    else if(l == "13_BBDot")
        return BBDOT_13;
    else if(l == "14_BBTube120")
        return BBTUBE120_14;
    else if(l == "16_Circle")
        return CIRCLE_16;
    else if(l == "07_Lantern")
        return LANTERN_07;
    else if(l == "05_Lantern")
        return LANTERN_05;
    else if(l == "Center03_Strip")
        return CENTER03_STRIP;
    else if(l == "Center02_Strip")
        return CENTER02_STRIP;
    else if(l == "22_Dot")
        return DOT_22;
    else if(l == "21_Tube60")
        return TUBE60_21;
    else if(l == "20_Tube60")
        return TUBE60_20;
    else if(l == "19_Tube60")
        return TUBE60_19;
    else if(l == "17_Tube120")
        return TUBE120_17;
    else if(l == "15_Tube60")
        return TUBE60_15;
    else if(l == "12_Dot")
        return DOT_12;
    else if(l == "08_Tube60")
        return TUBE60_08;
    else if(l == "06_Glass")
        return GLASS_06;
    else if(l == "04_Glass")
        return GLASS_04;
    else if(l == "03_Glass")
        return GLASS_03;
    else if(l == "02_Tube60")
        return TUBE60_02;
    else if(l == "01_Tube60")
        return TUBE60_01;
    else if(l == "Center01_Glass")
        return CENTER01_GLASS;
    else if(l == "Center04_Wash")
        return CENTER04_WASH;
    else if(l == "Center05_Wash")
        return CENTER05_WASH;
    else if(l == "Center06_Wash")
        return CENTER06_WASH;
    else if(l == "Center07_Beam")
        return CENTER07_BEAM;
    else if(l == "Center08_Wash")
        return CENTER08_WASH;
    else if(l == "Center04_Wash_flash")
        return CENTER04_WASH_FLASH;
    else if(l == "Center05_Wash_flash")
        return CENTER05_WASH_FLASH;
    else if(l == "Center06_Wash_flash")
        return CENTER06_WASH_FLASH;
    else if(l == "Center08_Wash_flash")
        return CENTER08_WASH_FLASH;
    else
        return NONE;
}

int layerNumber2Enum(uint8_t number){

    int result;
    switch(number){
        case 1:
            result = TUBE60_01; break;
        case 2:
            result = TUBE60_02; break;
        case 3:
            result = GLASS_03; break;
        case 4:
            result = GLASS_04; break;
        case 5:
            result = LANTERN_05; break;
        case 6:
            result = GLASS_06; break;
        case 7:
            result = LANTERN_07; break;
        case 8:
            result = TUBE60_08; break;
        case 9:
            result = BBTUBE120_09; break;
        case 10:
            result = BBDOT_10; break;
        case 11:
            result = TUBE60_11; break;
        case 12:
            result = DOT_12; break;
        case 13:
            result = BBDOT_13; break;
        case 14:
            result = BBTUBE120_14; break;
        case 15:
            result = TUBE60_15; break;
        case 16:
            result = CIRCLE_16; break;
        case 17:
            result = TUBE120_17; break;
        case 18:
            result = TUBE60_18; break;
        case 19:
            result = TUBE60_19; break;
        case 20:
            result = TUBE60_20; break;
        case 21:
            result = TUBE60_21; break;
        case 22:
            result = DOT_22; break;
        case 23:
            result = CENTER01_GLASS; break;
        case 24:
            result = CENTER02_STRIP; break;
        case 25:
            result = CENTER03_STRIP; break;
        case 26:
            result = CENTER04_WASH; break;
        case 27:
            result = CENTER05_WASH; break;
        case 28:
            result = CENTER06_WASH; break;
        case 29:
            result = CENTER07_BEAM; break;
        case 30:
            result = CENTER08_WASH; break;
        default:
            result = NONE;
    }
    
    return result;
    
}

uint8_t groupMaxIndex(vector<uint8_t> id, unordered_map<uint8_t, vector<uint8_t>> max_indexs){
    
    uint8_t max_index = 0;
    
    for(size_t i = 0 ; i < id.size() ; i++)
        if(max_indexs.find(id[i])->second[0] > max_index)
            max_index = max_indexs.find(id[i])->second[0];
        
    return max_index;
}

uint8_t pointMaxIndex(vector<uint8_t> id, unordered_map<uint8_t, vector<uint8_t>> max_indexs){
    
    uint8_t max_index = 0;
    
    for(size_t i = 0 ; i < id.size() ; i++)
        if(max_indexs.find(id[i])->second[1] > max_index)
            max_index = max_indexs.find(id[i])->second[1];
    
    return max_index;
}

//
void LayerGroupMaxIndex::addGroupsIndex(uint8_t _index){
    
//    if(points_max_index.find(_index) == points_max_index.end())
//        points_max_index.insert(pair<uint8_t, uint8_t>(_index, 0));
}

void LayerGroupMaxIndex::setGroupsIndex(uint8_t _index){
    
//    if(points_max_index.find(_index) == points_max_index.end())
//        points_max_index.insert(pair<uint8_t, uint8_t>(_index, 0));
    
//    if(_index >= groups_max_index)
    groups_max_index = _index;
}

void LayerGroupMaxIndex::setPointsIndex(uint8_t g_index, uint8_t p_index){
//    if(p_index > points_max_index.find(g_index)->second)
//        points_max_index.find(g_index)->second = p_index;
    
    points_max_index = p_index;
//    points_max_index.find(g_index)->second = p_index;
}

uint8_t LayerGroupMaxIndex::getMaxGroupsIndex(){
    return groups_max_index;
}
uint8_t LayerGroupMaxIndex::getMaxPointsIndex(uint8_t g_index){
    return points_max_index;
//    return points_max_index.find(g_index)->second;
}


int Json_ReadInt(Json::Value JV, int ori_value)
{
    int result = ori_value;
    Json::ValueType VT = JV.type();
    if (VT == Json::ValueType::intValue)
        result = JV.asInt();
    return result;
}
double Json_ReadDouble(Json::Value JV, double ori_value)
{
    double result = ori_value;
    Json::ValueType VT = JV.type();
//    cout<<Json::ValueType::realValue << " , " << VT <<endl;
    if (VT == Json::ValueType::realValue || VT == Json::ValueType::intValue)
        result = JV.asDouble();
    return result;
}
string Json_ReadString(Json::Value JV, string ori_value)
{
    string result = ori_value;
    Json::ValueType VT = JV.type();
    if (VT == Json::ValueType::stringValue)
        result = JV.asCString();
    return result;
}
bool Json_ReadBool(Json::Value JV, bool ori_value)
{
    bool result = ori_value;
    Json::ValueType VT = JV.type();
    if (VT == Json::ValueType::booleanValue)
        result = JV.asBool();
    return result;
}

int parsingMotorLayer(string string_layer){
    
    cout << string_layer << " ! "<< endl;
    if (string_layer == "U")
        return 1;
    else if(string_layer == "D")
        return 2;
    else if(string_layer == "ALL")
        return 3;
    else
        return 4;
}
vector<uint8_t> parsingMotorIndex(vector<string> vector_string_id)
{
    vector<uint8_t> id;
    
    string error_message = "";
    bool isParserGood = true;
    
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
            }else{
                isParserGood = false;
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
                isParserGood = false;
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
                isParserGood = false;
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
            }else{
                isParserGood = false;
            }
            
            break;
        }
        default:
        {
        }
    }
    
    vector<uint8_t> empty;
    if(isParserGood)
        return id;
    else
        return empty;
}

float lerp(float n1, float n2, float percent){
    
    float diff = n2 - n1;
    return n1 + (diff * percent);
    
}

float bezier_curve(float x2, float y2, float x3, float y3, float percent){
    
    float x1 = 0;
    float y1 = 0;
    float x4 = 1;
    float y4 = 1;
    
//    float xa = lerp( x1 , x2 , percent );
    float ya = lerp( y1 , y2 , percent );
//    float xb = lerp( x2 , x3 , percent );
    float yb = lerp( y2 , y3 , percent );
//    float xc = lerp( x3 , x4 , percent );
    float yc = lerp( y3 , y4 , percent );
    
    // The Blue Line
//    float xm = lerp( xa , xb , percent );
    float ym = lerp( ya , yb , percent );
//    float xn = lerp( xb , xc , percent );
    float yn = lerp( yb , yc , percent );
    
    // The Black Dot
//    float x = lerp( xm , xn , percent );
    float y = lerp( ym , yn , percent );
//
    return y;
}
