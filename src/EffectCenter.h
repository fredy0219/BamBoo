//
//  EffectCenter.hpp
//  BamBooGod
//
//  Created by Fish on 2020/12/30.
//

#ifndef EffectCenter_h
#define EffectCenter_h

#include "ofMain.h"
#include <stdio.h>
#include "Utils.h"

using namespace std;

struct WAVEParameter{
    vector<uint8_t> id;
    int crest_size = 0;
    float from_degree = 0;
    float to_degree = 0;
    int sin_period;
};

struct STAIRParameter{
    vector<uint8_t> id;
    float from_degree = 0;
    float step_size = 0;
    bool finished = false;
};



class EffectCenter{
    
public:
    EffectCenter();
    void update();
    
    void sinWave();
    void stair();
    
    float* getMotorsDegree();
    
    int effectMode_U;
    int effectMode_D;
    float motors_degree[108];
    
    WAVEParameter waveEffect_U;
    WAVEParameter waveEffect_D;
    
    STAIRParameter stairEffect_U;
    STAIRParameter stairEffect_D;
    
    void setMotorEffectWAVE(int group_select, vector<uint8_t> id, int _crest_size, float _from_degree, float _to_degree, int _sin_period);
    
    void setMotorEffectSTAIR(int group_select, vector<uint8_t> id, float _from_degree , float _step_size);
    
};


#endif /* EffectCenter_h */
