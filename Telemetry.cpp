#include "MainHeader.h"
#include <iostream>
#include <cstdint>
#include <string_view> 
#include <thread>
#include <bitset>
#include <chrono>
#include <iomanip>
#include <random>

namespace{
    constexpr int increaseG1{1000};
    constexpr int increaseG2{600};
    constexpr int increaseG3{300};
    constexpr int increaseG4{200};
    constexpr int increaseG5{150};
    constexpr int turboGain{180};
    //---------------------------
    constexpr double gearRatioG1{4.06};
    constexpr double gearRatioG2{2.40};
    constexpr double gearRatioG3{1.58};
    constexpr double gearRatioG4{1.19};
    constexpr double gearRatioG5{0.9};
}

namespace modelData{

    namespace bmw_335i{
        constexpr double ratioG1{4.06};
        constexpr double ratioG2{2.40};
        constexpr double ratioG3{1.58};
        constexpr double ratioG4{1.19};
        constexpr double ratioG5{0.9};
        // -------------------------
        constexpr int increaseG1{1000};
        constexpr int increaseG2{600};
        constexpr int increaseG3{300};
        constexpr int increaseG4{200};
        constexpr int increaseG5{150};
        constexpr int turboGain{180};
        // -------------------------
        bool turbo = true;
        constexpr double tireCircumference{200}; //cm
        constexpr double finalDriveRatio{3.08};
        constexpr int ModelID{1};
    }



    namespace bmw_M50i4{
        constexpr double ratioG1{4.06};
        constexpr double ratioG2{2.40};
        constexpr double ratioG3{1.58};
        constexpr double ratioG4{1.19};
        constexpr double ratioG5{0.9};
        // -------------------------
        constexpr int increaseG1{1000};
        constexpr int increaseG2{3000}; // 600
        constexpr int increaseG3{300};
        constexpr int increaseG4{200};
        constexpr int increaseG5{150};
        constexpr int turboGain{180};
        // -------------------------
        bool turbo = true;
        constexpr double tireCircumference{200}; //cm
        constexpr double finalDriveRatio{3.08};
        constexpr int ModelID{2};
    }
}

template <typename T>
constexpr T constAbs(T x){
    return (x < 0 ? -x : x);
}

constexpr bool approximatelyEqualRel(double a,double b,double epsilon){
    return (constAbs(a-b) <= (std::max(constAbs(a), constAbs(b))* epsilon));
};

data::drivetrainModel ModelSelectionCheck(int selected,data::drivetrainModel& drive){
    switch(selected){
        case 1:
            drive.gain1 = modelData::bmw_335i::increaseG1;
            drive.gain2 = modelData::bmw_335i::increaseG2;
            drive.gain3 = modelData::bmw_335i::increaseG3;
            drive.gain4 = modelData::bmw_335i::increaseG4;
            drive.gain5 = modelData::bmw_335i::increaseG5;
            //---------------------------------------
            drive.gearRatio1 = modelData::bmw_335i::ratioG1;
            drive.gearRatio2 = modelData::bmw_335i::ratioG2;
            drive.gearRatio3 = modelData::bmw_335i::ratioG3;
            drive.gearRatio4 = modelData::bmw_335i::ratioG4;
            drive.gearRatio5 = modelData::bmw_335i::ratioG5;
            //---------------------------------------
            drive.finadriveRatio = modelData::bmw_335i::finalDriveRatio;
            drive.tireCircumference = modelData::bmw_335i::tireCircumference;
            drive.turbo = modelData::bmw_335i::turbo;
            drive.turboAmount = modelData::bmw_335i::turboGain;
            drive.modelID = modelData::bmw_335i::ModelID;
            break;

        case 2:
            drive.gain1 = modelData::bmw_M50i4::increaseG1;
            drive.gain2 = modelData::bmw_M50i4::increaseG2;
            drive.gain3 = modelData::bmw_M50i4::increaseG3;
            drive.gain4 = modelData::bmw_M50i4::increaseG4;
            drive.gain5 = modelData::bmw_M50i4::increaseG5;
            //---------------------------------------
            drive.gearRatio1 = modelData::bmw_M50i4::ratioG1;
            drive.gearRatio2 = modelData::bmw_M50i4::ratioG2;
            drive.gearRatio3 = modelData::bmw_M50i4::ratioG3;
            drive.gearRatio4 = modelData::bmw_M50i4::ratioG4;
            drive.gearRatio5 = modelData::bmw_M50i4::ratioG5;
            //---------------------------------------
            drive.finadriveRatio = modelData::bmw_M50i4::finalDriveRatio;
            drive.tireCircumference = modelData::bmw_M50i4::tireCircumference;
            drive.turbo = modelData::bmw_M50i4::turbo;
            drive.turboAmount = modelData::bmw_M50i4::turboGain;
            drive.modelID = modelData::bmw_M50i4::ModelID;
            break;
    }

    return drive;
    
}

std::string modelName(data::drivetrainModel drive){
    switch(drive.modelID){
        case 1:
            return "BMW 335i"; 
        case 2:
            return "BMW M50 i4";
        default: return "ERROR";
    }
};

data::input FlagCheck(data::input& convert, data::drivetrainModel& drive, std::string_view VehicleName){

    if(convert.coolantTemp>70){
        data::ECU_REGISTER |= flag::COOLANT_TEMP;
        std::string coolant = "BELOW 70 C";
    }

    if(approximatelyEqualRel(convert.steeringAngle,0.0,0.1)){
        data::ECU_REGISTER |= flag::STEERING_ANGLE;
    }

    if(convert.brakecheck==1){
        data::ECU_REGISTER |= flag::BRAKE_CHECK;
    }

    if (flag::COOLANT_TEMP | flag::STEERING_ANGLE | flag::BRAKE_CHECK){
        data::ECU_REGISTER |= flag::LAUNCH_CTRL;
    }

    system("cls");

    using std::cout;
    cout << "================VEKTORWERK TELEMETRY================" << '\n';
    cout << "==========EVALUATION OF SIMULATION SETTINGS=========" << '\n';
    cout << '\n';
    cout << "[COOLANT TEMP]:   " << convert.coolantTemp << " C" << '\n';
    cout << "[STEERING ANGLE]: " << convert.steeringAngle << " deg" << '\n';
    cout << "[BRAKE SAFETY]:   " << std::boolalpha << convert.brakecheck << '\n';
    cout << "[ROAD COEFFICIENT]: " << convert.roadCoefficient << '\n';
    cout << "[VEHICLE HEALTH]: " << convert.vehicleHealth << '\n';
    cout << '\n';

    cout << "[SELECTED VEHICLE TYPE]: " << VehicleName << '\n';

    cout << '\n';
    constexpr std::uint8_t requiredFlags = flag::COOLANT_TEMP | flag::STEERING_ANGLE | flag::BRAKE_CHECK;
    if ((data::ECU_REGISTER & requiredFlags) == requiredFlags) {
        data::ECU_REGISTER |= flag::LAUNCH_CTRL;
        cout << "[LAUNCH CONTROL]: READY - STARTING SIMULATION!";
    }                                                               
    else{
        cout << "[LAUNCH CONTROL]: NOT READY - CANCELLING SIMULATION!"; 
    }

    cout << '\n';
    cout << "-----------DEBUG-----------\n";
    cout << "[MODEL ID] " << drive.modelID;

    std::this_thread::sleep_for(std::chrono::seconds(5));


    return convert;

}

int SpeedFailureChance(const data::input& input) {

        if (input.roadCoefficient >= 0.70){
            if (input.vehicleHealth >= 80)      { return 2; }
            if (input.vehicleHealth >= 60)      { return 3; }
            if (input.vehicleHealth >= 40)      { return 7; }
            if (input.vehicleHealth >= 20)      { return 15; }
        }
        else {
            if (input.vehicleHealth >= 80)      { return 2; }
            if (input.vehicleHealth >= 60)      { return 5; }
            if (input.vehicleHealth >= 40)      { return 7; }
            if (input.vehicleHealth >= 20)      { return 15; }
        }

        return 50;
}

int GearshiftFailureChance(const data::input& input) {

        if (input.vehicleHealth >= 80)      { return 2; }
        if (input.vehicleHealth >= 60)      { return 5; }
        if (input.vehicleHealth >= 40)      { return 7; }
        if (input.vehicleHealth >= 20)      { return 15; }
        return 50; 

}

data::package AssignHealth(data::package& data, data::input input){
    data.PackageVehicleHealth = input.vehicleHealth;
    return data;
}

data::package Drivetrain(data::package& data, data::drivetrainModel drive, data::input input){

    int realRPM;
    data.changedGear = ' ';
    data.rolledGearshiftFailureChance = GearshiftFailureChance(input);
    double activeGearRatio;

    if(!(data::ECU_REGISTER & flag::ENGINE_FAILURE)){

    switch(data.gear){
        case 1:
            realRPM = data.RPM + drive.gain1;
            data.gearRatio = drive.gearRatio1;
            break;
        case 2:
            realRPM = data.RPM + drive.gain2;
            data.gearRatio = drive.gearRatio2;
            break;
        case 3:
            realRPM = data.RPM + drive.gain3;
            data.gearRatio = drive.gearRatio3;
            break;
        case 4:
            realRPM = data.RPM + drive.gain4;
            data.gearRatio = drive.gearRatio4;
            break;
        default:
            realRPM = data.RPM + drive.gain5;
            data.gearRatio = drive.gearRatio5;
            break;
    }

    if (drive.turbo == true){
        realRPM += drive.turboAmount;
    }

}

    data.RPM = realRPM;

    if (data.RPM < 6000 && data.RPM > 2000){
        data.recommendation = "OPTIMAL";
    }
    if (data.RPM > 6000){
        data.recommendation = "UP-SHIFT";
    }
    if (data.RPM < 2000){
        data.recommendation = "DOWNSHIFT";
    }

    if (data.RPM > 7000 && data.gear <=6){
        data.RPM = 2400;
        ++data.gear;

        data.changedGear = 'X';

        static std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<int> roll(1, 100);

        if (roll(mt) <= GearshiftFailureChance(input)){
            data::ECU_REGISTER |= flag::ENGINE_FAILURE;
        }
    }

    static std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<int> roll(1, 100);

    if (roll(mt) <= SpeedFailureChance(input)){
        data::ECU_REGISTER |= flag::ENGINE_FAILURE;
    }

    if (data::ECU_REGISTER & flag::ENGINE_FAILURE){
        data.RPM -= 500;
        data.PackageVehicleHealth -= 20;
    }

    double totalGearRatio = data.gearRatio * drive.finadriveRatio;

    if(totalGearRatio > 0.0){
        double wheelRPM = data.RPM / totalGearRatio;
        data.sensorspeed = wheelRPM*drive.tireCircumference*0.0006;
    }
    else{
        data.sensorspeed = 0.0;
    }

    data.targetspeed = static_cast<int>(data.sensorspeed)+1;

    return data;



}


