#include <bits/stdc++.h>
using namespace std;

class MNTValues {
    int PP;
    int KP;
    int KPDTAB;
    int MDTP;

    public:
    MNTValues () {
        PP = 0;
        KP = 0;
        KPDTAB = 0;
        MDTP = 0;
    }

    friend class MacroProcessor;
};

class MacroProcessor {
    unordered_map<string, MNTValues> MNT;
    vector<vector<string>> MDT;
    vector<pair<string, string>> KPDTAB;
    vector<string> PNTAB;
    vector<pair<string, string>> APTAB;
    vector<string> words;
    fstream file;
    fstream macroNameTable;
    fstream macroDefinitionTable;
    fstream keywordParameterTabel;
    fstream parameterTable;
    fstream actualParameterTable;
    fstream output;

    public:
    MacroProcessor() {
        file.open("macro_input.txt", ios::in);
        if (!file.is_open()) {
            cout<<"Error opening input file"<<endl;
        }

        macroNameTable.open("macro_name_table.txt", ios::in);
        if (macroNameTable.is_open()) {
            string line;
            vector<string> words;
            MNTValues m;
            while (!macroNameTable.eof()) {
                string w = "";
                getline(macroNameTable, line);
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        words.push_back(w);
                        w = "";
                    }
                }

                words.push_back(w);
                m.PP = stoi(words[1]);
                m.KP = stoi(words[2]);
                m.KPDTAB = stoi(words[3]);
                m.MDTP = stoi(words[4]);
                MNT[words[0]] = m;
                words.clear();
            }
        } else {
            cout<<"Error opeing macro name table file"<<endl;
        }

        macroDefinitionTable.open("macro_definition_table.txt", ios::in);
        if (macroDefinitionTable.is_open()) {
            string line;
            while (!macroDefinitionTable.eof()) {
                string w = "";
                vector<string> words;
                getline(macroDefinitionTable, line);
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        words.push_back(w);
                        w = "";
                    }
                }
                words.push_back(w);
                MDT.push_back(words);
                words.clear();
            }
        } else {
            cout<<"Error opening macro definition table file"<<endl;
        }

        keywordParameterTabel.open("keyword_parameter_table.txt", ios::in);
        if (keywordParameterTabel.is_open()) {
            string line;
            while (!keywordParameterTabel.eof()) {
                string w = "";
                getline(keywordParameterTabel, line);
                vector<string> words;
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        words.push_back(w);
                        w = "";
                    }
                }
                words.push_back(w);
                KPDTAB.push_back({words[0], words[1]});
                words.clear();
            }
        } else {
            cout<<"Error opening keyword parameter table file"<<endl;
        }

        parameterTable.open("parameter_table.txt", ios::in);
        if (parameterTable.is_open()) {
            string line;
            while (!parameterTable.eof()) {
                string w = "";
                getline(parameterTable, line);
                PNTAB.push_back(line);
            }
        }

        actualParameterTable.open("actual_parameter.txt", ios::out);
        if (!actualParameterTable.is_open()) {
            cout<<"Error opening actual parameter table file"<<endl;
        }

        output.open("output.txt", ios::out);
        if (!output.is_open()) {
            cout<<"Error opening output file"<<endl;
        }
    }

    void actualParameter(int reference, string name) {
        APTAB.push_back({name, ""});
        for (int i = 1; i <= MNT[name].PP; i++) {
            APTAB.push_back({PNTAB[reference + i], words[i]});
        }

        for (int i = MNT[name].KPDTAB - 1; i < MNT[name].KPDTAB + MNT[name].KP - 1; i++) {
            if (KPDTAB[i].second != "") {
                APTAB.push_back({KPDTAB[i].first, KPDTAB[i].second});
            } else {
                APTAB.push_back({KPDTAB[i].first, ""});
            }
        }

        for (int i = MNT[name].PP + 1; i < words.size(); i++) {
            if (words[i].at(0) == '&') {
                int index = reference + MNT[name].PP + 1;
                for (int j = index; j < index + MNT[name].KP; j++) {
                    if (words[i] == APTAB[j].first) {
                        APTAB[j].second = words[i + 2];
                    }
                }
            }
        }
    }

    void expandMacro(string name, int reference) {
        bool insideMacro = true;
        for (int i = MNT[name].MDTP - 1; MDT.size(); i++) {
            if (MNT.find(MDT[i][0]) != MNT.end()) {
                words.clear();
                for (int j = 0; j < MDT[i].size(); j++) {
                    words.push_back(MDT[i][j]);
                }
                int temp = 0;
                for (int k = 0; k < PNTAB.size(); k++) {
                    if (MDT[i][0] == PNTAB[k]) {
                        temp = k;
                        break;
                    }
                }
                actualParameter(temp, MDT[i][0]);
                for (int k = 0; k < APTAB.size(); k++) {
                    if (MDT[i][0] == APTAB[k].first) {
                        temp = k;
                        break;
                    }
                }
                expandMacro(MDT[i][0], temp);
                continue;
            }
            for (int j = 0; j < MDT[i].size(); j++) {
                if (MDT[i][j] == "MEND") {
                    insideMacro = false;
                    break;
                } else {
                    if (MDT[i][j].at(0) == '(') {
                        int index = reference + int(MDT[i][j].at(3) - '0');
                        output<<" "<<APTAB[index].second;
                    } else {
                        if (j == 0) {
                            output<<MDT[i][j];
                        } else {
                            output<<" "<<MDT[i][j];
                        }
                    }
                }
            }

            if (!insideMacro) {
                break;
            }

            output<<endl;
        }
    }

    void passTwo() {
        string line;
        string name;
        bool insideStart;
        if (file.is_open()) {
            while (!file.eof()) {
                string w = "";
                getline(file, line);
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        words.push_back(w);
                        w = "";
                    }
                }
                words.push_back(w);

                if (words[0] == "START") {
                    insideStart = true;
                    output<<line<<endl;
                } else if (words[0] == "END") {
                    insideStart = false;
                    output<<line<<endl;
                }else if (insideStart && MNT.find(words[0]) != MNT.end()) {
                    name = words[0];
                    int reference = 0;
                    for (int i = 0; i < PNTAB.size(); i++) {
                        if (PNTAB[i] == name) {
                            reference = i;
                            break;
                        }
                    }
                    actualParameter(reference, name);
                    for (int i = 0; i < APTAB.size(); i++) {
                        if (APTAB[i].first == name) {
                            reference = i;
                            break;
                        }
                    }
                    expandMacro(name, reference);
                } else if (insideStart) {
                    output<<line<<endl;
                }
                words.clear();
            }
        }
    }

    void displayAPTAB() {
        for (auto x: APTAB) {
            actualParameterTable<<x.first<<" "<<x.second<<endl;
        }
    }
};

int main() {
    MacroProcessor m;
    m.passTwo();
    m.displayAPTAB();
}