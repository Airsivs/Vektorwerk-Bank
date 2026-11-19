#include "AudioHeader.h"
#include <iostream>
#include <cstdint>
#include <string_view> 
#include <thread>
#include <bitset>
#include <chrono>
#include <algorithm>
#include <cstdlib> 
#include <limits>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace backend{

    DATA::AccountInfo ConvertData(DATA::AccountInfo& AI, DATA::AccountCreation& accCr){
        AI.actualName = accCr.accName;
        AI.CurrentCash = accCr.cash;
        AI.CurrentcreditScore = accCr.creditScore;
        accCr.ConversionCompleted = 1;
        return AI;
    };

    void ShowWindowAPIcall(DATA::AccountInfo& AI, int accountNum, int AppW, int AppH){

        switch(AI.APIcall){
            case 1:
                frontend::TransferInterface(accountNum, AI, AppW, AppH,AI.showTransferWindow);
                break;
            case 2:
                frontend::RequestInterface(accountNum, AI, AppW, AppH,AI.showRequestWindow);
                break;
            case 3:
                frontend::SendMessageInterface(accountNum, AI, AppW, AppH,AI.showMessageWindow);
                break;
            default:
                break;
        }
    }
    
    std::string FetchName(DATA::AccountInfo& AI){
        return AI.actualName;
    };

    DATA::AccountInfo Transfer(std::string InitiatorName,DATA::AccountInfo& Recipient,int amount){
        for(int i = 0; i < 5; i++){
            if(InitiatorName == FetchName(AccData[i])){
                if(AccData[i].CurrentCash >= amount){
                    Recipient.CurrentCash += amount;
                    AccData[i].CurrentCash -= amount;
                }else{
                    Recipient.CurrentCash = 6767;
                }
                break;
            }
        }
        return Recipient;
    }

    DATA::AccountInfo Request(std::string InitiatorName,DATA::AccountInfo& Recipient,int amount){
        for(int i = 0; i < 5; i++){
            if(InitiatorName == FetchName(AccData[i])){
                if(Recipient.CurrentCash >= amount){
                    Recipient.CurrentCash -= amount;
                    AccData[i].CurrentCash += amount;
                }else{
                    Recipient.CurrentCash = 6767;
                }
                break;
            }
        }
        return Recipient;
    }

    int FindAccount(std::string name, std::string InitiatorName, int amount, int type){

        if(amount <= 0){
            return 2;
        }

        if(name == InitiatorName){
            return 1;
        }

        int temp;

        for(int i = 0; i < 5; i++){
            if(name == FetchName(AccData[i])){
                temp = AccData[i].CurrentCash;
                if(type == 1){
                    Transfer(InitiatorName,AccData[i],amount);
                }else if(type == 2){
                    Request(InitiatorName,AccData[i],amount);
                }

                if(AccData[i].CurrentCash == 6767){
                    AccData[i].CurrentCash = temp;
                    return 2; // not enough cash
                }
                return 0;
            }
        }
        return 1; // not found
    }

    std::string FetchName(DATA::AccountCreation& AC){
        return AC.accName;
    };

    int CheckForExistingUsername(std::string InitiatorName, DATA::AccountCreation& AC){
        if(InitiatorName == backend::FetchName(AC)){
            return 0;
        }else{return 1;}
    }

    int SendMessage(std::string InitiatorName,std::string Recipient, std::string Title, std::string Text){
        bool found{false};
        int i = 10;

        if(Recipient == "" || Title == ""){
            if(Text == ""){
                return 1;
            }
            return 1;
        }

        if(InitiatorName == Recipient){
            return 2;
        }

        for(int i = 0; i < 5; i++){
            if(Recipient == FetchName(AccData[i])){
                std::cout << "User found";
                found = true;
                break;
            };
        };

        Notification[i] = {Title,Text,0,Recipient, InitiatorName, i, true};

        if(!found){
            return 3;
        }

        return 0;
    };

    int CreateTransferNotification(int NotifID, int amount, std::string Initiator, std::string Recipient){
        std::string Title{"TRANSFER"};
        std::string Text = "You have received " + std::to_string(amount) + " from " + Initiator + "!";
        Notification[NotifID] = {Title,Text,0,Recipient, Initiator, NotifID, true};

        return 0;
    };

    int LogificationValidation(const DATA::log& log){
        if (!log.active || log.text.empty()) { 
            return 0;
        }
        return 1;
    }   

}
