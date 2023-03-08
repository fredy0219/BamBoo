//
//  EffectCenter.cpp
//  BamBooGod
//
//  Created by Fish on 2020/12/30.
//

#include <iostream>
#include "EffectCenter.h"

EffectCenter::EffectCenter(){
    
    effectMode_U = STILL;
    effectMode_D = STILL;
    
    for(int i = 0 ; i < 108 ; i++)
        if(i < 54)
            motors_degree[i] = 90;
        else
            motors_degree[i] = -90;
}

float* EffectCenter::getMotorsDegree(){
    return motors_degree;
}

void EffectCenter::update(){
    
    sinWave();
    stair();
}

void EffectCenter::sinWave(){
    
    if(effectMode_U == WAVE){
        
        // *** write new wave here
        
        for(int i = 0 ; i < waveEffect_U.id.size() ; i++){
            float wave = ofMap(sin(TWO_PI / waveEffect_U.sin_period * ofGetElapsedTimeMillis() + TWO_PI / waveEffect_U.crest_size * i),
                               -1,1,waveEffect_U.from_degree,waveEffect_U.to_degree);
            motors_degree[waveEffect_U.id[i]-1] = wave;
        }
        
//        for(int i = 0 ; i < 54 ; i++){
//            float wave = ofMap(sin(TWO_PI / waveEffect_U.sin_period * ofGetElapsedTimeMillis() + TWO_PI / waveEffect_U.crest_size * i),
//                               -1,1,waveEffect_U.from_degree,waveEffect_U.to_degree);
//            motors_degree[i] = wave;
//        }
    }
    
    if(effectMode_D == WAVE){
        
        for(int i = 0 ; i < waveEffect_D.id.size() ; i++){
            float wave = ofMap(sin(TWO_PI / waveEffect_D.sin_period * ofGetElapsedTimeMillis() + TWO_PI / waveEffect_D.crest_size * i),
                               -1,1,waveEffect_D.from_degree,waveEffect_D.to_degree);
            motors_degree[waveEffect_D.id[i]+54-1] = -wave;
        }
        
//        for(int i = 0 ; i < 54 ; i++){
//            float wave = ofMap(sin(TWO_PI / waveEffect_D.sin_period * ofGetElapsedTimeMillis() + TWO_PI / waveEffect_D.crest_size * i),
//                               -1,1,waveEffect_D.from_degree,waveEffect_D.to_degree);
//            motors_degree[i+54] = -wave;
//        }
    }

}

void EffectCenter::stair(){
    
    if(stairEffect_U.finished == false && effectMode_U == STAIR){
        for(int i = 0 ; i < stairEffect_U.id.size() ; i++)
            motors_degree[ stairEffect_U.id[i] - 1 ] = stairEffect_U.from_degree + stairEffect_U.step_size * i;
        stairEffect_U.finished = true;
    }
    
    if(stairEffect_D.finished == false && effectMode_D == STAIR){
        for(int i = 0 ; i < stairEffect_D.id.size() ; i++)
            motors_degree[ stairEffect_D.id[i] + 54 -1] = -(stairEffect_D.from_degree + stairEffect_D.step_size * i);
        stairEffect_D.finished = true;
    }
    
    
}



void EffectCenter::setMotorEffectWAVE(int _group_select, vector<uint8_t> id, int _crest_size, float _from_degree, float _to_degree, int _sin_period){
    
    WAVEParameter wp;
    wp.id = id;
    wp.crest_size = _crest_size;
    wp.from_degree = _from_degree;
    wp.to_degree = _to_degree;
    wp.sin_period = _sin_period;
    
    switch(_group_select){
        case 1:
            waveEffect_U = wp;
            effectMode_U = WAVE;
            break;
        case 2:
            waveEffect_D = wp;
            effectMode_D = WAVE;
            break;
        case 3:
            waveEffect_U = wp;
            waveEffect_D = wp;
            
            effectMode_U = WAVE;
            effectMode_D = WAVE;
            break;
    }
}

void EffectCenter::setMotorEffectSTAIR(int _group_select, vector<uint8_t> id, float _from_degree , float _step_size){

    STAIRParameter sp;
    sp.id = id;
    sp.from_degree = _from_degree;
    sp.step_size = _step_size;
    sp.finished = false;
    
    switch(_group_select){
        case 1:
            stairEffect_U = sp;
            effectMode_U = STAIR;
            break;
        case 2:
            stairEffect_D = sp;
            effectMode_D = STAIR;
            break;
        case 3:
            stairEffect_U = sp;
            stairEffect_D = sp;
            effectMode_U = STAIR;
            effectMode_D = STAIR;
            break;
    }
}

