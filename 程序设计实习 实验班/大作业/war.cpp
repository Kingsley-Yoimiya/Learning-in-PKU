// -----------------------------
// by yinjinrun 2300012929
// create date: 2024/2/23 16:12
// -----------------------------

#define fi first
#define se second
#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)
#define eb emplace_back
#define ep emplace

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

using pii = pair < int, int >;

int m, n, R, K, T;

// ----------------
// proceding status
pii curTime;
int curCity;
// ----------------

string fixt(int t, string v) { while(v.size() < t) v = "0" + v; return v; }

string getTime() {
    return fixt(3, to_string(curTime.fi)) + ":" + fixt(2, to_string(curTime.se));
}

string getFlag(int flag) {
    return flag == 0 ? "red" : "blue";
}

int nxtCity(int flag, int place) {
    if(flag == 0) return place + 1; 
    return place - 1;
}

// ----------------------

class _eventRecoder{
private:
    class event {
    public:
        string info;
        pii time;
        int place, tpId, flag;
    };

    friend bool operator < (event a, event b) {
        if(a.time != b.time) return a.time < b.time;
        if(a.place != b.place) return a.place < b.place;
        if(a.tpId != b.tpId) return a.tpId < b.tpId;
        if(a.flag != b.flag) return a.flag < b.flag;
        return 0;
    }
    vector < event > allEvents;
public:
    void takePlace(string val, int tpId, int flag) { 
        allEvents.emplace_back((event) { val, curTime, curCity, tpId, flag}); 
    }
    void outputall() {
        sort(allEvents.begin(), allEvents.end());
        for(auto v : allEvents) {
            cout << v.info << endl;
        }
    }
} eventRecoder;

// ----------------------

class city {
    int currentFlag; // red : 0， blue : 1
    int id, restElements, lstWinner; // lstWinner :{-1 : no, 0 : red, 1 : blue}
};

city cities[50];
int restElements[2], haveEnvader[2];

// ----------------------

class weaponSystem {
protected:
    class weapon {
    public:
        int tp;
        weapon() { tp = -1; }
        bool checkStatus();
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
public:
    weaponSystem() { wp[0] = wp[1] = wp[2] = weapon(); }
    void getWeapon(int tp, int attack) {
        if(wp[tp].tp != -1) return;
        if(tp == 0) wp[tp] = sword(attack);
        if(tp == 1) wp[tp] = arrow();
        if(tp == 2) wp[tp] = bomb();
    }
    void kill_getWeapon(int t, weapon x) {
        if(wp[t].tp != -1) wp[t] = x;
    }
    void checkWeapon() {
        for(int i = 0; i < 3; i++) {
            if(wp[i].tp == -1) continue;
            if(!wp[i].checkStatus()) wp[i] = weapon();
        }
    }
private:
    weapon wp[3];
};

// ----------------------

int eleDragon, eleNinja, eleIceman, eleLion, eleWolf;
int atkDragon, atkNinja, atkIceman, atkLion, atkWolf;

class warrior {
protected:
    int flag, element, attack, id;
    string name;
    weaponSystem weapons;
    
    void setupWeapon(){};
    void warriorInit(string info = "") {
        stringstream out;
        out << getTime() << " " << getFlag(flag) << " " << name << " " << id << " born";
        eventRecoder.takePlace(out.str() + info, 1, flag);
    }
public:
    warrior() { 
        name = "fuck"; weapons = weaponSystem(); 
        attack = id = flag = element = -1;
        // not a warrior, no warriorInit;
    }
    string getname() { return name; }
    void walk() {}
    bool checkLoyalty() { return true; }
    bool exist() { return flag != -1; }
    void reach(int city) {
        if(city == 0 || city == n + 1) {
            // event 12 : reach end;
        }
        // more ....
    }
};

class dragon : public warrior {
private:
    double morale;
    void setupWeapon() {
        weapons.getWeapon(id % 3, attack);
    }
public:
    dragon(int fl, int tid) {
        flag = fl;
        id = tid;
        element = eleDragon;
        attack = atkDragon;
        name = "dragon";
        morale = 1. * restElements[flag] / element;
        stringstream out;
        out << endl << "Its morale is " << setprecision(2) << morale << endl;
        warriorInit(out.str());
        setupWeapon();
    }
    void moraleUp(bool win) { if(win) morale += 0.2; else morale -= 0.2; }
    void cheerUp() { 
        if(morale > 0.8) {
            stringstream out;
            out << getTime() << " " << getFlag(flag) << " dragon " << id << " yelled in city " << curCity;
            eventRecoder.takePlace(out.str(), 9, flag);
        }
    }
};

class ninja :  public warrior {
private:
    void setupWeapon() {
        weapons.getWeapon(id % 3, attack);
        weapons.getWeapon((id + 1) % 3, attack);
    }
public:
    ninja(int fl, int tid) {
        flag = fl;
        id = tid;
        element = eleNinja;
        attack = atkNinja;
        name = "ninja";
        warriorInit();
        setupWeapon();
    }
};

class iceman : public warrior {
private:
    int takeStep;
    void setupWeapon() {
        weapons.getWeapon(id % 3, attack);
    }
public:
    void walk() {
        takeStep++;
        if(takeStep % 2 == 0) element -= 9, attack += 20;
        if(element <= 0) element = 1; 
    }
    iceman(int fl, int tid) {
        flag = fl;
        id = tid;
        element = eleIceman;
        attack = atkIceman;
        name = "iceman";
        takeStep = 0;
        warriorInit();
        setupWeapon();
    }
};

class lion :   public warrior {
private:
    int loyalty;
    // sorry, no weapon.
public:
    lion(int fl, int tid) {
        flag = fl;
        id = tid;
        element = eleLion;
        attack = atkLion;
        name = "lion";
        loyalty = restElements[flag];
        stringstream out;
        out << endl << "Its loyalty is " << loyalty << endl;
        warriorInit(out.str());
        setupWeapon();
    }
    bool checkLoyalty() { return loyalty > 0; }
    void loyaltyDown() { loyalty -= K; }
};

class wolf :   public warrior {
// sorry, no weapon.
public:
    wolf(int fl, int tid) {
        flag = fl;
        id = tid;
        element = eleWolf;
        attack = atkWolf;
        name = "wolf";
        warriorInit();
        setupWeapon();
    }
};

const int tpCreate[2][5] = {
    { 2, 3, 4, 1, 0 },
    { 3, 0, 1, 2, 4 }
};
int curID[2], curP[2];

warrior createWarrior(int fl) {
    int tid = curID[fl] + 1, tp = tpCreate[fl][curP[fl] % 5];
    if(tp == 0) return eleDragon <= restElements[fl] ? 
                        restElements[fl] -= eleDragon, curID[fl]++, curP[fl]++, dragon(fl, tid) :
                        warrior();
    if(tp == 1) return eleNinja <= restElements[fl] ? 
                        restElements[fl] -= eleNinja, curID[fl]++, curP[fl]++, ninja(fl, tid) :
                        warrior();
    if(tp == 2) return eleIceman <= restElements[fl] ? 
                        restElements[fl] -= eleIceman, curID[fl]++, curP[fl]++, iceman(fl, tid) :
                        warrior();
    if(tp == 3) return eleLion <= restElements[fl] ? 
                        restElements[fl] -= eleLion, curID[fl]++, curP[fl]++, lion(fl, tid) :
                        warrior();
    if(tp == 4) return eleWolf <= restElements[fl] ? 
                        restElements[fl] -= eleWolf, curID[fl]++, curP[fl]++, wolf(fl, tid) :
                        warrior();
    return warrior(); // impossible.
}

warrior wCity[2][30], tCity[2][30];

void resetAll() {
    curTime = {0, 0};
    curCity = 0;
    restElements[0] = restElements[1] = m;
    haveEnvader[0] = haveEnvader[1] = 0;
    eventRecoder = _eventRecoder();
    rep(fl, 0, 1) rep(i, 1, 22) wCity[fl][i] = warrior();
    // more ...
}

// ----------------------
int main() {
    int testcases;
    cin >> testcases;
    rep(tcase, 1, testcases) {
        cout << "Case " << tcase << ":" << endl;
        cin >> m >> n >> R >> K >> T;
        cin >> eleDragon >> eleNinja >> eleIceman >> eleLion >> eleWolf;
        cin >> atkDragon >> atkNinja >> atkIceman >> atkLion >> atkWolf;
        resetAll();
        rep(hours, 0, T) {
            curTime.fi = hours;

            // 1. create warrior
            curTime.se = 0;
            curCity = 0;
            wCity[0][0] = createWarrior(0);
            curCity = n + 1;
            wCity[1][n + 1] = createWarrior(1);

            // 2. check lions loyalty
            curTime.se = 5;
            rep(fl, 0, 1) 
                rep(i, 0, n + 1) if(wCity[fl][i].getname() == "lion") {
                    curCity = i;
                    if(!wCity[fl][i].checkLoyalty()) {
                        stringstream out;
                        out << getTime() << " " << getFlag(fl) << " lion " << i << " ran away";
                        eventRecoder.takePlace(out.str(), 2, fl);
                        wCity[fl][i] = warrior();
                    }
                }

            // 3. march !
            curTime.se = 10;
            rep(fl, 0, 1) rep(i, 0, n + 1) tCity[fl][i] = warrior();
            rep(fl, 0, 1) rep(i, 0, n + 1) if(wCity[fl][i].exist()) {
                curCity = i;
                wCity[fl][i].walk();
                wCity[fl][i].reach(nxtCity(fl, i));
                tCity[fl][nxtCity(fl, i)] = wCity[fl][i];
            }
            if(tCity[0][n + 1].exist()) haveEnvader[1]++, tCity[0][n + 1] = warrior();
            if(tCity[1][0].exist()) haveEnvader[0]++, tCity[1][0] = warrior();
            rep(fl, 0, 1) rep(i, 0, n + 1) wCity[fl][i] = tCity[fl][i];

        }
        eventRecoder.outputall();
    }
    return 0;
}