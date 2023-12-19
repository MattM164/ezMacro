#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
//#include "MacroClass.h"

void SaveMacro(vector<MacroClass> myMacros, char fileNameSavebuf[255]) {
    ofstream outfile(fileNameSavebuf);
    //outfile << "Saved" << endl;

    outfile << "#" << endl;
    for (size_t i = 0; i < myMacros.size(); i++)
    {

        outfile << myMacros[i].name << endl;
        outfile << myMacros[i].timer << endl;
        outfile << myMacros[i].myButton << endl;
        outfile << myMacros[i].holdCtrl << endl;
        outfile << myMacros[i].holdShift << endl;
        //Was Getting an extra empty macro because of the last character.  This just makes sure its not there
        if (i != myMacros.size() - 1) {
            outfile << "#" << endl;
        }
       

    }


    outfile.close();
}

vector<MacroClass> LoadMacro(string fileName) {
    vector<MacroClass> MyMacrosFromFile;
    cout << "Loading from file: " << fileName << endl;
    ifstream macroFile;
    macroFile.open(fileName);
    string line = "";
    int macroCount = 0;
    while (getline(macroFile, line))
    {
        MacroClass newMacro;
        string nextline;
        MyMacrosFromFile.push_back(newMacro);
        getline(macroFile, nextline);
        MyMacrosFromFile[macroCount].name = nextline;
        getline(macroFile, nextline);
        float theTimer = stof(nextline);
        MyMacrosFromFile[macroCount].timer = theTimer;
        //cout << nextline;
        getline(macroFile, nextline);
        MyMacrosFromFile[macroCount].myButton = *nextline.c_str();
        getline(macroFile, nextline);
        //MyMacrosFromFile[macroCount].holdCtrl = nextline[0];
        if (nextline == "0") {
            MyMacrosFromFile[macroCount].holdCtrl = false;
        }
        else {
            MyMacrosFromFile[macroCount].holdCtrl = true;
        }
        getline(macroFile, nextline);
       // MyMacrosFromFile[macroCount].holdShift = nextline[0];
        if (nextline == "0") {
            MyMacrosFromFile[macroCount].holdShift = false;
        }
        else {
            MyMacrosFromFile[macroCount].holdShift = true;
        }
        macroCount++;
    }
    macroFile.close();
    return MyMacrosFromFile;
}