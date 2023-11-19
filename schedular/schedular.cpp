#include <bits/stdc++.h>
using namespace std;

class Job {
    int id;
    string name;
    int burstTime;
    int arrivalTime;
    int waitingTime;
    int turnAroundTime;
    int completionTime;

public:
    Job() {
        id = 0;
        name = "";
        burstTime = 0;
        waitingTime = 0;
        turnAroundTime = 0;
        arrivalTime = 0;
        completionTime = 0;
    }

    Job(int id, string name, int arrivalTime, int burstTime) {
        this->id = id;
        this->name = name;
        this->burstTime = burstTime;
        this->arrivalTime = arrivalTime;
        waitingTime = 0;
        turnAroundTime = 0;
        completionTime = 0;
    }

    friend class Schedular;
};

class Schedular {
    int n;

    public:
    Job *processes;
    vector<Job> readyQueue;
    Schedular(int n) {
        this->n = n;
        processes = new Job[n];
    }

    void input() {
        int bt, at;
        for (int i = 0; i < n; i++) {
            cout<<"Enter arrival time and burst time for process "<<(i)<<endl;
            cin>>at>>bt;
            Job j(i, "P" + to_string(i), at, bt);
            processes[i] = j;
        }
    }

     void display() {
         cout<<"id\tname\tat\tbt\tct\ttat\twt"<<endl;
         for (int i = 0; i < n; i++) {
             cout<<processes[i].id<<"\t"<<processes[i].name<<"\t"<<processes[i].arrivalTime<<"\t"<<processes[i].burstTime<<"\t"<<processes[i].completionTime<<"\t"<<processes[i].turnAroundTime<<"\t"<<processes[i].waitingTime<<endl;
         }
     }

    void FCFS() {
        int totalTime = 0;
        int currentTime = 0;
        int processCompleted = 0;

        sort(processes, processes + n, [](const Job &lhs, const Job &rhs) {
            return lhs.arrivalTime < rhs.arrivalTime;
        });

        for (int i = 0; i < n; i++) {
            totalTime += processes[i].burstTime;
        }

        while (currentTime <= totalTime) {
            if (processCompleted < n && processes[processCompleted].arrivalTime <= currentTime) {
                readyQueue.push_back(processes[processCompleted]);
                processCompleted++;
            }

            if (readyQueue.size() > 0) {
                readyQueue[0].burstTime--;

                if (readyQueue[0].burstTime == 0) {
                    for (int i = 0; i < n; i++) {
                        if (processes[i].id == readyQueue[0].id) {
                            processes[i].completionTime = currentTime + 1;
                            processes[i].turnAroundTime = processes[i].completionTime - processes[i].arrivalTime;
                            processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
                        }
                    }
                    readyQueue.erase(readyQueue.begin());
                }
            }
            currentTime++;
        }
        display();
    }
    
    void SJF() {
        int totalTime = 0;
        int currentTime = 0;
        int processCompleted = 0;
        
        sort(processes, processes + n, [] (const Job &lhs, const Job &rhs) {
            return lhs.arrivalTime < rhs.arrivalTime;
        });
        
        for (int i = 0; i < n; i++) {
            totalTime += processes[i].burstTime;
        }
        
        while (currentTime <= totalTime) {
            if (processCompleted < n && processes[processCompleted].arrivalTime <= currentTime) {
                readyQueue.push_back(processes[processCompleted]);
                processCompleted++;
            }
            
            sort(readyQueue.begin(), readyQueue.end(), [] (const Job &lhs, const Job &rhs) {
                return lhs.burstTime < rhs.burstTime;
            });
            
            if (readyQueue.size() > 0) {
                readyQueue[0].burstTime--;
                
                if (readyQueue[0].burstTime == 0) {
                    for (int i = 0; i < n; i++) {
                        if (processes[i].id == readyQueue[0].id) {
                            processes[i].completionTime = currentTime + 1;
                            processes[i].turnAroundTime = processes[i].completionTime - processes[i].arrivalTime;
                            processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
                        }
                    }
                    readyQueue.erase(readyQueue.begin());
                }
            }
            currentTime++;
        }
        display();
    }
    
    void priority() {
        int totalTime = 0;
        int currentTime = 0;
        int processCompleted = 0;
        int priority[n];
        
        cout<<"Enter priorities"<<endl;
        for (int i = 0; i < n; i++){
            cin>>priority[i];
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - i; j++) {
                if (priority[j] > priority[j + 1]) {
                    swap(priority[j], priority[j + 1]);
                    swap(processes[j], processes[j + 1]);
                }
            }
        }
        
        for (int i = 0; i < n; i++) {
            totalTime += processes[i].burstTime;
        }
        
        while(currentTime <= totalTime) {
            if (processCompleted < n) {
                readyQueue.push_back(processes[processCompleted]);
                processCompleted++;
            }
            
            if (readyQueue.size() > 0) {
                readyQueue[0].burstTime--;
                
                if (readyQueue[0].burstTime == 0) {
                    for (int i = 0; i < n; i++) {
                        if (processes[i].id == readyQueue[0].id) {
                            processes[i].completionTime = currentTime + 1;
                            processes[i].turnAroundTime = processes[i].completionTime - processes[i].arrivalTime;
                            processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
                        }
                    }
                    readyQueue.erase(readyQueue.begin());
                }
            }
            currentTime++;
        }
        display();
    }

    void RR() {
        int totalTime = 0;
        int currentTime = 0;
        int processCompleted = 0;
        int interval;
        int addedBurst = 0;
        Job addedJob;

        cout<<"Enter time interval: ";
        cin>>interval;

        for (int i = 0; i < n; i++) {
            totalTime += processes[i].burstTime;
        }

        sort(processes, processes + n, [] (const Job &lhs, const Job &rhs) {
            return lhs.arrivalTime < rhs.arrivalTime;
        });

        while (currentTime < totalTime) {
            while (processCompleted < n && processes[processCompleted].arrivalTime <= currentTime) {
                readyQueue.push_back(processes[processCompleted]);
                processCompleted++;
            }

            if (addedJob.burstTime > 0) {
                readyQueue.push_back(addedJob);
            }

            if (readyQueue.size() > 0) {
                addedJob = readyQueue[0];
                readyQueue.erase(readyQueue.begin());

                if (addedJob.burstTime > interval) {
                    addedJob.burstTime = addedJob.burstTime - interval;
                    addedBurst = interval;
                } else {
                    addedBurst = addedJob.burstTime;
                    addedJob.burstTime = 0;
                }

                if (addedJob.burstTime == 0) {
                    for (int i = 0; i < n; i++) {
                        if (processes[i].id == addedJob.id) {
                            processes[i].completionTime = currentTime + addedBurst;
                            processes[i].turnAroundTime = processes[i].completionTime - processes[i].arrivalTime;
                            processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
                        }
                    }
                }
            }
            currentTime += addedBurst;
        }
        display();
    }
};

int main() {
    int n;
    cout<<"Enter number of processes: ";
    cin>>n;
    Schedular s(n);
    s.input();
    s.FCFS();
    s.SJF();
    s.priority();
    s.RR();
}