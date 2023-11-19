#include <bits/stdc++.h>
using namespace std;

class PageReplacement {
    int n;
    int *frames;

    public:
    vector<int> pages;
    PageReplacement (int n) {
        this->n = n;
        frames = new int[n];
        for (int i = 0; i < n; i++) {
            frames[i] = -1;
        }
    }

    bool searchFrame(int page) {
        for (int i = 0; i < n; i++) {
            if (frames[i] == page) {
                return true;
            }
        }
        return false;
    }

    void clearFrame() {
        for (int i = 0; i < n; i++) {
            frames[i] = -1;
        }
    }

    void FIFO() {
        int index = 0;
        int hitCount = 0;
        for (int i = 0; i < pages.size(); i++) {
            if (frames[index] == -1) {
                frames[index] = pages[i];
                index++;
                index %= n;
                continue;
            }

            if (searchFrame(pages[i])) {
                hitCount++;
            } else {
                frames[index] = pages[i];
                index++;
            }
            index %= n;
        }
        cout<<"Hit Count: "<<hitCount<<endl;
        cout<<"Miss Count: "<<pages.size() - hitCount<<endl;
    }

    void LRU() {
        int index = 0;
        int hitCount = 0;
        unordered_map<int, int> recent;
        for (int i = 0; i < pages.size(); i++) {
            recent[pages[i]] = i;
            if (frames[index] == -1) {
                frames[index] = pages[i];
                index++;
                continue;
            }

            if (searchFrame(pages[i])) {
                hitCount++;
            } else {
                int min = pages.size();
                for (int j = 0; j < n; j++) {
                    if (recent[frames[j]] < min) {
                        min = recent[frames[j]];
                        index = j;
                    }
                }
                frames[index] = pages[i];
            }
        }
        cout<<"Hit Count: "<<hitCount<<endl;
        cout<<"Miss Count: "<<pages.size() - hitCount<<endl;
    }

    void opatimal() {
        int index = 0;
        int hitCount = 0;
        for (int i = 0; i < pages.size(); i++) {
            if (frames[index] == -1) {
                frames[index] = pages[i];
                index++;
                continue;
            }

            if (searchFrame(pages[i])) {
                hitCount++;
            } else {
                vector<int> next;
                for (int j = 0; j < n; j++) {
                    bool found = false;
                    for (int k = i + 1; k < pages.size(); k++) {
                        if (pages[k] == frames[j]) {
                            found = true;
                            next.push_back(k);
                            break;
                        }
                    }
                    
                    if (!found) {
                        next.push_back(pages.size());
                    }
                }

                int max = -1;
                for (int j = 0; j < n; j++) {
                    if (next[j] > max) {
                        max = next[j];
                        index = j;
                    }
                }
                frames[index] = pages[i];
            }
        }
        cout<<"Hit Count: "<<hitCount<<endl;
        cout<<"Miss Count: "<<pages.size() - hitCount<<endl;
    }
};

int main() {
    PageReplacement pr(3);
    pr.pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 3};
    cout<<"FIFO"<<endl;
    pr.FIFO();
    pr.clearFrame();
    cout<<"LRU"<<endl;
    pr.LRU();
    pr.clearFrame();
    cout<<"Optimal"<<endl;
    pr.opatimal();
}