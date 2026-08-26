#include "MainHeader.h"
#include <iostream>
#include <cstdint>
#include <string_view> 
#include <thread>
#include <bitset>
#include <chrono>
#include <algorithm>

template <typename T>
constexpr T constAbs(T x){
    return (x < 0 ? -x : x);
}

namespace check{
    std::string CheckFlag(std::uint8_t flag, std::uint8_t ecuRegister){

        if (flag & ecuRegister){
            return "TRUE";
        }
        else{return "FALSE";}
    }

    char CheckFailure(std::uint8_t flag, std::uint8_t ecuRegister){

        if (flag & ecuRegister){
            return 'X';
        }
        else{return ' ';} 
    }
}

int drivetrainInputFrame(){
    using std::cout;
    using std::cin;

    int selected{};

    while (true)
    {
        cout << "===================VEKTORWERK TELEMETRY===================\n";
        cout << "================DRIVETRAIN MODEL SELECTION================\n";
        cout << '\n';
        cout << "[PLEASE SELECT ONE OF THE FOLLOWING AVAILABLE MODELS]\n";
        cout << "-- BMW E92 335i (N54) [1]\n";
        cout << "-- BMW M50 i4         [2]\n";
        cout << "---------------------------------\n";
        cout << "MAKE A SELECTION: ";
        cin >> selected;

        if(selected == 1){
            break;
        }
        if(selected == 2){
            break;
        }
    }

    return selected;
}

data::input InputFrame(data::input& input){

    using std::cout;
    using std::cin; 

    cout << "===================VEKTORWERK TELEMETRY INPUT FRAME===================\n"; // general vehicle configs
    cout << "Please input the following info:\n";
    
    cout << "Coolant temp (> 70.0 C):   ";
    cin >> input.coolantTemp;

    cout << "Vehicle Health (Preferable above 80%):   ";
    cin >> input.vehicleHealth;

    cout << "Road coefficient (1-0.5 range):   ";
    cin >> input.roadCoefficient;
    
    cout << "Steering angle (~0.0 deg): ";
    cin >> input.steeringAngle;
    
    cout << "Brake safety check (0/1):  ";
    cin >> input.brakecheck;

    cout << "Simulation duration (s):  ";
    cin >> input.tickAmount;

    return input; 
};

constexpr bool approximatelyEqualRel(double a,double b,double epsilon){
    return (constAbs(a-b) <= (std::max(constAbs(a), constAbs(b))* epsilon));
};

bool CheckDirect(double a,double b){
    if (a == b){
        return 1;
    }
    else{
        return 0;
    }
}

std::string FailureCheck(){
    if (data::ECU_FAIL_REGISTER && error::CHANGE_GEAR){
        return "CHANGED GEAR FAILURE [ERROR 1]";
    }
    else if (data::ECU_FAIL_REGISTER && error::SPEED_FAILURE){
        return "UNKNOWN [ERROR 2]";
    }

    return "NO ERROR";
}

void OutputFrame(data::package& data, int tick, data::input& input, std::string_view projectVer, data::drivetrainModel drive){
    using std::cout;

    cout << "===================VEKTORWERK TELEMETRY [TICK #" << tick << "]===================" << '\n';
    cout << "[DRIVETRAIN] RPM: " << data.RPM << " | Calculated gear slot: " << data.gear << " | Shift recommendation: " << data.recommendation << '\n';
    cout << "[CRUISE CONTROL] Target: " << data.targetspeed << " km/h | Sensor: " << data.sensorspeed << " km/h" << '\n';
    cout << "- Direct (==) Check result: " << CheckDirect(data.targetspeed,data.sensorspeed) << " (False negative due to FP precision)" << '\n';
    cout << "- Epsilon (|DIFF| < 1e-4):  " << approximatelyEqualRel(data.targetspeed, data.sensorspeed,0.1) << " (Target speed exceeded)" << '\n';

    cout << '\n';

    cout << "[LAUNCH CONTROL]" << '\n';
    cout << "- Coolant Temp (> 70.0 C):     " << check::CheckFlag(flag::COOLANT_TEMP, data::ECU_REGISTER) << " ("<< input.coolantTemp << " C)" << '\n';
    cout << "- Steering angle (~0.0 deg):   " << check::CheckFlag(flag::STEERING_ANGLE, data::ECU_REGISTER) << " (~ "<< input.steeringAngle << " deg)" << '\n';
    cout << "- Brake safety check:          " << check::CheckFlag(flag::BRAKE_CHECK, data::ECU_REGISTER) << '\n';
    cout << "-> LAUNCH CONTROL STATUS:      " << check::CheckFlag(flag::LAUNCH_CTRL, data::ECU_REGISTER) <<  '\n';

    cout << '\n';
    cout << "[FAILURE]" << '\n';
    cout << "- ENGINE FAULURE: [" << check::CheckFailure(flag::ENGINE_FAILURE, data::ECU_REGISTER) << "]\n";
    cout << "- FAILURE REASON: " << FailureCheck() << '\n';

    if(true){

        cout << '\n';

        cout << "[DEBUG]" << '\n';
        cout << "-- GEAR RATIO: " << data.gearRatio << '\n';
        cout << "-- MODEL ID: " << drive.modelID << '\n';
        cout << "-- MODEL NAME: " << modelName(drive) << '\n';
        cout << "-- TURBO GAIN: " << drive.turboAmount << '\n';
        cout << "-- CHANGED GEAR: [" << data.changedGear << "]\n";
        cout << "--FAILURE CHANCE: " << data.rolledGearshiftFailureChance + data.rolledSpeedFailureChance << "% | BEGINNING VEHICLE HEALTH: " << input.vehicleHealth <<"% | PACKAGE VEHICLE HEALTH: " << data.PackageVehicleHealth << "%" <<'\n'; 
    }

    cout << '\n';
    cout << "PROJECT VERSION: " << projectVer;

};

void EndStatistics(int tick){

    using std::cout;

    for(int i {10}; i >= 0; --i){
        cout << "===================VEKTORWERK TELEMETRY===================" << '\n';
        cout << "======================END STATISTICS======================" << '\n';
        cout << '\n';
        cout << "FRAMES RAN: " << tick-1 << '\n';
        cout << "CLOSING PROGRAM IN: " << i;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        system("cls");
    };
}
