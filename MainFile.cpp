// Dear ImGui: standalone example application for DirectX 9

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <vector>
#include "MacroClass.h"
#include <cstring> 
#include "ButtonPresses.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include "SaveLoadMacro.h"


using namespace std;

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    //FreeConsole();

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ezMacro", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"ezMacro", WS_OVERLAPPEDWINDOW, 100, 100, 500, 700, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



    //File Exploerer Stuff/////////////////////////////////////////////////////////
    //I need to write this stuff down, took like 45 minutes just to open the 'Open file' box in windows
    //This is copied off the internet forums.  Who cares how it actually works

    OPENFILENAME ofn;       // common dialog box structure
    char szFile[260];       // buffer for file name
    //HWND hwnd = NULL;              // owner window
    HANDLE hf;              // file handle
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    //
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    //
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _TEXT("All\0*.*\0Text\0*.TXT\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ////////////////////////////////////////////////////////////////////////////////


    ///////////////Variables////////////////

    bool loopEverything = false;
    vector<MacroClass> MyMacros;

    ////////////////////////////////////////


    // Main loop
    bool done = false;

    //Stuff for adding new keys
    bool windowOpen = false; // For the add Key Window
    MacroClass newKey;
    char buf[255]{};
    char keybuf[1]{};
    char fileNameSavebuf[255]{};
    int whereToInsert = 0; //Change this before adding a new key
    //
    int loopXAmount = 0;
    int loopCounter = 0;
    ///
    //Counter for which key to press
    int keyCounter = 99999;
    time_t start = clock();
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        //////////////////////////////////////////////////////////////

        //Stuff To make window full screen in OS window
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        //Main Window
        ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

        //Load macro From File
        if (ImGui::Button("Select Macro File")) {
            GetOpenFileName(&ofn);
            //Load Macro
            MyMacros = LoadMacro(ofn.lpstrFile);
        }

        //Display File Name
        ImGui::Text(ofn.lpstrFile);

        //Save Macro Button and Text
        if (ImGui::Button("Save Macro as: ")) {
            SaveMacro(MyMacros, fileNameSavebuf);
        }

        ImGui::SameLine();
        ImGui::InputText(" ", fileNameSavebuf, sizeof(fileNameSavebuf));
        ///////////////////

        //Play Macro Keys
        if (ImGui::Button("Play Macro")) {
            start = clock();
            keyCounter = 0;
        }
        ImGui::SameLine();
        //Stop Macro
        if (ImGui::Button("Stop Macro")) {
            //start = clock();
            keyCounter = 9999999;
        }
        ImGui::SameLine();

        //Loop Everything after it's done button
        if (ImGui::Checkbox("Loop   ",&loopEverything)) {
            if (loopEverything == true) {
                printf("Loop Everything");
            }
            else {
                printf("Don't Loop Everything");
            }
        }
        //Loop X amount of Times Stuff
        if (loopEverything == true) {
            ImGui::SameLine();
            ImGui::PushItemWidth(85);
            ImGui::InputInt("Times", &loopXAmount, 1, 100);
            ImGui::PopItemWidth();
        }

        //Add Key Stuff
        
        if (ImGui::Button("Add New Key")){
            windowOpen = true;
            whereToInsert = MyMacros.size();
            
        }
        ImGui::Separator();

        if (windowOpen == true) {
            //New Key Window
            ImGui::Begin("Add New Key");
            //Settings

            ImGui::InputText("Key Name", buf, sizeof(buf));
            ImGui::InputText("Key To Press", keybuf, sizeof(buf));
            ImGui::InputFloat("Timer", &newKey.timer, 0.1f, 1.0f, "%.3f");

            //Set button
            if (ImGui::Button("Set Key")) {
                newKey.name = buf;
                newKey.myButton = keybuf[0];
                MyMacros.insert(MyMacros.begin() + whereToInsert, newKey);
                windowOpen = false;
                //MyMacros[0].pushButton();
            }


            ImGui::End();
        }

        //Show List of Keys
        for (size_t i = 0; i < MyMacros.size(); i++)
        {
            string timerNumber = to_string(MyMacros[i].timer);
            string addButtonID = "Add Key Here##" + to_string(i);
            string removeButtonID = "Delete Key##" + to_string(i);
            //string editButtonID = "Edit##" + to_string(i);
            /////////
            
            //If statement around name for if we are Running this key currently, make it green
            if (keyCounter == i) {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                ImGui::Text(MyMacros[i].name.c_str());
                ImGui::PopStyleColor();
            }
            else {
                ImGui::Text(MyMacros[i].name.c_str());
            }
            string myButtonTemp = &MyMacros[i].myButton;
            ImGui::Text(&myButtonTemp[0]);
            ImGui::Text(timerNumber.c_str());

            /* Might not need this.
            if (ImGui::Button(editButtonID.c_str())) {
                
            }
            */

            if (ImGui::Button(removeButtonID.c_str())) {
                MyMacros.erase(MyMacros.begin() + i);
            }
            if (ImGui::Button(addButtonID.c_str())) {
                windowOpen = true;
                whereToInsert = i + 1;
            }
            //ImGui::InputFloat("Timer",&MyMacros[i].timer, 0.1f, 1.0f, "%.3f");

            //Do The Macro stuff here///////////////////////////////////////////////////////////
            if (keyCounter == i) {
                if (difftime(clock(), start) / 1000 >= MyMacros[i].timer) {
                    cout << "Push the button" << endl;
                    MyMacros[i].pushButton();
                    start = clock();
                    keyCounter++;
                    //Check if we need to restart
                    if (loopEverything == true && keyCounter > MyMacros.size() - 1) {
                        //If loop counter is 0, just keep looping forever
                        if (loopXAmount == 0) {
                            keyCounter = 0;
                            start = clock();
                        }
                        else {
                            keyCounter = 0;
                            start = clock();
                            loopCounter++;
                        }
                    }
                }
                //cout << difftime(clock(), start) << endl; //outputting every millisecond
            }
            if (loopXAmount != 0 && loopCounter >= loopXAmount) {
                cout << "End of looping x amount of times" << endl;
                loopCounter = 0;
                keyCounter = 99999;
            }
            ImGui::Separator();
        }

        //cout << keyCounter << endl;
        ImGui::NewLine();
        ImGui::End();

        char tester = 'k';
        int testthexnum = int('k');
        //cout << std::hex << testthexnum << endl;



        /*
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        */


        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
