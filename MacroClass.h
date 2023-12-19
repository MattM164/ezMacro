#include <string>
#include <vector>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <winuser.h>
#include <Windows.h>
using namespace std;

void pushMyButton(char button, bool shift, bool control);

class MacroClass {

	public:
		string name = "Name";

		char myButton = '0';
		float timer = 0.0f;
        bool holdShift = false;
        bool holdCtrl = false;

		void pushButton() {
			pushMyButton(myButton, holdShift, holdCtrl);
		}

	
	private:

};

void pushMyButton(char button, bool shift, bool control) {
    //Setup
    INPUT ip;
    if (GetAsyncKeyState(0x05)) {}

    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ///////////////////////////


    //If Shift key needs pressed, press it here.  We release it at the bottom
    if (shift) {
        //Press 0
        ip.ki.wVk = 0x30; // virtual-key code for the "numpad 0" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    }

    //int testthexnum = int(button);
    //std::stringstream ss();
    //ss << std::hex << testthexnum << endl;
    //std::string mystr = ss.str();
    //cout << "normal: " << int(button) << endl;
    //cout << "-32: " << int(button) - 32 << endl;
    int buttonTranslated = 0;
    if (int(button) >= 65) {
        buttonTranslated = int(button) - 32;
        //cout << "Letter" << endl;
    }
    else {
        buttonTranslated = int(button);
        //cout << "Number" << endl;
    }
    ip.ki.wVk = buttonTranslated; // virtual-key code
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    //Release
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));


    
    //cout << int('0') << endl;
    //Number row Keycodes
    /*
    if (button == '0') {
        ip.ki.wVk = 0x30; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '1') {
        ip.ki.wVk = 0x31; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '2') {
        ip.ki.wVk = 0x32; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '3') {
        ip.ki.wVk = 0x33; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '4') {
        ip.ki.wVk = 0x34; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '5') {
        ip.ki.wVk = 0x35; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '6') {
        ip.ki.wVk = 0x36; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '7') {
        ip.ki.wVk = 0x37; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '8') {
        ip.ki.wVk = 0x38; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    if (button == '9') {
        ip.ki.wVk = 0x39; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    */

    ///////////////////////////////////TEMPTEMPTEMPTEMP
    /*
    if (button == 'g') {
        ip.ki.wVk = VK_NUMPAD0; // virtual-key code for 
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        //Release
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    */



    //If Shift key needs pressed, Release it here
    if (shift) {
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
    
}