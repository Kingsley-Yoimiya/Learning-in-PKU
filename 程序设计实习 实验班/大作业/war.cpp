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
#include <assert.h>

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
public:
    int currentFlag; // red : 0， blue : 1
    int restElements, lstWinner; // lstWinner :{-1 : no, 0 : red, 1 : blue}
    city() { currentFlag = lstWinner = -1; restElements = 0; }
    int getFirst(int id) { 
        if(currentFlag != -1) return currentFlag;
        return (id + 1) & 1;
    }
};

city cities[50];
int restElements[2], haveEnvader[2];

// ----------------------

class weaponSystem {
private:
    class weapon {
    public:
        int tp, attack;
        weapon() { tp = -1; }
        bool checkStatus() { return attack > 0; }
        void fight() {
            if(tp == 0) attack = attack * 0.8;
            else if(tp == 1) attack--;
        }
        int atk() { return tp == 0 ? attack : R; }
        string info() { 
            //cerr << "INFO : " << tp << " " <<endl;
            if(tp == -1) return "no";
            else if(tp == 0) return "sword(" + to_string(attack) + ")";
            else if(tp == 1) return "arrow(" + to_string(attack) + ")";
            else return "bomb";
        }
    };

    class sword : public weapon {
    public :
        sword(int attack_) { attack = attack_ * 0.2; tp = 0; }
        string info() { return "sword(" + to_string(attack) + ")"; }
    };

    class arrow : public weapon {
    public:
        arrow() { attack = 3, tp = 1; }
    };

    class bomb : public weapon {
    public:
        bomb() { tp = 2; attack = 1; }
    };
public:
    //void tryArrow(warrior self, warrior &enemy);
    weaponSystem() { }
    void getWeapon(int tp, int attack) { // inits weapon
        //if(wp[tp].tp != -1) return;
        if(tp == 1) tp = 2; else if(tp == 2) tp = 1;
        if(tp == 0) wp[tp] = sword(attack);
        if(tp == 1) wp[tp] = arrow();
        if(tp == 2) wp[tp] = bomb();
        //cerr << "!TTTT" << tp << " " << wp[tp].checkStatus() << endl;
    }
    void kill_getWeapon(int t, weapon &x) {
        if(!wp[t] -> checkStatus()) *wp[t] = x;
    }
    weapon * trans(int x) { return wp[x]; }
    void kill_getWeapons(weaponSystem t) {
        rep(i, 0, 2) kill_getWeapon(i, *t.trans(i));
    }
    void checkWeapon() {
        for(int i = 0; i < 3; i++) {
            if(wp[i] -> tp == -1) continue;
            if(!wp[i] -> checkStatus()) wp[i] = new weapon();
        }
    }
    
    int getSword() { return wp[0] -> atk(); }
    bool haveBomb() { return wp[2] -> checkStatus(); }
    string info() {
        vector < string > ret;
        if(wp[1] -> checkStatus()) ret.eb(wp[1] -> info());
        if(wp[2] -> checkStatus()) ret.eb(wp[2] -> info());
        if(wp[0] -> checkStatus()) ret.eb(wp[0] -> info());
        string ans;
        if(ret.size() == 0) ans = "no weapon";
        else {
            bool fl = 0;
            for(auto v : ret) {
                if(fl) ans += ",";
                fl = 1; ans += v;
            } 
        } return ans;
    }
    bool checkArrow() { return wp[2].checkStatus();}
    void useArrow() { wp[2].fight(); }    
    void fightSword() { wp[0].fight(); }
private:
    weapon *wp[3];
};

// ----------------------

int eleDragon, eleNinja, eleIceman, eleLion, eleWolf;
int atkDragon, atkNinja, atkIceman, atkLion, atkWolf;

class warrior {
protected:
    int flag, element, attack, id, taksStep;
    int loyalty;
    int takeStep;
    string name;
    weaponSystem weapons;
    double morale;

    //void setupWeapon(){};
    void warriorInit(string info = "") {
        stringstream out;
        out << getTime() << " " << getFullname() <<  " born";
        eventRecoder.takePlace(out.str() + info, 1, flag);
    }
public:
    int getid() { return id; }
    int getflag() { return flag; }
    warrior() { 
        name = "fuck"; weapons = weaponSystem(); 
        attack = id = flag = element = -1;
        // not a warrior, no warriorInit;
    }
    string getname() { return name; }
    string getFullname() { return getFlag(flag) + " " + name + " " + to_string(id); }
    string getStatus() { return to_string(element) + " elements and force " + to_string(attack); }
    
    void walk() {
        if(name != "iceman") return;
        takeStep++;
        if(takeStep % 2 == 0) element -= 9, attack += 20;
        if(element <= 0) element = 1; 
    }
    //bool checkLoyalty() { return true; }
    bool exist() { return flag != -1; }
    bool alive() { return element >= 0; }
    void addElement() { element += 8; }
    weaponSystem giveWeapon() {
        assert(!alive());
        return weapons;
    }
    string weapon_info() {
        return weapons.info();
    }
    void scanBody(weaponSystem t) {
        weapons.kill_getWeapons(t);
    }
    void reach(int city) {
        if(city == 0 || city == n + 1) {
            // event 12 : reach end;
            stringstream out;
            out << getTime() << " " << getFullname() << " " << 
            "reached " << getFlag(flag ^ 1) << " " << "headquarter with " << getStatus();
            eventRecoder.takePlace(out.str(), 12, flag); 
        } else {
            // event 3 : march
            stringstream out;
            out << getTime() << " " << getFullname() << " marched to city " << city <<
             " with " <<  getStatus();
            eventRecoder.takePlace(out.str(), 3, flag);
        }
        // more .... done!
    }
    void earnElements(int val) {
        restElements[flag] += val;
        stringstream out;
        out << getTime() << " " << getFullname()
        << " earned " << val << " elements for his headquarter";
        eventRecoder.takePlace(out.str(), 10, flag);
    }
    void beAttacked(int val, bool isArrow) {
        element -= val;
        if(element < 0 && !isArrow) {
            stringstream out;
            out << getTime() << " " << getFullname() << " was killed in city " << curCity;
            eventRecoder.takePlace(out.str(), 8, flag);
            element = 0; // half dead, remain to be checked.
        }
    }
    void tryArrow(warrior &enemy) {
        if(!weapons.checkArrow()) return;
        enemy.beAttacked(R, 1);
        // event 4 : shoot arrow.
        stringstream out;
        out << getTime() << " " << getFullname() << " shot";
        if(!enemy.alive()) 
            out << "and killed " << enemy.getFullname();
        eventRecoder.takePlace(out.str(), 4, getflag());
        weapons.useArrow();
    }
    int attackValue() {
        return attack + weapons.getSword();
    }
    int fightBackValue() {
        if(name == "ninja") return 0;
        return attack / 2 + weapons.getSword();
    }
    bool evaluate(int harm, warrior &enemy) {
        if(harm >= element) {
            if(weapons.haveBomb()) {
                // do something...
                stringstream out;
                out << getTime() << " " << getFullname() << " used a bomb and killed " << getFullname();
                eventRecoder.takePlace(out.str(), 5, flag);
                element = 0; enemy.beAttacked(1e6, 0);
            } return false;
        } return true;
    }

    void moraleUp(bool win) { if(win) morale += 0.2; else morale -= 0.2; }
    void cheerUp() { 
        if(morale > 0.8) {
            stringstream out;
            out << getTime() << " " << getFullname() << " yelled in city " << curCity;
            eventRecoder.takePlace(out.str(), 9, flag);
        }
    }

    bool checkLoyalty() { if(name != "lion") return true; return loyalty > 0; }
    void loyaltyDown() { loyalty -= K; }
    void cheers_lions(bool win) { if(!win) loyaltyDown(); }
    void cheers(bool win) {
        if(name == "dragon") { moraleUp(win); cheerUp(); }
        if(name == "lion") { cheers_lions(win); }
    }
    void cheerSword() { weapons.fightSword(); }
};

class dragon : public warrior {
private:
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
        out << endl << "Its morale is " << fixed << setprecision(2) << morale;
        warriorInit(out.str());
        setupWeapon();
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
    void setupWeapon() {
        weapons.getWeapon(id % 3, attack);
    }
public:
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
        out << endl << "Its loyalty is " << loyalty;
        warriorInit(out.str());
        //setupWeapon();
    }
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
        //setupWeapon();
    }
};

const int tpCreate[2][5] = {
    { 2, 3, 4, 1, 0 },
    { 3, 0, 1, 2, 4 }
};
int curID[2];

warrior createWarrior(int fl) {
    int tid = curID[fl] + 1, tp = tpCreate[fl][curID[fl] % 5];
    if(tp == 0) return eleDragon <= restElements[fl] ? 
                        restElements[fl] -= eleDragon, curID[fl]++, dragon(fl, tid) :
                        warrior();
    if(tp == 1) return eleNinja <= restElements[fl] ? 
                        restElements[fl] -= eleNinja, curID[fl]++,  ninja(fl, tid) :
                        warrior();
    if(tp == 2) return eleIceman <= restElements[fl] ? 
                        restElements[fl] -= eleIceman, curID[fl]++, iceman(fl, tid) :
                        warrior();
    if(tp == 3) return eleLion <= restElements[fl] ? 
                        restElements[fl] -= eleLion, curID[fl]++, lion(fl, tid) :
                        warrior();
    if(tp == 4) return eleWolf <= restElements[fl] ? 
                        restElements[fl] -= eleWolf, curID[fl]++, wolf(fl, tid) :
                        warrior();
    return warrior(); // impossible.
}

void tryFight(warrior &a, warrior &b) {
    int t = a.attackValue();
    if(b.evaluate(t, a)) {
        t = b.fightBackValue();
        a.evaluate(t, b);
    } // bomb or not?
}

void fightIt(warrior &a, warrior &b) {
    b.beAttacked(a.attackValue(), 0);
    stringstream out;
    out << getTime() << " " << a.getFullname() << " " << "attacked" << b.getFullname() << 
    " in city " << curCity << " with " << a.getStatus();
    eventRecoder.takePlace(out.str(), 6, a.getflag());
    if(b.alive()) {
        stringstream out1;
        out1 << getTime() << " " << b.getFullname() << " " << "fought back against" << a.getFullname() << " in city " << curCity;
        eventRecoder.takePlace(out1.str(), 7, b.getflag());

        a.beAttacked(b.fightBackValue(), 0);
    }
}

warrior pos[30], wCity[2][30], tCity[2][30];
bool willFight[30];

void resetAll() {
    curTime = pii(0, 0);
    curCity = 0;
    curID[0] = curID[1] = 0;
    rep(i, 0, 22) pos[i] = warrior();
    restElements[0] = restElements[1] = m;
    haveEnvader[0] = haveEnvader[1] = 0;
    eventRecoder = _eventRecoder();
    rep(fl, 0, 1) rep(i, 0, 22) wCity[fl][i] = warrior();
    rep(i, 1, n) cities[i] = city();
    // more ...
}

// ----------------------
int main() {
    freopen("data.in", "r", stdin);
    freopen("war.out", "w", stdout);
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
                curCity = nxtCity(fl, i); // notice!
                wCity[fl][i].walk();
                wCity[fl][i].reach(nxtCity(fl, i));
                tCity[fl][nxtCity(fl, i)] = wCity[fl][i];
            }
            if(tCity[0][n + 1].exist()) haveEnvader[1]++, pos[n + 1] = tCity[0][n + 1], tCity[0][n + 1] = warrior();
            if(tCity[1][0].exist()) haveEnvader[0]++, pos[0] = tCity[1][0], tCity[1][0] = warrior();
            rep(fl, 0, 1) rep(i, 0, n + 1) wCity[fl][i] = tCity[fl][i];

            // -------------------
            // check Envader
            bool endOfGame = 0;
            if(haveEnvader[0] >= 2 || haveEnvader[1] >= 2) {
                
                eventRecoder.outputall(); 
                eventRecoder = _eventRecoder();
            }
            if(haveEnvader[0] >= 2) {
                cout << getTime() << " red headquarter was taken" << endl;
                endOfGame = 1;
            }
            if(haveEnvader[1] >= 2) {
                cout << getTime() << " blue headquarter was taken" << endl;
                endOfGame = 1;
            }
            if(endOfGame){
                break;
            }
            // -------------------

            // 4. produce elements
            curTime.se = 20;
            rep(i, 1, n) cities[i].restElements += 10;

            // 5. Take elements
            curTime.se = 30;
            rep(i, 1, n) {
                curCity = i; willFight[i] = 0;
                if(wCity[0][i].exist() && wCity[1][i].exist()) {
                    willFight[i] = true;
                    continue;
                }
                rep(fl, 0, 1) if(wCity[fl][i].exist()) {
                    //restElements[fl] += cities[i].restElements;
                     wCity[fl][i].earnElements(cities[i].restElements); 
                    cities[i].restElements = 0;
                }
            }

            // 6. Shoot Arrow
            curTime.se = 35;
            rep(fl, 0, 1) rep(i, 1, n) {
                curCity = i;
                if(wCity[fl][i].exist() && wCity[fl][nxtCity(fl, i)].exist())
                    wCity[fl][i].tryArrow(wCity[fl][nxtCity(fl, i)]);
            }

            // 7. Evaluate Bomb
            curTime.se = 38;
            rep(i, 1, n) if(willFight[i]) {
                curCity = i;
                int first = cities[i].getFirst(i);
                tryFight(wCity[first][i], wCity[first ^ 1][i]);
            }

            // 8. Fight
            curTime.se = 40;
            rep(i, 1, n) if(willFight[i]) {
                curCity = i;
                int first = cities[i].getFirst(i);
                fightIt(wCity[first][i], wCity[first ^ 1][i]);
            }

            // 9. Scan the Area
            curTime.se = 40;
            rep(i, 1, n) if(willFight[i]) {
                curCity = i;
                // Change Flag
                rep(fl, 0, 1)
                    if(wCity[fl][i].alive() && !wCity[fl ^ 1][i].alive()) {
                        if(cities[i].lstWinner == fl) {
                            if(cities[i].currentFlag != fl) {
                                stringstream out; 
                                out << getTime() << " " << getFlag(fl) << " flag raised in city " << i;
                                eventRecoder.takePlace(out.str(), 11, fl);
                            }
                            cities[i].currentFlag = fl;
                            cities[i].lstWinner = fl;
                        } else {
                            cities[i].lstWinner = fl;
                        }
                    } 
                if(!wCity[0][i].alive() && !wCity[1][i].alive())
                    cities[i].lstWinner = -1;
                // ---------------

                // afterWar ...
                rep(fl, 0, 1) if(wCity[fl][i].alive()) {
                    wCity[fl][i].cheers(!wCity[fl ^ 1][i].alive());
                    wCity[fl][i].cheerSword();
                } 
                rep(fl, 0, 1) if(wCity[fl][i].alive() && !wCity[fl ^ 1][i].alive()) {
                    if(wCity[fl][i].getname() == "wolf") 
                        wCity[fl][i].scanBody(wCity[fl ^ 1][i].giveWeapon());
                } // cheers and get weapon
                
            }
            rep(fl, 1, n) rep(i, 1, n) if(!wCity[fl][i].alive()) wCity[fl][i] = warrior();

            // distribute elements
            per(i, n, 1) if(willFight[i] && wCity[0][i].exist() && !wCity[1][i].exist() && restElements[0] >= 8) {
                wCity[0][i].addElement();
                restElements[0] -= 8;
            }
            rep(i, 1, n) if(willFight[i] && wCity[1][i].exist() && !wCity[0][i].exist() && restElements[1] >= 8) {
                wCity[1][i].addElement();
                restElements[1] -= 8;
            }
            // get elements
            rep(i, 1, n) if(willFight[i]) {
                curCity = i;
                if(wCity[0][i].exist() && wCity[1][i].exist()) continue;
                rep(fl, 0, 1) if(wCity[fl][i].exist()) {
                    wCity[fl][i].earnElements(cities[i].restElements);
                    cities[i].restElements = 0;
                }
            }

            eventRecoder.outputall(); 
            eventRecoder = _eventRecoder();

            // 10. Report elements
            curTime.se = 50;
            cout << getTime() << " " << restElements[0] << " elements in red headquarter" << endl;
            cout << getTime() << " " << restElements[1] << " elements in blue headquarter" << endl;
            
            // 11. Report all weapons
            curTime.se = 55;
            auto report = [&](warrior t) {
                if(t.exist()) {
                    cout << getTime() << " " << t.getFullname() << " has " << t.weapon_info() << endl;
                }
            };
            rep(i, 1, n) report(wCity[0][i]);
            report(pos[n + 1]);
            report(pos[0]);
            rep(i, 1, n) report(wCity[1][i]);
        }
    }
    return 0;
}