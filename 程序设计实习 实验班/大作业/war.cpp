// -----------------------------
// by yinjinrun 2300012929
// create date: 2024/2/23 16:12
// -----------------------------

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

using pii = pair < int, int >;

int R, K, T;
pii curTime;

// ----------------------

class event {
public:
    string info;
    pii time;
    int place, tpId, flag;
};

bool operator < (event a, event b) {
    if(a.time != b.time) return a.time < b.time;
    if(a.place != b.place) return a.place < b.place;
    if(a.tpId != b.tpId) return a.tpId < b.tpId;
    if(a.flag != b.flag) return a.flag < b.flag;
    return 0;
}

vector < event > allEvents;
void takePlace(string val, int place, int tpId, int flag) { 
    allEvents.emplace_back((event) { val, curTime, place, tpId, flag}); 
}

// ----------------------

class city {
    int currentFlag; // red : 0， blue : 1
    int id, restElements, lstWinner; // lstWinner :{-1 : no, 0 : red, 1 : blue}
};

city cities[50];
int restElements[2], haveEnvader[2];

// ----------------------

class weapon {
public:
    int tp;
};

class sword : public weapon {
public :
    int attack;
    sword(int attack) : attack(attack * 0.2) { tp = 0; }
    void fight() { attack *= 0.8; }
    bool checkStatus() { return attack > 0; }
};

class arrow : public weapon {
public:
    int usdtime, attack;
    arrow() : attack(R), usdtime(3) { tp = 1; }
    bool checkStatus() { return usdtime > 0; }
};

class bomb : public weapon {
public:
    bool status;
    bomb() : status(true) { tp = 2; }
    bool checkStatus() { return true; }
};

// ----------------------

int eleDragon, eleNinja, eleIceman, eleLion, eleWolf;
int atkDragon, atkNinja, atkIceman, atkLion, atkWolf;

class warrior {
public:
    int flag, element, attack, id;
};

class dragon : public warrior {
public:
    string name = "dragon";
    
};

// ----------------------
int main() {
    return 0;
}