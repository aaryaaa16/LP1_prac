#include <bits/stdc++.h>
using namespace std;

class MNTValues {
    int PP;
    int KP;
    int MDTP;
    int KPDTP;

    public:
    MNTValues () {
        PP = 0;
        KP = 0;
        MDTP = 0;
        KPDTP = 0;
    }

    friend class MacroProcessor;
};

class MacroProcessor {
    vector<pair<string, MNTValues>> MNT;
    vector<vector<string>> MDT;
    vector<pair<string, string>> KPDTAB;
    vector<string> PNTAB;
    fstream file;
    fstream macroNameTable;
    fstream macroDefinitionTable;
    fstream keywordParameterTable;
    fstream parameterTable;

    public:
    MacroProcessor () {
        file.open("macro_input.txt", ios::in);
        if (!file.is_open()) {
            cout<<"Error opening input file"<<endl;
        }

        macroNameTable.open("macro_name_table.txt", ios::out);
        if (!macroNameTable.is_open()) {
            cout<<"Error opening macro name table file"<<endl;
        }

        macroDefinitionTable.open("macro_definition_table.txt", ios::out);
        if (!macroDefinitionTable.is_open()) {
            cout<<"Error opening macro definition table file"<<endl;
        }

        keywordParameterTable.open("keyword_parameter_table.txt", ios::out);
        if (!keywordParameterTable.is_open()) {
            cout<<"Error opening keyword parameter table file"<<endl;
        }
        
        parameterTable.open("parameter_table.txt", ios::out);
        if (!parameterTable.is_open()) {
            cout<<"Error opening parameter table file"<<endl;
        }
    }

    int searchParameter (string parameter, string name) {
        int reference;
        for (int i = 0; i < PNTAB.size(); i++) {
            if (PNTAB[i] == name) {
                reference = i;
                break;
            }
        }

        for (int i = reference + 1; i < PNTAB.size(); i++) {
            if (PNTAB[i] == parameter) {
                return i - reference;
            }
        }

        return -1;
    }

    void passOne () {
        vector<string> words;
        vector<string> temp;
        string line;
        bool insideMacro;
        bool firstLine;
        string name;
        int pcounter = 0;
        int kcounter = 0;
        int mcounter = 0;
        MNTValues m;

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

                // for (int i = 0; i < words.size(); i++) {
                //     cout<<words[i]<<" ";
                // }
                // cout<<endl;

                if (words[0] == "MACRO") {
                    firstLine = true;
                    insideMacro = true;
                } else if (firstLine && insideMacro) {
                    name = words[0];
                    PNTAB.push_back(name);
                    for (int i = 0; i < words.size(); i++) {
                        if (words[i].at(0) == '&') {
                            PNTAB.push_back(words[i]);
                            pcounter++;
                        } else if (words[i] == "=") {
                            if (i == words.size() - 1 || words[i + 1].at(0) == '&') {
                                KPDTAB.push_back({words[i], ""});
                                kcounter++;
                            } else {
                                KPDTAB.push_back({words[i - 1], words[i + 1]});
                                kcounter++;
                            }
                        }
                    }
                    if (kcounter != 0) {
                        m.KPDTP = KPDTAB.size() - kcounter + 1;
                    }
                    m.KP = kcounter;
                    m.PP = pcounter - kcounter;
                    m.MDTP = MDT.size() + 1;
                    firstLine = false;

                    kcounter = 0;
                    pcounter = 0;
                    MNT.push_back({words[0], m});
                } else if(!firstLine && insideMacro) {
                    for (int i = 0; i < words.size(); i++) {
                        if (words[i].at(0) == '&') {
                            string index = to_string(searchParameter(words[i], name));
                            if (index != "-1") {
                                string parameter = "(P,";
                                parameter += index;
                                parameter += ")";
                                temp.push_back(parameter);
                            } else {
                                temp.push_back(words[i]);
                            }
                        } else {
                            temp.push_back(words[i]);
                        }
                    }
                    MDT.push_back(temp);
                    temp.clear();
                } else {
                    continue;
                }
                if (words[0] == "MEND") {
                    insideMacro = false;
                }
                words.clear();
            }
        }
    }

    void displayParameterTable() {
        for (auto x: PNTAB) {
            parameterTable<<x<<endl;
        }
    }

    void displayKeywordTable() {
        for (auto x: KPDTAB) {
            keywordParameterTable<<x.first<<" "<<x.second<<endl;
        }
    }

    void displayMacroNameTable() {
        for (auto x: MNT) {
            macroNameTable<<x.first<<" "<<x.second.PP<<" "<<x.second.KP<<" "<<x.second.KPDTP<<" "<<x.second.MDTP<<endl;
        }
    }

    void displayMacroDefinitionTable() {
        for (int i = 0; i < MDT.size(); i++) {
            for (int j = 0; j < MDT[i].size(); j++) {
                macroDefinitionTable<<MDT[i][j]<<" ";
            }
            macroDefinitionTable<<endl;
        }
    }
};

int main() {
    MacroProcessor m;
    m.passOne();
    m.displayParameterTable();
    m.displayKeywordTable();
    m.displayMacroNameTable();
    m.displayMacroDefinitionTable();
}