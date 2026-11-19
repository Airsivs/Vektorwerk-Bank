#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "AudioHeader.h"
#include <chrono>
#include <thread>
#include <cstdint>
#include <array>

std::array<int,2> WindowSizeMain {1280,720};
std::array<int,2> WindowSizeSecondary {720,840};
DATA::AccountCreation AccInitData[5]{};
DATA::AccountInfo AccData[5]{};
DATA::notif Notification[49]{};
DATA::log Logification[125]{};
static bool AllowAccountInfoWin{0};
static bool DisplayActiveAccounts{0};
static bool DBG_notifExample{1};

void RunAccountCreation(int AOA) {
    static bool IsOperational{1};
    static int currentAccount{1};
    static bool isCompleted{0};

    if (IsOperational == 0) {
        return;
    }

    if (currentAccount <= AOA) {
        frontend::AccountInfo(currentAccount, AccInitData[currentAccount - 1], WindowSizeMain[0], WindowSizeMain[1]);
        backend::ConvertData(AccData[currentAccount - 1],AccInitData[currentAccount - 1]);
        if (AccInitData[currentAccount - 1].isCompleted == 1) {
            currentAccount += 1;
        }
    } else {
        isCompleted = 1;
        AllowAccountInfoWin = 1;
        DisplayActiveAccounts = 1;
        IsOperational = 0;
    }
}

void DisplayAllAvailableAccounts(int ActiveAccounts){

    if (DisplayActiveAccounts == 0){
        return;
    }

    switch(ActiveAccounts){
        case 5:
            backend::ShowWindowAPIcall(AccData[4],5,WindowSizeMain[0], WindowSizeMain[1]);
            frontend::AccountInterface(5,AccData[4],WindowSizeMain[0], WindowSizeMain[1],AllowAccountInfoWin);
            [[fallthrough]];
        case 4:
            backend::ShowWindowAPIcall(AccData[3],4,WindowSizeMain[0], WindowSizeMain[1]);
            frontend::AccountInterface(4,AccData[3],WindowSizeMain[0], WindowSizeMain[1],AllowAccountInfoWin);
            [[fallthrough]];
        case 3:
            backend::ShowWindowAPIcall(AccData[2],3,WindowSizeMain[0], WindowSizeMain[1]);
            frontend::AccountInterface(3,AccData[2],WindowSizeMain[0], WindowSizeMain[1],AllowAccountInfoWin);
            [[fallthrough]];
        case 2:
            backend::ShowWindowAPIcall(AccData[1],2,WindowSizeMain[0], WindowSizeMain[1]);
            frontend::AccountInterface(2,AccData[1],WindowSizeMain[0], WindowSizeMain[1],AllowAccountInfoWin);
            [[fallthrough]];
        case 1:
            backend::ShowWindowAPIcall(AccData[0],1,WindowSizeMain[0], WindowSizeMain[1]);
            frontend::AccountInterface(1,AccData[0],WindowSizeMain[0], WindowSizeMain[1],AllowAccountInfoWin);
            break;
        default:
            std::cout << "ERROR - DAAA switch";
    }

};

int main(){

    if(!glfwInit()){return -1;};

    GLFWwindow* WindowMain = glfwCreateWindow(WindowSizeMain[0], WindowSizeMain[1], "VEKTORWERK BANK - [MAIN]", NULL, NULL);
    if(!WindowMain){
        glfwTerminate();
        return -1;
    }

    GLFWwindow* WindowSecondary = glfwCreateWindow(WindowSizeSecondary[0], WindowSizeSecondary[1], "VEKTORWERK BANK - [SECONDARY]", NULL, NULL);
    if(!WindowSecondary){
        glfwDestroyWindow(WindowMain);
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(WindowMain);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGuiContext* ctxMain = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctxMain);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(WindowMain, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glfwMakeContextCurrent(WindowSecondary);
    glfwSwapInterval(1);

    ImGuiContext* ctxSecondary = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctxSecondary);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(WindowSecondary, true);
    ImGui_ImplOpenGL3_Init("#version 130");


    while(!glfwWindowShouldClose(WindowMain) && !glfwWindowShouldClose(WindowSecondary)){

        glfwPollEvents();

        glfwMakeContextCurrent(WindowMain);
        ImGui::SetCurrentContext(ctxMain);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const int AOA = frontend::AmountOfAccounts(300, 125, WindowSizeMain[0], WindowSizeMain[1]);

        if (AOA > 0){
            RunAccountCreation(AOA);
        }

        if(DBG_notifExample==1){
            Notification[0] = {"Poopie", "Lorem ipsum, fart on my finger", 300, AccData[0].actualName.c_str(), AccData[1].actualName.c_str(), 0, true};
            Notification[1] = {"Dream", "Lorem ipsum, fart on my finger. I need to extend this text so. Bla bla poop in my mouth tom pearl", 300, AccData[1].actualName.c_str(), AccData[2].actualName.c_str(), 1, true};
            Notification[2] = {"Cream", "Lorem ipsum, fart on my finger", 300, AccData[2].actualName.c_str(), AccData[3].actualName.c_str(), 2, true};
            Notification[3] = {"Poopie Max", "Lorem ipsum, fart on my finger", 300, AccData[2].actualName.c_str(), AccData[4].actualName.c_str(), 3, true};

            Logification[1] = {"TYPE", " [TIME]", " | TEXT", 1, true};
        }

        DisplayAllAvailableAccounts(AOA);

        ImGui::Render();
        int display_w1, display_h1;
        glfwGetFramebufferSize(WindowMain, &display_w1, &display_h1);
        glViewport(0, 0, display_w1, display_h1);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(WindowMain);

        glfwMakeContextCurrent(WindowSecondary);
        ImGui::SetCurrentContext(ctxSecondary);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        frontend::LogInterface(WindowSizeSecondary[0], WindowSizeSecondary[1]);

        ImGui::Render();
        int display_w2, display_h2;
        glfwGetFramebufferSize(WindowSecondary, &display_w2, &display_h2);
        glViewport(0, 0, display_w2, display_h2);
        glClearColor(0.35f, 0.45f, 0.50f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(WindowSecondary);
    }

    ImGui::SetCurrentContext(ctxSecondary);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(ctxSecondary);

    ImGui::SetCurrentContext(ctxMain);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(ctxMain);

    glfwDestroyWindow(WindowMain);
    glfwDestroyWindow(WindowSecondary);
    glfwTerminate();

    return 0;
}
