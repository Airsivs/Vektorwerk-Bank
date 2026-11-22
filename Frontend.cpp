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
#include <ctime>
#include <iomanip>

using ID = std::string;
using text = char;

std::string timeLogF() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%H:%M:%S"); 
    return oss.str();
}

namespace frontend{

    static bool Notif[49]{};

int AmountOfAccounts(int width, int height, int AppWidth, int AppHeight){
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;

    static int tempValue{1};
    static int AOA{0}; // amount of accounts int value
    static bool isOpen{1};

    if(isOpen == 0){
        return AOA;
    }

    float xPos = (AppWidth - width) / 2.0f;
    float yPos = (AppHeight - height) / 2.0f;

    ImGui::SetNextWindowPos(ImVec2(xPos, yPos));
    ImGui::SetNextWindowSize(ImVec2(width, height));

    ImGui::Begin("Amount of Banking Accounts", nullptr, windowFlags);
    ImGui::Text("Choose your desired amount:");
    ImGui::SliderInt("##AmountSlider", &tempValue, 1, 5);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    if(ImGui::Button("Confirm", ImVec2(-1,0))){
        AOA = tempValue;
        isOpen = 0;
    }
    ImGui::End();

    return AOA;
}

DATA::AccountCreation AccountInfo(int AccNumber, DATA::AccountCreation& accCr, int AppWidth, int AppHeight){
    std::array<int,2> AIwindowSize = {250,300}; // AI - Account Info

    static int lastAccNumber{-1};
    static bool IsOpen{true};
    static char tempName[128] = "";
    static int tempCash{999};
    static int tempCreditScore{999};

    if (lastAccNumber != AccNumber) {
        IsOpen = true;
        lastAccNumber = AccNumber;
        snprintf(tempName, sizeof(tempName), "");
        tempCash = 999;
        tempCreditScore = 1;
    }

    if(IsOpen == false){
        return accCr;
    }

    ImGuiWindowFlags CreationWindow_flags = 0;
    CreationWindow_flags |= ImGuiWindowFlags_NoCollapse;
    CreationWindow_flags |= ImGuiWindowFlags_NoMove;
    CreationWindow_flags |= ImGuiWindowFlags_NoResize;

    float xPos = (AppWidth - AIwindowSize[0]) / 2.0f;
    float yPos = (AppHeight - AIwindowSize[1]) / 2.0f;

    ImGui::SetNextWindowPos(ImVec2(xPos, yPos));
    ImGui::SetNextWindowSize(ImVec2(AIwindowSize[0], AIwindowSize[1]));

    std::string windowTitle = "Account Creation #" + std::to_string(AccNumber);
    ImGui::Begin(windowTitle.c_str(), nullptr, CreationWindow_flags);
    ImGui::Text("Account number: %d", AccNumber);

    ImGui::InputText("Username##USERName", tempName, sizeof(tempName));
    ImGui::InputInt("Cash##CASH", &tempCash);
    ImGui::InputInt("Credit Score##CreditScore", &tempCreditScore);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if(ImGui::Button("Confirm", ImVec2(-1,0))){

        for(int i = 0; i < 5; i++){
            int temp = backend::CheckForExistingUsername(tempName,AccInitData[i]);
            if(temp == 0){
                break;
            }
        }

        accCr.accName = tempName;
        accCr.cash = tempCash;
        accCr.creditScore = tempCreditScore;
        accCr.isCompleted = 1; 
        IsOpen = false;
    }
    ImGui::End();

    return accCr;
}

DATA::notif NotificationWindow(DATA::notif& notif, bool isOpen){

    if(!isOpen){
        return notif;
    }

    ImGuiWindowFlags notifWin_flags = 0;
    notifWin_flags |= ImGuiWindowFlags_NoCollapse;
    notifWin_flags |= ImGuiWindowFlags_NoResize;


    using text = std::string;

    text windowNLTitle = "Notification Window ###" + notif.title;
    text TextInNotif = "Text: " + notif.text;
    text Sender = "From: " + notif.sender;
    text Receiver = "Receiver: " + notif.receiver;

    ImGui::SetNextWindowSize(ImVec2(300,200));
    ImGui::Begin(windowNLTitle.c_str(), nullptr, notifWin_flags);

    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("%s", notif.title.c_str());
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::SetWindowFontScale(0.75f);
    ImGui::Text("%s", TextInNotif.c_str());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("%s", Receiver.c_str());
    ImGui::Text("%s", Sender.c_str());

    if(ImGui::Button("CLOSE NOTIFICATION", ImVec2(-FLT_MIN, 25.0f))){
        frontend::Notif[notif.ID] = false;
        notif.active = false;
    }

    ImGui::End();

    return notif;
}

int NotificationValidation(const DATA::notif& notif, DATA::AccountInfo& AI) {
    if (!notif.active || notif.receiver != AI.actualName) { 
        return 0;
    }
    return 1;
}

void DisplayNotif(DATA::notif& notif){
    std::string NotifTitle = "Notification - " + notif.title;
    if(ImGui::Button(NotifTitle.c_str(), ImVec2(-FLT_MIN, 25.0f))){
        frontend::Notif[notif.ID] = true;
    };
    NotificationWindow(notif, frontend::Notif[notif.ID]);
}

void notifList(DATA::AccountInfo& AI){
    if (ImGui::BeginChild("ScrollingList", ImVec2(0.0f, 100.0f), ImGuiChildFlags_Borders)){
    for (int i = 0; i < 50; i++)
    {
        if(NotificationValidation(Notification[i], AI)==1){
            DisplayNotif(Notification[i]);
        };
    }
    ImGui::EndChild(); 
    }
};

DATA::AccountInfo AccountInterface(int AccountNumber, DATA::AccountInfo& AI, int AppWidth, int AppHeight, bool isOpen){

    if(isOpen == false){
        return AI;
    }

    ImGuiWindowFlags AccountInfo_flags = 0;
    AccountInfo_flags |= ImGuiWindowFlags_NoCollapse;
    AccountInfo_flags |= ImGuiWindowFlags_NoResize;
    AccountInfo_flags |= ImGuiWindowFlags_NoScrollbar;

    std::array<int,2> AIwindowSize = {270,300};
    float xPos = (AppWidth - AIwindowSize[0]) / 2.0f;
    float yPos = (AppHeight - AIwindowSize[1]) / 2.0f;

    ImGui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(AIwindowSize[0], AIwindowSize[1]));

    std::string windowTitle = "Account Interface #" + std::to_string(AccountNumber);

    ImGui::Begin(windowTitle.c_str(), nullptr, AccountInfo_flags);
    ImGui::Text("Account name: %s", AI.actualName.c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Cash amount: %d", AI.CurrentCash);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Credit score: %d", AI.CurrentcreditScore);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::SetNextItemWidth(-1.0f);
    if(ImGui::Button("TRANSFER MONEY TO ANOTHER ACCOUNT")){
        AI.APIcall = 1;
        AI.showTransferWindow = true;
        AI.showRequestWindow = false;
        AI.showMessageWindow = false;
    }
    ImGui::SetNextItemWidth(-1.0f);
    if(ImGui::Button("REQUEST MONEY FROM ANOTHER ACCOUNT")){
        AI.APIcall = 2;
        AI.showTransferWindow = false;
        AI.showRequestWindow = true;
        AI.showMessageWindow = false;
    }
    ImGui::SetNextItemWidth(-1.0f);
    if(ImGui::Button("SEND MESSAGE")){
        AI.APIcall = 3;
        AI.showTransferWindow = false;
        AI.showRequestWindow = false;
        AI.showMessageWindow = true;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "INBOX");

    notifList(AI);
    
    ImGui::End();

    return AI;
};

void SendMessageInterface(int InitialAccountNumber, DATA::AccountInfo& AI, int AppWidth, int AppHeight, bool& isOpen){
    if(isOpen == false){
        return;
    }

    static char Recipient[64] = "";
    static char Title[64] = "";
    static char Text[128] = "";

    static int SMIerrorLog{0};
    static bool ShowSMI_Error1{false};
    static bool ShowSMI_Error2{false};
    static bool ShowSMI_Error3{false};

    ImGuiWindowFlags SMI_flag = 0;
    SMI_flag |= ImGuiWindowFlags_NoCollapse;
    SMI_flag |= ImGuiWindowFlags_NoResize;
    SMI_flag |= ImGuiWindowFlags_NoScrollbar;

    std::array<int,2> AIwindowSize = {250,355};
    float xPos = (AppWidth - AIwindowSize[0]) / 2.0f;
    float yPos = (AppHeight - AIwindowSize[1]) / 2.0f;
    ImGui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(AIwindowSize[0], AIwindowSize[1]));

    ID SMI_Title = "Send message: " + AI.actualName + "###SMI_" + std::to_string(reinterpret_cast<uintptr_t>(&AI));
    ID SMI_TitleID = "###TitleID_" + std::to_string(reinterpret_cast<uintptr_t>(&AI));
    ID SMI_RecipientID = "###RecipientID_" + std::to_string(reinterpret_cast<uintptr_t>(&AI));
    ID SMI_TextID = "###TextID_" + std::to_string(reinterpret_cast<uintptr_t>(&AI));

    ImGui::Begin(SMI_Title.c_str(), nullptr, SMI_flag);
    ImGui::Text("MESSAGE RECIPIENT:");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText(SMI_RecipientID.c_str(), Recipient, IM_ARRAYSIZE(Recipient));
    ImGui::Text("MESSAGE TITLE:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText(SMI_TitleID.c_str(), Title, IM_ARRAYSIZE(Title));
    ImGui::Text("MESSAGE TEXT:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputTextMultiline(SMI_TextID.c_str(), Text, IM_ARRAYSIZE(Text));

    if(ShowSMI_Error1 == true || ShowSMI_Error2 == true){
        AIwindowSize[1] = 375;
    }

    if(ShowSMI_Error1 == true){
        ImGui::Spacing();
        ImGui::SetWindowFontScale(0.85f);
        ImGui::Text("ERROR: PLEASE INPUT TEXT INTO ALL BOXES!");
        ImGui::SetWindowFontScale(1.0f);
    }

    if(ShowSMI_Error2 == true){
        ImGui::Spacing();
        ImGui::SetWindowFontScale(0.85f);
        ImGui::Text("ERROR: PLEASE INPUT A VALID USER!");
        ImGui::SetWindowFontScale(1.0f);
    }

    if(ShowSMI_Error3 == true){
        AIwindowSize[1] = 375;
        ImGui::Spacing();
        ImGui::SetWindowFontScale(0.85f);
        ImGui::Text("ERROR: NO MATCHING USER FOUND!");
        ImGui::SetWindowFontScale(1.0f);
    }

    if(ImGui::Button("SEND")){
        SMIerrorLog = backend::SendMessage(AI.actualName,Recipient,Title,Text);
        if(SMIerrorLog == 0){
            std::string LogifText = "SENT MESSAGE (" + AI.actualName + " - " + Recipient + ")";
            Logification[LogifNumber] = {"STANDARD", timeLogF().c_str(), LogifText.c_str(), LogifNumber, true};
            LogifNumber += 1;
        }
    }

    ImGui::SameLine();
    if(ImGui::Button("CANCEL")){
        memset(Recipient, 0, sizeof(Recipient));
        memset(Title, 0, sizeof(Title));
        memset(Text, 0, sizeof(Text));
        SMIerrorLog = 0;
        ImGui::SetWindowFontScale(1.0f);
        isOpen = false;
    };

    switch(SMIerrorLog){
        case 0:
            ShowSMI_Error1 = false;
            ShowSMI_Error2 = false;
            ShowSMI_Error3 = false;
            break;
        case 1:
            ShowSMI_Error1 = true;
            ShowSMI_Error2 = false;
            ShowSMI_Error3 = false;
            break;
        case 2:
            ShowSMI_Error1 = false;
            ShowSMI_Error2 = true;
            ShowSMI_Error3 = false;
            break;
        case 3:
            ShowSMI_Error1 = false;
            ShowSMI_Error2 = false;
            ShowSMI_Error3 = true;
            break;
    }

    ImGui::End();
}

void TransferInterface(int InitialAccountNumber, DATA::AccountInfo& AI, int AppWidth, int AppHeight, bool& isOpen){

    if(isOpen == false){
        return;             // change when changing function data type!!!! ########
    }



    ImGuiWindowFlags TI_flags = 0;
    TI_flags |= ImGuiWindowFlags_NoCollapse;
    TI_flags |= ImGuiWindowFlags_NoResize;
    TI_flags |= ImGuiWindowFlags_NoScrollbar;

    static char Buffer[64] = "";
    static int cashTransfer = 0;

    static int ErrorLog {0};
    static int NotifErrorLog {0};
    static bool NotifError1{false};
    static bool ShowError1{false};
    static bool ShowError2{false};

    std::array<int,2> AIwindowSize = {250,300};
    float xPos = (AppWidth - AIwindowSize[0]) / 2.0f;
    float yPos = (AppHeight - AIwindowSize[1]) / 2.0f;

    ImGui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(AIwindowSize[0], AIwindowSize[1]));

    ID windowTitle = "Transfer: " + AI.actualName + "###TransferInterface_" + std::to_string(reinterpret_cast<uintptr_t>(&AI));
    
    ImGui::Begin(windowTitle.c_str(), nullptr, TI_flags);
    ImGui::Text("TRANSFERRING MONEY TO:");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(-1.0f);
    ID inputId = "##TransferInput_" + std::to_string(reinterpret_cast<uintptr_t>(&AI));
    ImGui::InputText(inputId.c_str(), Buffer, IM_ARRAYSIZE(Buffer));
    ImGui::Text("TRANSFER AMOUNT:");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(-1.0f);
    ID AmountinputId = "##TransferCashInput_" + std::to_string(reinterpret_cast<uintptr_t>(&AI.APIcall));
    ImGui::InputInt(AmountinputId.c_str(), &cashTransfer);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    if(ImGui::Button("CONFIRM")){
        std::cout << Buffer;
        std::cout << cashTransfer;
        ErrorLog = backend::FindAccount(Buffer,AI.actualName,cashTransfer,1);
        if(ErrorLog == 0){
            isOpen = false;
            std::cout << "User Found";
            NotifErrorLog = backend::CreateTransferNotification(InitialAccountNumber+14,cashTransfer,AI.actualName,Buffer);
        }
    };
    ImGui::SameLine();
    if(ImGui::Button("CANCEL")){
        isOpen = false;
        ErrorLog = 0;
    };

    switch(ErrorLog){
        case 0:
            ShowError1 = false;
            break;
        case 1:
            ShowError1 = true;
            break;
        case 2:
            ShowError2 = true;
            break;
    }

    if(!(NotifErrorLog == 0)){
        NotifError1 = true;
    }

    if(NotifError1 == true){
        ImGui::Spacing();
        ImGui::Text("NOTIFICATION ERROR");
    }

    if( ShowError1 == true){
        ImGui::Spacing();
        ImGui::Text("UNABLE TO LOCATE ACCOUNT");
    }

    if( ShowError2 == true){
        ImGui::Spacing();
        ImGui::Text("INVALID CASH AMOUNT");
    }


    ImGui::End();
}

void RequestInterface(int InitialAccountNumber, DATA::AccountInfo& AI, int AppWidth, int AppHeight, bool& isOpen){

    if(isOpen == false){
        return;            
    }

    ImGuiWindowFlags RI_flags = 0;
    RI_flags |= ImGuiWindowFlags_NoCollapse;
    RI_flags |= ImGuiWindowFlags_NoResize;
    RI_flags |= ImGuiWindowFlags_NoScrollbar;

    static char Buffer[64] = "";
    static int cashTransfer = 0;

    static int ErrorLog {0};
    static bool ShowError1{false};
    static bool ShowError2{false};

    std::array<int,2> AIwindowSize = {250,300};
    float xPos = (AppWidth - AIwindowSize[0]) / 2.0f;
    float yPos = (AppHeight - AIwindowSize[1]) / 2.0f;

    ImGui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(AIwindowSize[0], AIwindowSize[1]));

    ID windowTitle = "Request: " + AI.actualName + "###RequestInterface_" + std::to_string(reinterpret_cast<uintptr_t>(&AI));
    
    ImGui::Begin(windowTitle.c_str(), nullptr, RI_flags);
    ImGui::Text("REQUESTING MONEY FROM:");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(-1.0f);
    ID inputId = "##TransferInput_" + std::to_string(reinterpret_cast<uintptr_t>(&AI));
    ImGui::InputText(inputId.c_str(), Buffer, IM_ARRAYSIZE(Buffer));
    ImGui::Text("REQUEST AMOUNT:");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(-1.0f);
    ID AmountinputId = "##TransferCashInput_" + std::to_string(reinterpret_cast<uintptr_t>(&AI.APIcall));
    ImGui::InputInt(AmountinputId.c_str(), &cashTransfer);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    if(ImGui::Button("CONFIRM")){
        std::cout << Buffer;
        std::cout << cashTransfer;
        ErrorLog = backend::FindAccount(Buffer,AI.actualName,cashTransfer, 2);
        if(ErrorLog == 0){
            isOpen = false;
            std::cout << "User Found";
        }
    };
    ImGui::SameLine();
    if(ImGui::Button("CANCEL")){
        std::cout << "Pressed";
        isOpen = false;
        ErrorLog = 0;
    };

    switch(ErrorLog){
        case 0:
            ShowError1 = false;
            break;
        case 1:
            ShowError1 = true;
            break;
        case 2:
            ShowError2 = true;
            break;
    }

    if( ShowError1 == true){
        ImGui::Spacing();
        ImGui::Text("UNABLE TO LOCATE ACCOUNT");
    }

    if( ShowError2 == true){
        ImGui::Spacing();
        ImGui::Text("INVALID CASH AMOUNT");
    }


    ImGui::End();
}

void PrintLogification(const DATA::log& log){

    std::string LogText = log.type + " | [" + log.time + "] | " + log.text;

    if(log.type == "ERROR"){
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), LogText.c_str());
    }

    if(log.type == "STANDARD"){
        ImGui::Text(LogText.c_str());
    }

    if(log.type == "WARNING"){
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), LogText.c_str());
    }
}

void LogInterface(int AppWidth, int AppHeight){

    ImGuiWindowFlags LI_flag = 0;
    LI_flag |= ImGuiWindowFlags_NoCollapse;
    LI_flag |= ImGuiWindowFlags_NoResize;
    LI_flag |= ImGuiWindowFlags_NoMove;
    LI_flag |= ImGuiWindowFlags_NoInputs;
    LI_flag |= ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(AppWidth, AppHeight));

    ID LI_Title = "LOG ##LogInterface_" + std::to_string(AppWidth);

    ImGui::Begin(LI_Title.c_str(), nullptr, LI_flag);

    for(int i = 0; i <= 125; i++){
        if(backend::LogificationValidation(Logification[i])==1){
            PrintLogification(Logification[i]);
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }
    }

    ImGui::End();

}

}