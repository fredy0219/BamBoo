//
//  UDPCenter.cpp
//  BamBooGod
//
//  Created by Fish on 2020/12/30.
//

#include "UDPCenter.h"
//using namespace std;

void UDPCenter::setup()
{

    udp_start_time = ofGetElapsedTimeMillis();
    
    // QLab Port
    udp_cue.Create();
    udp_cue.Bind(udp_qlab_recv_port); // for listening QLab
    udp_cue.SetNonBlocking(true);
    
    // Motor Boss Port
    udp_motor_boss.Create();
    udp_motor_boss.Bind(udp_motor_boss_recv_port); // for listening Motor Boss
    udp_motor_boss.Connect(udp_motor_boss_ip.c_str(), udp_motor_boss_send_port); // for sending to Motor Boss
    udp_motor_boss.SetNonBlocking(true);
    
    udp_motor_boss_plc.Create();
    udp_motor_boss_plc.Bind(udp_motor_boss_recv_port_plc);
    udp_motor_boss_plc.Connect(udp_motor_boss_ip.c_str(), udp_motor_boss_send_port_plc); // for sending to Motor Boss PLC

    // Light Boss Port

    udp_light_boss.Create();
    udp_light_boss.Bind(udp_light_boss_recv_port); // for listening Light Boss
    udp_light_boss.Connect(udp_light_boss_ip.c_str(), udp_light_boss_send_port); // for sending to Light Boss
    udp_light_boss.SetNonBlocking(true);
    
    udp_light_boss_pixels.Create();
    udp_light_boss_pixels.SetSendBufferSize(65535);
    udp_light_boss_pixels.Connect(udp_light_boss_ip.c_str(), udp_light_boss_send_port_pixels);
//    udp_light_boss_pixels.Connect("127.0.0.1", udp_light_boss_send_port_pixels);
    udp_light_boss_pixels.SetNonBlocking(true);
    
    udp_video_boss.Create();
    udp_video_boss.Connect(udp_video_boss_ip.c_str(), udp_video_boss_send_port);
    
}

void UDPCenter::update()
{
    char udp_message[100];
    int message_length = udp_cue.Receive(udp_message, 100);

    //put message into cue_queue
    if (message_length > 0){
        string s(udp_message);
        cue_queue.push(s);
    }
    
    // for motor boss
    char udp_motor_boss_message[10000];
    string addr;
    int port;
    message_length = udp_motor_boss.ReceiveAndGetIP(udp_motor_boss_message, 10000 , addr, port);
    if (message_length > 0){
        
//        cout<<"Get message from : " << addr <<endl;
        
        if(udp_motor_boss_ip.compare(addr) != 0){
            cout<<"Change MotorBoss IP to " << addr <<endl;
            udp_motor_boss_ip = addr;
            udp_motor_boss_send_port = port;
            udp_motor_boss.Connect( udp_motor_boss_ip.c_str() , udp_motor_boss_send_port);
        }
        
        isMotorBossRx = true;
        string s(udp_motor_boss_message);
        string stringJson = s;
        Json::Reader reader;
        reader.parse(stringJson, motor_boss_json);
    }else{
        isMotorBossRx = false;
    }
    
    char udp_light_boss_message[1000];
    message_length = udp_light_boss.ReceiveAndGetIP(udp_light_boss_message, 1000, addr, port);
    
    if(message_length > 0){
        
        if(udp_light_boss_ip.compare(addr) != 0){
            cout<<"Change LightBoss IP to " << addr <<endl;
            udp_light_boss_ip = addr;
            udp_light_boss.Connect(udp_light_boss_ip.c_str(), udp_light_boss_send_port);
            udp_light_boss_pixels.Connect(udp_light_boss_ip.c_str(), udp_light_boss_send_port_pixels);
        }
        
        isLightBossRx = true;
    }else{
        isLightBossRx = false;
    }
    
    
}

void UDPCenter::sendMotorBossStatus(vector<float> degrees, bool _isMotorOn, bool isMotorRebootTrigger){
    
    if(ofGetElapsedTimeMillis() - current_motor_boss_send_timer > 1000/40){
        Json::Value motors_json_message;
        Json::Value motors_degree;
        
        for(int i = degrees.size()/2 ; i < degrees.size() ; i++)
            motors_degree.append(roundf(degrees[i]*1000)/1000);
        
        for(int i = 0 ; i < degrees.size()/2 ; i++)
            motors_degree.append(roundf(degrees[i]*1000)/1000);
        
        motors_json_message["ALL_M_DEG"] = motors_degree;
        motors_json_message["E_STOP"] = !_isMotorOn;
        motors_json_message["SERVO_ON"] = _isMotorOn;
        motors_json_message["TS"] = ofGetElapsedTimeMillis() - udp_start_time;
        
        Json::FastWriter writer;
        string toMotorBossMessage = writer.write(motors_json_message);
        
        udp_motor_boss.Send(toMotorBossMessage.c_str(), toMotorBossMessage.size());
        isMotorBossTx = true;
        current_motor_boss_send_timer = ofGetElapsedTimeMillis();
    }else{
        isMotorBossTx = false;
    }
    
}
void UDPCenter::sendMotorBossPLCMessage(string _cue_string){
    udp_motor_boss.Send(_cue_string.c_str(), _cue_string.size());
}
//void UDPCenter::sendMotorBossPLCStatus(bool _isPLCOn){
//
//    Json::Value motor_plc_message;
//    motor_plc_message["PLC"] = _isPLCOn;
//
//    Json::FastWriter writer;
//    string toMotorBossPLCMessage = writer.write(motor_plc_message);
//    udp_motor_boss_plc.Send(toMotorBossPLCMessage.c_str(), toMotorBossPLCMessage.size());
//}

//void UDPCenter::sendLightBossMessage(){
//
//}

void UDPCenter::sendLightBossMARDIXMessage(string _cue_string){
    
    udp_light_boss.Send(_cue_string.c_str(), _cue_string.size());

}

void UDPCenter::sendLightBossPixels(unsigned char * pixelData){
    
//    memcpy(pixels, pixelData, 30057);
    if(ofGetElapsedTimeMillis() - current_light_boss_send_timer> 1000/40){
        
        isLightBossTx = true;
        udp_light_boss_pixels.Send((char *)pixelData, 38480);
        current_light_boss_send_timer = ofGetElapsedTimeMillis();
    }else{
        isLightBossTx = false;
    }
    
}

void UDPCenter::sendVideoBossMessage(string _cue_string){
    udp_video_boss.Send(_cue_string.c_str(), _cue_string.size());
}

void UDPCenter::popCueMessage(string &current_cue_message)
{

    if (cue_queue.size() > 0)
    {
        current_cue_message = cue_queue.front();
        cue_queue.pop();
    }
    else
        current_cue_message = "";
}

void UDPCenter::exit()
{
    udp_cue.Close();
    udp_motor_boss.Close();
    udp_motor_boss_plc.Close();
    udp_light_boss.Close();
}
