#include <bits/stdc++.h>
using namespace std;

class Assembler {
    vector<pair<string, string>> SYMTAB;
    vector<pair<string, string>> LITTAB;
    fstream intermediateCode;
    fstream symbolTable;
    fstream literalTable;
    fstream file;
    fstream output;
    
    public:
    Assembler () {
        intermediateCode.open("intermediate_code.txt", ios::in);
        if (!intermediateCode.is_open()) {
            cout<<"Error opening intermediate code file"<<endl;
        }

        symbolTable.open("symbol_table.txt", ios::in);
        if (!symbolTable.is_open()) {
            cout<<"Error opening symbol tabel file"<<endl;
        } else {
            string line;
            while (!symbolTable.eof()) {
                getline(symbolTable, line);
                string w = "";
                string key = "";
                string value = "";
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        key.assign(w);
                        w = "";
                    }
                }
                value = w;
                SYMTAB.push_back(pair<string, string> (key, value));
            }
        }

        literalTable.open("literal_table.txt", ios::in);
        if (!literalTable.is_open()) {
            cout<<"Error opening literal tabel file"<<endl;
        } else {
            string line;
            while (!literalTable.eof()) {
                getline(literalTable, line);
                string w = "";
                string key = "";
                string value = "";
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        key.assign(w);
                        w = "";
                    }
                }
                value = w;
                LITTAB.push_back(pair<string, string> (key, value));
            }
        }

        output.open("output.txt", ios::out);
        if (!output.is_open()) {
            cout<<"Error opening output file"<<endl;
        }
    }

    void passTwo () {
        string line = "";
        vector<string> words;
        if (intermediateCode.is_open()) {
            while (!intermediateCode.eof()) {
                getline(intermediateCode, line);
                string w = "";
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        words.push_back(w);
                        w = "";
                    }
                }
                words.push_back(w);

                if (words[1].at(1) == 'I' && words[1].at(2) == 'S') {
                    if (words.size() == 2) {
                        output<<words[0]<<" "<<words[1].at(4)<<endl;
                    } else if (words.size() == 3) {
                        int index = int(words[2].at(3) - '0');
                        output<<words[0]<<" "<<words[1].at(4)<<" "<<SYMTAB[index - 1].second<<endl;
                    } else {
                        if (words[3].at(1) == 'S') {
                            int index = int(words[3].at(3) - '0');
                            output<<words[0]<<" "<<words[1].at(4)<<" "<<words[2].at(1)<<" "<<SYMTAB[index - 1].second<<endl;
                        } else if (words[3].at(1) == 'L') {
                            int index = int(words[3].at(3) - '0');
                            output<<words[0]<<" "<<words[1].at(4)<<" "<<words[2].at(1)<<" "<<LITTAB[index - 1].second<<endl;
                        } else {
                            output<<words[0]<<" "<<words[1].at(4)<<" "<<words[2].at(1)<<" "<<words[3].at(1);
                        }
                    }
                }
                words.clear();
            }

        }
    }
};

int main() {
    Assembler a;
    a.passTwo();
    return 0;
}