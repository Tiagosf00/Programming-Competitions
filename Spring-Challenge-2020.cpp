// My code from Spring Challenge 2020 - codingame.com
// Tiago de Souza Fernandes
// 1438TH world rank
// 10TH Brazil rank

#include <bits/stdc++.h>
#define ff first
#define ss second
#define ll long long
#define ld long double
#define pb push_back
#define eb emplace_back
#define mp make_pair
#define mt make_tuple
#define pii pair<int, int>
#define vi vector<int>
#define vl vector<ll>
#define vii vector<pii>
#define sws ios_base::sync_with_stdio(false);cin.tie(NULL)
#define teto(a, b) (a+b-1)/(b)
 
const int MAX = 300010;
const int MOD = 998244353;
const int INF = 0x3f3f3f3f;
const ll LLINF = 0x3f3f3f3f3f3f3f3f;
const ld EPS = 1e-4;
const ld PI = acosl(-1.0);

using namespace std;

struct pac{
    int x, y, id;
    string type;
    int cd;
};
vector<string> mapa;
bool pego[17][35];
vector<pair<bool, string>> activate(5, {false, ""});
int speed[5]={0,0,0,0,0};


int d[5]={1,0,-1,0,1};

int main()
{
    // MAPA

    int width, height;
    cin >> width >> height; cin.ignore();
    for(int i=0; i<height; i++){
        string row;
        getline(cin, row); // space " " is floor, pound "#" is wall
        mapa.pb(row);
    }

    // JOGO

    while(1){
        int myScore, opponentScore;
        int visiblePacCount;
        cin >> myScore >> opponentScore; cin.ignore();
        cin >> visiblePacCount; cin.ignore();

        vector<pac> pacs, enemy;

        // PACS

        for(int i=0; i<visiblePacCount; i++){
            int speedTurnsLeft, abilityCooldown;
            int pacId, x, y;
            bool mine;
            string typeId;
            cin >> pacId >> mine >> x >> y >> typeId >> speedTurnsLeft >> abilityCooldown; cin.ignore();

            pego[y][x]=true;

            if(mine) pacs.pb({x, y, pacId, typeId, abilityCooldown});
            else if(typeId!="DEAD"){ // ATACKS
                enemy.pb({x, y, pacId, typeId, abilityCooldown});
            }
            
        }
        int mypacs = pacs.size(); // number of pacs

        // PELLETS

        vii moves(mypacs);
        set<pii> bonus;
        bool pellets[17][35]; memset(pellets, false, sizeof(pellets));

        int visiblePelletCount;
        cin >> visiblePelletCount; cin.ignore();

        for(int i=0; i<visiblePelletCount; i++){
            int x, y, value;
            cin >> x >> y >> value; cin.ignore();
            if(value==10)
                bonus.insert(mp(y, x));
            pellets[y][x]=true;
        }

        // COUNT PELLEKS
        for(int i=0; i<mypacs; i++){
            int x=pacs[i].x, y=pacs[i].y;
            for(int i=0;x-i>=0;i++){
                if(mapa[y][x-i]=='#') break;
                if(!pellets[y][x-i]) pego[y][x-i]=true;
            }
            for(int i=0;y-i>=0;i++){
                if(mapa[y-i][x]=='#') break;
                if(!pellets[y-i][x]) pego[y-i][x]=true;
            }
            for(int i=0;x+i<width;i++){
                if(mapa[y][x+i]=='#') break;
                if(!pellets[y][x+i]) pego[y][x+i]=true;
            }
            for(int i=0;y+i<height;i++){
                if(mapa[y+i][x]=='#') break;
                if(!pellets[y+i][x]) pego[y+i][x]=true;
            }
        }

        // MOVE

        for(int k=0; k<mypacs; k++){

            pii s=mp(pacs[k].y, pacs[k].x); // source vertex

            queue<pii> q;
            int dist[17][35];
            for(int i=0;i<17;i++) for(int j=0;j<35;j++) dist[i][j]=INF;
            dist[s.ff][s.ss]=0;

            q.push(s);
            while(!q.empty()){
                pii v = q.front();
                q.pop();
                for(int i=0;i<4;i++){
                    if(mapa[(v.ff+d[i]+height)%height][(v.ss+d[i+1]+width)%width]=='#') continue;
                    if(dist[(v.ff+d[i]+height)%height][(v.ss+d[i+1]+width)%width] > dist[v.ff][v.ss]+1){
                        q.push(mp((v.ff+d[i]+height)%height, (v.ss+d[i+1]+width)%width));
                        dist[(v.ff+d[i]+height)%height][(v.ss+d[i+1]+width)%width] = dist[v.ff][v.ss]+1;
                    }
                }
            }

            int dmin=INF;
            pii aux;
            for(int j=0;j<height;j++)
                for(int i=0;i<width;i++){
                    int b = (bonus.find(mp(j, i))==bonus.end() ? 0:-10);
                    if(mapa[j][i]!='#' and !pego[j][i] and dist[j][i]+b<=dmin){
                        dmin = dist[j][i]+b;
                        aux = mp(j, i);
                    }
                }
            moves[k]=aux;
        }



        // COLLISION
        for(int i=0; i<mypacs; i++){
            for(int j=i+1; j<mypacs; j++){
                if(abs(pacs[i].y-pacs[j].y)+abs(pacs[i].x-pacs[j].x)<=2){
                    bool ind = rand()%2;
                    moves[(ind ? i:j)]=mp(pacs[(ind ? i:j)].y, pacs[(ind ? i:j)].x);
                }
            }
        }


        // ATTACKS
        for(auto e: enemy){
            for(int i=0;i<mypacs;i++){
                int dist = abs(e.y-pacs[i].y)+abs(e.x-pacs[i].x);
                if(dist<=4){
                    speed[i]=0;

                    // ATTACK
                    if(e.cd!=0 and e.type=="SCISSORS" and pacs[i].type=="ROCK" and dist<=3){
                        moves[i]=mp(e.y, e.x);
                        speed[i]=4;
                    }
                    else if(e.cd!=0 and e.type=="PAPER" and pacs[i].type=="SCISSORS" and dist<=3){
                        moves[i]=mp(e.y, e.x);
                        speed[i]=4;
                    }
                    else if(e.cd!=0 and e.type=="ROCK" and pacs[i].type=="PAPER" and dist<=3){
                        moves[i]=mp(e.y, e.x);
                        speed[i]=4;
                    }

                    // DEFEND
                    else if(e.type=="ROCK" and pacs[i].type=="SCISSORS"){
                        activate[i]=mp(true, "PAPER");
                    }
                    else if(e.type=="SCISSORS" and pacs[i].type=="PAPER"){
                        activate[i]=mp(true, "ROCK");
                    }
                    else if(e.type=="PAPER" and pacs[i].type=="ROCK"){
                        activate[i]=mp(true, "SCISSORS");
                    }

                    // SURPRISE
                    else if(e.type=="SCISSORS" and pacs[i].type=="SCISSORS" and dist<=2){
                        activate[i]=mp(true, "ROCK");
                        speed[i]=0;
                    }
                    else if(e.type=="PAPER" and pacs[i].type=="PAPER" and dist<=2){
                        activate[i]=mp(true, "SCISSORS");
                        speed[i]=0;
                    }
                    else if(e.type=="ROCK" and pacs[i].type=="ROCK" and dist<=2){
                        activate[i]=mp(true, "PAPER");
                        speed[i]=0;
                    }
                }
            }
        }




        // MOVES
        for(int i=0; i<mypacs; i++){
            if(i) cout << "|";
            if(activate[i].ff and pacs[i].cd==0){
                cout << "SWITCH " << pacs[i].id << " " << activate[i].ss;
                activate[i].ff=false;
            }
            else if(pacs[i].cd==0 and speed[i]>=4){
                cout << "SPEED " << pacs[i].id;
                speed[i]=0;
            }
            else{
                cout << "MOVE " << pacs[i].id << " ";
                cout << moves[i].ss << " " << moves[i].ff;
            }
            speed[i]++;
        }
        cout << endl;

    }
}