#include <bits/stdc++.h>
using namespace std;

class Tables {
    unordered_map<string, pair<string, int>> OPTAB;
    unordered_map<string, int> REG;
    unordered_map<string, int> CONDCS;
    vector<pair<string, int>> SYMBTAB;
    vector<pair<string, int>> LITTAB;
    vector<int> POOLTAB;

    public:
    Tables()  {
        OPTAB["STOP"] = {"IS",0};
		OPTAB["ADD"] = {"IS",1};
		OPTAB["SUB"] = {"IS",2};
		OPTAB["MULT"] = {"IS",3};
		OPTAB["MOVER"] = {"IS",4};
		OPTAB["MOVEM"] = {"IS",5};
		OPTAB["COMP"] = {"IS",6};
		OPTAB["BC"] = {"IS",7};
		OPTAB["DIV"] = {"IS",8};
		OPTAB["READ"] = {"IS",9};
		OPTAB["PRINT"] = {"IS",10};
		OPTAB["START"] = {"AD",1};
		OPTAB["END"] = {"AD",2};
		OPTAB["ORIGIN"] = {"AD",3};
		OPTAB["EQU"] = {"AD",4};
		OPTAB["LTORG"] = {"AD",5};
		OPTAB["DC"] = {"DL",1};
		OPTAB["DS"] = {"DL",2};

        REG["AREG"] = 1;
		REG["BREG"] = 2;
		REG["CREG"] = 3;
		REG["DREG"] = 4;

		CONDCS["LT"] = 1;
		CONDCS["LE"] = 2;
		CONDCS["EQ"] = 3;
		CONDCS["GT"] = 4;
		CONDCS["GE"] = 5;
		CONDCS["ANY"] = 6;
    }

    int searchSymbol(string symbol) {
        for (int i = 0; i < SYMBTAB.size(); i++) {
            if (SYMBTAB[i].first == symbol) {
                return i;
            }
        }
        return -1;
    }

    friend class Assembler;
};

class Assembler {
    Tables t;
    fstream file;
    fstream intermediateCode;
    fstream symbolTable;
    fstream literalTable;
    fstream poolTable;
    int lc = 0;
    int counter = 0;
    int pcounter = 0;
    int scounter = 0;
    int lcounter = 0;
    
    public:
    Assembler() {
        file.open("test.txt", ios::in);
        if (!file.is_open()) {
            cout<<"Error opening test file"<<endl;
        }

        intermediateCode.open("intermediate_code", ios::out);
        if (!intermediateCode.is_open()) {
            cout<<"Error opening intermediate code file"<<endl;
        }

        symbolTable.open("symbol_table", ios::out);
        if (!symbolTable.is_open()) {
            cout<<"Error opening symbol table file"<<endl;
        }

        literalTable.open("literal_tabel", ios::out);
        if (!literalTable.is_open()) {
            cout<<"Error opening literal table file"<<endl;
        }

        poolTable.open("pool_table", ios::out);
        if (!poolTable.is_open()) {
            cout<<"Error opening pool table file"<<endl;
        }
    }

    pair<string, int> split(string str) {
        pair<string, int> res;
        string w = "";

        for (int i = 0; i < str.size(); i++) {
            if (str[i] != '+') {
                w += str[i];
            } else {
                res.first = w;
                w = "";
            }
        }
        res.second = stoi(w);
        return res;
    }

    void sizeOne(vector<string> words) {
        pair<string, int> ic;
        int temp = 0;

        if (words[0] == "LTORG") {
            ic = t.OPTAB[words[0]];
            t.POOLTAB.push_back(pcounter + 1);

            lc = counter + 1;
            for (int i = pcounter; i < t.LITTAB.size(); i++) {
                t.LITTAB[i].second = ++counter;
                temp = i;
            }
            pcounter = temp + 1;
        } else {
            lc = ++counter;
            ic = t.OPTAB[words[0]];
        }
        intermediateCode<<lc<<" ("<<ic.first<<","<<ic.second<<")"<<endl;
    }

    void sizeTwo(vector<string> words) {
        lc = ++counter;
        pair<string, int> ic;
        pair<string, int> ic1;
        pair<string, int> symbol;
        int index = 0;

        if (words[0] == "ORIGIN") {
            pair<string, int> res;
            string w = "";
            res = split(words[1]);
            ic = t.OPTAB[words[0]];
            index = t.searchSymbol(res.first);
            counter = t.SYMBTAB[index].second + res.second - 1;
            ic1.first = "S";
            ic1.second = index + 1;
        } else {
            ic = t.OPTAB[words[0]];
            index = t.searchSymbol(words[1]);
            ic1.first = "S";
            if (index != -1) {
                ic1.second = index + 1;
            } else {
                ic1.second = ++scounter;
                symbol.first = words[1];
                symbol.second = -1;
                t.SYMBTAB.push_back(symbol);
            }
        }
        intermediateCode<<lc<<" ("<<ic.first<<","<<ic.second<<") ("<<ic1.first<<","<<ic1.second<<")"<<endl;
    }

    void sizeThree(vector<string> words) {
        lc = ++counter;
        pair<string, int> ic;
        pair<string, int> literal;
        pair<string, int> symbol;
        int index = 0;

        ic = t.OPTAB[words[0]];
        if (t.REG.find(words[1]) != t.REG.end()) {
            int ic1 = t.REG[words[1]];
            if (t.REG.find(words[2]) != t.REG.end()) {
                int ic2 = t.REG[words[2]];
                intermediateCode<<lc<<" ("<<ic.first<<","<<ic.second<<") ("<<ic1<<") ("<<ic2<<")"<<endl;
            } else if (words[2].at(0) == '=') {
                pair<string, int> ic2;
                ic2.first = "L";
                ic2.second = ++lcounter;
                literal.first = words[2];
                literal.second = -1;
                t.LITTAB.push_back(literal);
                intermediateCode<<lc<<" ("<<ic.first<<","<<ic.second<<") ("<<ic1<<") ("<<ic2.first<<","<<ic2.second<<")"<<endl;
            } else {
                pair<string, int> ic2;
                ic2.first = "S";
                index = t.searchSymbol(words[2]);
                if (index != -1) {
                    ic2.second = index + 1;
                } else {
                    ic2.second = ++scounter;
                    symbol.first = words[2];
                    symbol.second = -1;
                    t.SYMBTAB.push_back(symbol);
                }
                intermediateCode<<lc<<" ("<<ic.first<<","<<ic.second<<") ("<<ic1<<") ("<<ic2.first<<","<<ic2.second<<")"<<endl;
            }
        } else {
            ic = t.OPTAB[words[0]];
            int ic1 = t.CONDCS[words[1]];
            pair<string, int> ic2;

            index = t.searchSymbol(words[2]);
            ic2.first = "S";
            if (index  != -1) {
                ic2.second = index + 1;
            } else {
                ic2.second = ++scounter;
                symbol.first = words[2];
                symbol.second = -1;
                t.SYMBTAB.push_back(symbol);
            }
            intermediateCode<<lc<<" ("<<ic.first<<","<<ic.second<<") ("<<ic1<<") ("<<ic2.first<<","<<ic2.second<<")"<<endl;
        }
    }

    void passOne() {
        vector<string> words;
        string line;
        string w = "";
        pair<string, int> ic;
        if (file.is_open()) {
            while (!file.eof()) {
                getline(file, line);
                w = "";
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
                //     intermediateCode<<words[i]<<" ";
                // }
                // intermediateCode<<endl;

                if (words[0] == "START") {
                    counter = stoi(words[1]);
                    ic = t.OPTAB[words[0]];
                    intermediateCode<<" ("<<ic.first<<","<<ic.second<<") "<<"(C,"<<words[1]<<")"<<endl;
                } else if (words[0] == "END") {
                    ic = t.OPTAB[words[0]];
                    intermediateCode<<" ("<<ic.first<<","<<ic.second<<")"<<endl;
                    
                    lc = counter + 1;
                    t.POOLTAB.push_back(pcounter + 1);
                    for (int i = pcounter; i < t.LITTAB.size(); i++) {
                        t.LITTAB[i].second =  ++counter;
                    }
                    break;
                } else if (t.OPTAB.find(words[0]) != t.OPTAB.end()) {
                    if (words.size() == 1) {
                        sizeOne(words);
                    } else if (words.size() == 2) {
                        sizeTwo(words);
                    } else {
                        sizeThree(words);
                    }
                } else {
                    pair<string, int> ic;
                    pair<string, int> ic1;
                    pair<string, int> ic2;
                    pair<string, int> symbol;
                    int index = 0;

                    ic.first = "S";
                    index = t.searchSymbol(words[0]);
                    if (index != -1) {
                        ic.second = index + 1;
                    } else {
                        ic.second = ++scounter;
                        symbol.first = words[0];
                        symbol.second = -1;
                        t.SYMBTAB.push_back(symbol);
                    }

                    if (words[1] == "EQU") {
                        lc = ++counter;
                        ic1 = t.OPTAB[words[1]];
                        index = t.searchSymbol(words[0]);
                        t.SYMBTAB[index].second = lc;
                        ic2.first = "S";
                        index = t.searchSymbol(words[2]);
                        ic2.second = index + 1;
                        intermediateCode<<lc<<" ("<<ic.first<<","<<ic.second<<") ("<<ic1.first<<","<<ic1.second<<") ("<<ic2.first<<","<<ic2.second<<")"<<endl;
                    } else if (words[1] == "DS" || words[1] == "DC") {
                        lc = ++counter;
                        ic1 = t.OPTAB[words[1]];
                        ic2.first = "C";
                        ic2.second = stoi(words[2]);
                        index = t.searchSymbol(words[0]);
                        t.SYMBTAB[index].second = lc;
                        intermediateCode<<lc<<" ("<<ic.first<<","<<ic.second<<") ("<<ic1.first<<","<<ic1.second<<") ("<<ic2.first<<","<<ic2.second<<")"<<endl;
                    } else {
                        vector<string> withoutLabel;
                        for (int i = 1; i < words.size(); i++) {
                            withoutLabel.push_back(words[i]);
                        }

                        index = t.searchSymbol(words[0]);
                        t.SYMBTAB[index].second = counter + 1;
                        if (words.size() == 2) {
                            sizeOne(withoutLabel);
                        } else if (words.size() == 3) {
                            sizeTwo(withoutLabel);
                        } else {
                            sizeThree(withoutLabel);
                        }
                    }
                }
                words.clear();
            }
        }
    }

    void displaySymTab() {
        for (int i = 0; i < t.SYMBTAB.size(); i++) {
            symbolTable<<t.SYMBTAB[i].first<<" "<<t.SYMBTAB[i].second<<endl;
        }
    }

    void displayLitTab() {
        for (int i = 0; i < t.LITTAB.size(); i++) {
            literalTable<<t.LITTAB[i].first<<" "<<t.LITTAB[i].second<<endl;
        }
    }

    void displayPoolTab() {
        for (int i = 0; i < t.POOLTAB.size(); i++) {
            poolTable<<t.POOLTAB[i]<<endl;
        }
    }
};

int main() {
    Assembler a;
    a.passOne();
    a.displaySymTab();
    a.displayLitTab();
    a.displayPoolTab();
    return 0;
}