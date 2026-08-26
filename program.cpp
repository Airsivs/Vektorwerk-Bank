#include "MainHeader.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdint>

static int Tick{0};

int main(){

    std::string projectVer{"0.1.3"};

    data::input myInput{};
    data::package DrivetrainPackage{};
    data::drivetrainModel DrivetrainProgram{};
    ModelSelectionCheck(drivetrainInputFrame(),DrivetrainProgram);
    myInput = InputFrame(myInput);
    DrivetrainPackage.gear = 1;
    system("cls");
    FlagCheck(myInput,DrivetrainProgram,modelName(DrivetrainProgram));
    AssignHealth(DrivetrainPackage, myInput);
    for (; Tick < myInput.tickAmount+1; Tick++){
        Drivetrain(DrivetrainPackage,DrivetrainProgram,myInput);
        OutputFrame(DrivetrainPackage, Tick, myInput,projectVer,DrivetrainProgram);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        system("cls");
    }

    EndStatistics(Tick);

    return 0;
    
}