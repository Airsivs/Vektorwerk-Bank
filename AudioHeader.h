#pragma once
#include <cstdint>
#include <string>
#include <algorithm>
#include <array>

namespace DATA{
    struct AccountCreation{
        std::string accName;
        int cash;
        int creditScore;
        bool isCompleted;
        bool ConversionCompleted;
    };

    struct AccountInfo{
        std::string actualName;
        int CurrentCash;
        int CurrentcreditScore;
        int APIcall;
        bool showTransferWindow = false;
        bool showRequestWindow = false;
        bool showMessageWindow = false;
        int notificationCode;
    };

    struct APIcalls{
        bool Transfer;
        bool withdraw;
        int InitiatorNum;
        int ReceiverNum;
        int amount;
        bool deposit;
    };

    struct notif{
        std::string title;
        std::string text;
        int amount;
        std::string receiver;
        std::string sender;
        int ID;
        bool active;
    };

    struct log{
        std::string type;
        std::string time;
        std::string text;
        int id;
        bool active;
    };
}

namespace backend{
    DATA::AccountInfo ConvertData(DATA::AccountInfo& AI, DATA::AccountCreation& accCr);
    void ShowWindowAPIcall(DATA::AccountInfo& AI, int accountNum, int AppW, int AppH);
    int FindAccount(std::string name, std::string InitiatorName, int amount, int type);
    int CheckForExistingUsername(std::string InitiatorName, DATA::AccountCreation& AC);
    int SendMessage(std::string InitiatorName,std::string Recipient, std::string Title, std::string Text);
    int CreateTransferNotification(int NotifID, int amount, std::string Initiator, std::string Recipient);
    int LogificationValidation(const DATA::log& log);
}

namespace frontend{
    int AmountOfAccounts(int width, int height, int AppWidth, int AppHeight);
    DATA::AccountCreation AccountInfo(int AccNumber, DATA::AccountCreation& accCr, int AppWidth, int AppHeight);
    DATA::AccountInfo AccountInterface(int AccountNumber, DATA::AccountInfo& AI, int AppWidth, int AppHeight, bool isOpen);
    void TransferInterface(int InitialAccountNumber, DATA::AccountInfo& AI, int AppWidth, int AppHeight, bool& isOpen);
    void RequestInterface(int InitialAccountNumber, DATA::AccountInfo& AI, int AppWidth, int AppHeight, bool& isOpen);
    void SendMessageInterface(int InitialAccountNumber, DATA::AccountInfo& AI, int AppWidth, int AppHeight, bool& isOpen);
    void LogInterface(int AppWidth, int AppHeight);
}

extern DATA::AccountInfo AccData[5];
extern DATA::AccountCreation AccInitData[5];
extern DATA::notif Notification[49];
extern DATA::log Logification[125];
extern int LogifNumber;