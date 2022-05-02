// My code from Spring Challenge 2022 - codingame.com
// Tiago de Souza Fernandes
// Legend League:
//     2th Brazil rank
//     292th World rank

#include <bits/stdc++.h>
#define ff first
#define ss second
#define ll long long
#define ld double
#define pb push_back
#define eb emplace_back
#define vi vector<int>

using namespace std;

const int INF = 0x3f3f3f3f;
const ld PI = acos(-1);

void zuera(int i) {
    if(i < 5){
        cout << " " << "desenrola";
    }else if(i < 10){
        cout << " " << "bate";
    }else if(i < 15) {
        cout << " " << "joga de ladinho";
    }
}

#define vp vector<point>
typedef int cod;
bool eq(cod a, cod b){ return (a==b); }
struct point{
    cod x, y;
    int id;
    point(cod x=0, cod y=0): x(x), y(y){}


    point operator+(const point &o) const{
        return {x+o.x, y+o.y};
    }
    point operator-(const point &o) const{
        return {x-o.x, y-o.y};
    }
    point operator*(cod t) const{
        return {x*t, y*t};
    }
    point operator/(cod t) const{
        return {x/t, y/t};
    }
    cod operator*(const point &o) const{ // dot
        return x * o.x + y * o.y;
    }
    cod operator^(const point &o) const{ // cross
        return x * o.y - y * o.x;
    }
    bool operator<(const point &o) const{
        if(!eq(x, o.x)) return x < o.x;
        return y < o.y;
    }
    bool operator==(const point &o) const{
        return eq(x, o.x) and eq(y, o.y);
    }

    void show(){
        cout << x << " " << y;
    }
};

int norm(point a) {
    return sqrt(a * a);
}

point rotcw(point p, ld a){
    a = PI*a/180; // graus
    return point((p.x*cos(a)-p.y*sin(a)), (p.y*cos(a)+p.x*sin(a)));
}

point forca_mod(point p, ld m){
    ld cm = norm(p);
    if(cm<0) return point();
    return point(p.x*m/cm,p.y*m/cm);
}

int dist(point a, point b){
    point c = (a - b);
    return sqrt(c * c);
}

struct entity{
    int id; // Unique identifier
    int type; // 0=monster, 1=your hero, 2=opponent hero
    point pos; // Position of this entity
    int shield_life; // Count down until shield spell fades
    int is_controlled; // Equals 1 when this entity is under a control spell
    int health; // Remaining health of this monster
    point v; // Trajectory of this monster
    int near_base; // 0=monster with no target yet, 1=monster targeting a base
    int threat_for; // Given this monster's trajectory, is it a threat to 1=your base, 2=your opponent's base, 0=neither
};

point find_perfect_place(entity m, point pos, point enemy_base) {
    point mf = m.pos + m.v;
    point mfb = enemy_base - mf;
    mfb = forca_mod(mfb, 1200);
    point ans = mf + mfb;
    cerr << "ans = " << ans.x << " " << ans.y << endl;
    if (dist(ans, pos) > 1000 or dist(mf, enemy_base) > 6650 or mf.y > 8900 or mf.y < 100) {
        return point(INF, INF);
    } else {
        return ans;
    }
}

bool attack = false;
bool spell = false;
int wind_state = -INF;
int walk_state = -INF;

set<int> idx;

int main()
{
    point base; // The corner of the map representing your base
    cin >> base.x >> base.y; cin.ignore();
    int heroes_per_player; // Always 3
    cin >> heroes_per_player; cin.ignore();

    point enemy_base = base - point(17630, 9000);
    enemy_base.x = abs(enemy_base.x);
    enemy_base.y = abs(enemy_base.y);

    int dir = (base.x == 0 ? 1:-1);
    vp def(heroes_per_player);
    def[0] = base + point(8000, 6000) * dir;
    def[1] = base + point(4000, 7000) * dir;
    def[2] = base + point(7000, 1000) * dir;

    // game loop
    int turn = 0;
    while (1) {
        int mana[2];
        for (int i = 0; i < 2; i++) {
            int health; // Each player's base health
            int manax; // Spend ten mana to cast a spell
            cin >> health >> manax; cin.ignore();
            mana[i] = manax;
        }

        int entity_count;
        cin >> entity_count; cin.ignore();
        vector<entity> heroes, m, enemies;
        for (int i = 0; i < entity_count; i++) {
            entity e; cin >> e.id >> e.type >> e.pos.x >> e.pos.y >> e.shield_life >> e.is_controlled >> e.health >> e.v.x >> e.v.y >> e.near_base >> e.threat_for; cin.ignore();
            if (e.type == 0) m.pb(e);
            if (e.type == 1) heroes.pb(e);
            if (e.type == 2) enemies.pb(e);
        }
        int m_count = m.size();
        int heroes_count = heroes.size();
        int enemies_count = enemies.size();


        if (mana[0] > 160) {
            attack = true;
            def[0] = def[1] = base + point(12000, 8400) * dir;
            def[2] = base + point(2000, 2000) * dir;
        }

        for (int i = 0; i < heroes_per_player; i++) {

            sort(m.begin(), m.end(), [&] (entity &m1, entity &m2) -> bool{
                if (i == 2) {
                    return dist(m1.pos, base) < dist(m2.pos, base);
                } else {
                    return dist(m1.pos, def[i]) < dist(m2.pos, def[i]);
                }
            });

            int use_spell = -1;
            for(auto mi: m) {
                if (
                    !mi.shield_life and
                    dist(mi.pos, heroes[i].pos) < 2200 and
                    !idx.count(mi.id) and
                    (i!=spell) and
                    mi.threat_for != 2 and
                    dist(mi.pos, enemy_base) > dist(heroes[i].pos, enemy_base) + 400
                ) {
                    use_spell = mi.id;
                }
            }


            if(!attack or i==2) {
                if (i == 2) { // defend
                    if (m.empty() or dist(m[0].pos, base) > 8000) {
                        cout << "MOVE "; def[i].show();
                    }
                    else if(dist(m[0].pos, base) < 800 and m[0].shield_life==0 and dist(m[0].pos, heroes[i].pos) < 1280 and mana[0] > 30) {
                        cout << "SPELL WIND "; enemy_base.show();
                    }
                    else {
                        cout << "MOVE "; m[0].pos.show();
                    }
                } else { // attack
                    if (m.empty() or dist(m[0].pos, heroes[i].pos) > 3000) {
                        cout << "MOVE "; def[i].show();
                    } else {
                        cout << "MOVE "; m[0].pos.show();
                    }
                }
            } else { // attack

                if (walk_state == turn-1) {
                    cerr << "walk" << endl;
                    if (i == 0) cout << "WAIT";
                    else {
                        cout << "SPELL WIND "; (enemy_base - point(200, 200)*dir).show();
                        wind_state = turn;
                        walk_state = 0;
                    }
                }
                else if (wind_state == turn-1) {
                    cerr << "wind" << endl;
                    cout << "SPELL WIND "; (enemy_base - point(200, 200)*dir).show();
                    if (i == 1) wind_state = 0;
                }
                else if(use_spell != -1) {
                    cout << "SPELL CONTROL " << use_spell << " "; (def[i] - point(800, 0) * dir).show();
                    idx.insert(use_spell);
                }
                else if(
                    !m.empty() and
                    find_perfect_place(m[0], heroes[i].pos, enemy_base).x != INF and
                    dist(heroes[0].pos, heroes[1].pos) < 50
                ){
                    cout << "MOVE "; find_perfect_place(m[0], heroes[i].pos, enemy_base).show();
                    if (i == 1) walk_state = turn;
                }
                else {
                    cout << "MOVE "; def[i].show();
                }
                
            }

            zuera(turn); cout << endl;
        }
        spell = ! spell;
        turn ++;
    }
}
