#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#ifdef _WIN32
#include <windows.h>
#endif
#include <queue>

using namespace std;
class map;
string rgb(int r, int g, int b, string str)
{
    r = max(min(r, 255), 0);
    g = max(min(g, 255), 0);
    b = max(min(b, 255), 0);
    return "\033[0;38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + str + "\033[m";
}
int X = 0;
int Y = 0;
int cX = 0;
int cY = 0;

double money = 0;
int quota = 1;
const int quotaMultiplier = 10;

int maxO2 = 100; //       d1  | maximum oxygen
int O2gen = 0; //         d2  | produce oxygen when not docked at surface
int scuba = 0; //         d3  | distance from surface oxy gen works, oxy gen decreases farther from surface however

double swindle = 1; //    d7  | lower quota
double light = 0; //      d8  | larger light
double glass = 3.5; //    d9  | farther sight
double faulty = 0.5; //   d10 | chance of detecting false treasures
double dish = 2.5; //     d11 | inspect scan radius
int bargain = 3; //       d12 | number of upgrade choices each island
int tolerance = 5; //     d13 | number of times pirates will let you dock before moving to the next location

int O2 = maxO2;

int attempts = 0;
int ct = tolerance;
int sub = 0; // 0 = docked, 1 = roaming, 2 = island
int issue = 0; // 0 = none, 1 = tile blocking way, 2 = enttiy blocking way, 3 = world edge

bool tutDesc = true;
bool tutScan = true;
bool tutGet = true;
bool tutDock = true;

double dist(int x1, int y1, int x2, int y2)
{
    int dx = x1 - x2;
    int dy = y1 - y2;
    return sqrt((dx * dx) + (dy * dy));
}

double luck = 0; //       d4  | liklihood of getting better treasure
double maxLuck = 0.5; //  d5  | highest treasure component proportional to number of components
double prosperity = 0; // d6  | more treasure

class tile // tiles are static.
{
public:
    string shape;
    bool checked = false;
    int state = -10;
    bool walkable = true;
    bool tr = false;
    tile(string h)
    {
        shape = h;
    }

    tile()
    {
        shape = "  ";
    }

    string getShape()
    {
        return shape;
    }
};

string state[20] = { "an unrecognizable ", "a pulverized ", "a shattered ", "a broken ", "a decayed ", "an eroded ", "a withered ", "an ancient ", "a rusty ", "an old ", "a chipped ", "a scratched ", "a dull ", "a ", "an ok ", "a decent ", "an unused ", "a brand-new ", "an intricate ", "a pristine " };
double stateMultiplier[20] = { 0.05, 0.08, 0.12, 0.15, 0.20, 0.25, 0.35, 0.50, 0.65, 0.80, 1.00, 1.20, 1.40, 1.65, 2.00, 2.50, 3.20, 4.00, 5.00, 6.50 };
string matStart[102] = { "Poly", "Bi", "Di", "Carb", "Carbo", "Lith", "Alum", "Iron", "Lead", "Photo", "Beryl", "Gall", "Cob", "Moly", "Tung", "Merc", "Rhod", "Osm", "Irid", "Tant", "Mecha", "Chemo", "Ferro", "Hema", "Fort", "Plat", "Pallad", "Dura", "Bio", "Aero", "Aerial", "Aqua", "Hydro", "Sea", "San", "Terra", "Tarra", "Vita", "Peren", "Oxy", "Hel", "Pyro", "Cryo", "Scor", "Nept", "Naut", "Keki", "Polo", "Ura", "Rad", "Franc", "F", "Commu", "Signal", "Mater", "Voca", "Vox", "Endo", "Sil", "Lum", "Lumin", "Aura", "Crim", "Lunar", "Manta", "Red", "Extra", "Trans", "Ultra", "Super", "Neo", "Reallyhard", "Thermo", "Tempo", "Tele", "Nan", "Trit", "Prom", "Quant", "Meteor", "Stellar", "Astral", "Exod", "Ender", "Tiber", "Krypto", "Crypto", "Naqu", "Anti", "Aether", "Nether", "End", "Vibra", "Thaum", "Orich", "Mith", "Myth", "Ada", "Unob", "Admin" };
double matStartValue[102] = { 1.00, 1.05, 1.10, 1.15, 1.20, 1.25, 1.30, 1.35, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 2.00, 2.15, 2.30, 2.45, 2.60, 2.75, 2.90, 3.10, 3.30, 3.50, 3.75, 4.00, 4.25, 4.50, 4.80, 5.10, 5.40, 5.70, 6.00, 6.40, 6.80, 7.20, 7.60, 8.00, 8.50, 9.00, 9.50, 10.0, 10.6, 11.2, 11.8, 12.5, 13.2, 14.0, 14.8, 15.6, 16.5, 17.5, 18.5, 19.5, 20.5, 21.5, 22.8, 24.1, 25.5, 27.0, 28.5, 30.0, 31.8, 33.6, 35.5, 37.5, 39.5, 42.0, 44.5, 47.0, 50.0, 53.0, 56.0, 60.0, 64.0, 68.0, 72.0, 77.0, 82.0, 87.0, 92.0, 98.0, 104.0, 110.0, 117.0, 125.0, 133.0, 142.0, 151.0, 160.0, 170.0, 181.0, 192.0, 204.0, 216.0, 228.0, 242.0, 256.0, 270.0, 285.0, 300.0, 320.0 };
string matMiddle[22] = { "a", "o", "l", "al", "in", "un", "de", "lith", "therm", "ston", "tai", "ver", "man", "eth", "uck", "ill", "ah", "synth", "bde", "istrat", "carn", "kill" };
double matMiddleBonus[22] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 4.0, 5.0, 6.5, 8.0, 10.0, 12.5, 15.0, 18.0, 22.0, 26.0, 31.0, 37.0, 43.0, 50.0, 60.0 };
string matEnd[40] = { "e", "n", "t", "on", "le", "ic", "il", "ia", "ing", "con", "icon", "alcum", "alt", "ril", "ten", "sten", "num", "ium", "nium", "lium", "ury", "osm", "ite", "tite", "nite", "cite", "cyte", "lite", "ine", "line", "tine", "tane", "ide", "er", "gen", "form", "dah", "mory", "vox", "matter" };
double matEndMultiplier[40] = { 1.00, 1.03, 1.06, 1.09, 1.12, 1.15, 1.18, 1.21, 1.24, 1.27, 1.30, 1.34, 1.38, 1.42, 1.46, 1.50, 1.55, 1.60, 1.65, 1.70, 1.75, 1.81, 1.87, 1.93, 2.00, 2.08, 2.16, 2.24, 2.33, 2.42, 2.52, 2.62, 2.73, 2.85, 2.97, 3.10, 3.24, 3.38, 3.54, 3.70 };
string type[33] = { " nail", " coin", " arrow head", " stick", " wire", " nugget", " ball", " bullet", " knife", " cup", " bowl", " arrow", " hunk", " spear", " pole", " shortsword", " gear", " axe", " shovel", " pickaxe", " hammer", " katana", " chain", " broadsword", " scythe", " ingot", " cannonball", " drill head", " beam", " plate", " greataxe", " helmet", " block" };
double typeExponent[33] = { 1.00, 1.04, 1.08, 1.12, 1.16, 1.20, 1.25, 1.30, 1.35, 1.40, 1.46, 1.52, 1.58, 1.64, 1.71, 1.78, 1.85, 1.92, 2.00, 2.08, 2.16, 2.25, 2.34, 2.43, 2.52, 2.61, 2.70, 2.79, 2.88, 2.98, 3.08, 3.18, 3.30 };

class treasure // treasures are randomly generated, with their values dependent on their names.
{
private:
    string name;
    double value;
public:
    int x, y;
    bool t = false;
    treasure(int _x, int _y)
    {
        setName();
        x = _x; y = _y;
    }

    treasure()
    {
        setName();
    }

    int brand()
    {
        return (rand() * RAND_MAX) + rand();
    }

    int chance(int c, double n)
    {
        int r = brand();
        int t = floor(pow(c, n));
        int out = floor(maxLuck * pow(r % t, 1 / n));
        if (out == 0)
            return 1;
        else return out;
    }

    void setName()
    {
        int st = brand() % chance(20, (4.0 - luck));
        int ms = brand() % chance(102, (4.0 - luck));
        int mm = brand() % chance(22, (4.0 - luck));
        int me = brand() % chance(40, (4.0 - luck));
        int ty = brand() % chance(33, (4.0 - luck));
        name = name + state[st];
        name = name + matStart[ms];
        if (rand() % 10 < 4)
            name = name + matMiddle[mm];
        else
            mm = 0;
        name = name + matEnd[me];
        name = name + type[ty];
        value = stateMultiplier[st] * pow((matStartValue[ms] + matMiddleBonus[mm]) * matEndMultiplier[me], typeExponent[ty]);
    }

    string getName()
    {
        return name;
    }

    double getValue()
    {
        return value;
    }
};

class entity // entities do not need to be static, and have unique actions when a game step occurs, when interacted with (inspected while overlapping with player, or, in the case that it is solid, ran into by the player), or when triggered (levers, buttons, pressure plates, etc. of the corresponding ID)
{
protected:
    int x, y, ID;
    bool solid = false;
    string shape;
public:
    entity(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    bool solidity()
    {
        return solid;
    }
    int getx()
    {
        return x;
    }
    int gety()
    {
        return y;
    }
    virtual int gettype()
    {
        return 0;
    }
    string getShape()
    {
        return shape;
    }
    int getID()
    {
        return ID;
    }
    virtual void start(map* m) {}
    virtual void step() {}
    virtual void interact() {}
    virtual void trigger() {}
};
class bubble : public entity {
    //private:
    //    int life;
public:
    bubble(int _x, int _y) : entity(_x, _y) { shape = "()"; };
    void step()
    {
        if (x == X && y == Y)
            if (O2 < maxO2 - 5)
            {
                O2 += 5;
            }
            else
                O2 = maxO2;
    }
    int gettype()
    {
        return 1;
    }
    void start(map* m) {}
};
class lever : public entity {
private:
    bool toggled = false;
    int r, g, b = 0;
public:
    lever(int _x, int _y, int _r, int _g, int _b, int _ID) : entity(_x, _y) { ID = _ID; shape = "\033[38;2;" + to_string(_r) + ";" + to_string(_g) + ";" + to_string(_b) + "m\\\\\033[m"; r = _r; g = _g; b = _b; } // rgb from 0 to 255 pls :)
    void interact();
    int gettype()
    {
        return 3;
    }
    void start(map* m) {}
};
class door : public entity {
private:
    string doorShape = "==";
    int r, g, b = 0;
public:
    door(int _x, int _y, int _r, int _g, int _b, int _ID) : entity(_x, _y) { ID = _ID; solid = true; shape = "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + doorShape + "\033[m"; r = _r; g = _g; b = _b; }
    void trigger();
    int gettype()
    {
        return 4;
    }
    void start(map* m);
};
vector<treasure> treasureList;

class map
{
private:
    bool active = false;
    int size;
    int seed = 0;
    vector<vector<tile>> main;
    vector<treasure> hidden;
public:
    int maxState = -10;
    vector<entity*> moving;
    vector<bool> trigger;
    map(int num, int s)
    {
        size = num;
        seed = s;
        main = vector<vector<tile>>(num, vector<tile>(num, tile()));
    }

    void trig()
    {
        trigger.push_back(false);
    }
    void swatch(int ID)
    {
        trigger[ID] = !trigger[ID];
    }
    vector<treasure> tlist()
    {
        return hidden;
    }
    void setTile(double c, int _x, int _y, tile t)
    {
        if (c < rand() / (double)RAND_MAX)
        {
            main[_x][_y] = t;
        }
    }

    void genCircle(int x, int y, int r, double ci, double co, tile t) // world, start x, start y, radius, inner chance, outer chance, tile
    {
        int px = x - r;
        int py = y - r;
        for (int i = 0; i < (2 * r) + 1; i++)
        {
            for (int f = 0; f < (2 * r) + 1; f++)
            {
                double d = sqrt(((px + i - x) * (px + i - x)) + ((py + f - y) * (py + f - y)));
                if (((px + i >= 0) && (py + f >= 0)) && ((px + i < size) && (py + f < size)))
                {
                    if (d <= r)
                    {
                        double cot = ((d * co) / r) + ((1 - (d / r)) * ci);
                        setTile(cot, (px + i), (py + f), t);
                    }
                }
            }
            py = y - r;
        }
    }

    void genPath(int nr, int xr, int jmp, int l, double ci, double co, tile t) // min radius, max radius, max jump distance, # of circles created, inner chance, outer chance, tile
    {
        int sx = rand() % size;
        int sy = rand() % size;
        for (int i = 0; i < l; i++)
        {
            if ((xr - nr) != 0)
            genCircle(sx, sy, (rand() % (xr - nr)) + nr, ci, co, t);
            sx = (sx + (rand() % ((2 * jmp) + 1)) - jmp) % size;
            sy = (sy + (rand() % ((2 * jmp) + 1)) - jmp) % size;
        }
    }

    bool tr(int x, int y) // is there a treasure at (x, y)?
    {
        for (treasure t : hidden)
            if (t.x == x && t.y == y) return true;
        return false;
    }

    void caveGen()
    {
        //srand(seed);
        //int seed = rand();
        //srand(seed);
        double irand = rand();
        double drand = irand / (double)RAND_MAX;
        for (int i = 0; i < (int)(pow(log2(size), 2.5 + drand)); i++)
        {
            tile t = tile("██");
            t.walkable = false;
            genPath(rand() % (int)max(0, log2(size) - 2), (rand() % (int)max(1, log2(size) - 1)) + (int)max(1, log2(size) - 1), rand() % (int)max(1, log2(size) - 1), (int)max(1, log2(size) - 1), 0, 1, t);
        }
        irand = rand();
        drand = irand / (double)RAND_MAX;
        //for (int i = 0; i < (int)(pow(log2(size), 2 + drand + prosperity)); i++)
        //{
        //    int rx = rand() % size;
        //    int ry = rand() % size;
        //    while (!main[rx][ry].walkable || tr(rx, ry))
        //    {
        //        rx = rand() % size;
        //        ry = rand() % size;
        //    }
        //    hidden.push_back(treasure(rx, ry));
        //}
        for (int i = 0; i < size; i++)
        {
            if (main[0][i].getShape() == "  ")
            {
                main[0][i] = tile("~~");
            }
        }
    }

    void caveGen2()
    {
        //srand(seed);
        //int seed = rand();
        //srand(seed);
        double irand = rand();
        double drand = irand / (double)RAND_MAX;
        for (int i = 0; i < (int)(pow(log2(size), 2.5 + drand)); i++)
        {
            tile t = tile("██");
            t.walkable = false;
            genPath(rand() % (int)max(0, log2(size) - 2), (rand() % (int)max(1, log2(size) - 1)) + (int)max(1, log2(size) - 1), rand() % (int)max(1, log2(size) - 1), (int)max(1, log2(size) - 1), 0, 1, t);
        }
        irand = rand();
        drand = irand / (double)RAND_MAX;
        //for (int i = 0; i < (int)(pow(log2(size), 2 + drand + prosperity)); i++)
        //{
        //    int rx = rand() % size;
        //    int ry = rand() % size;
        //    while (!main[rx][ry].walkable || tr(rx, ry))
        //    {
        //        rx = rand() % size;
        //        ry = rand() % size;
        //    }
        //    hidden.push_back(treasure(rx, ry));
        //}
        //for (int i = 0; i < size; i++)
        //{
        //    if (main[0][i].getShape() == "  ")
        //    {
        //        main[0][i] = tile("~~");
        //    }
        //}
    }
    class point {
    public:
        int x;
        int y;
        point(int _x, int _y)
        {
            x = _x; y = _y;
        }
    };
    void collapse(int x, int y, int ID)
    {
        if (x < 0 || x >= size || y < 0 || y >= size)
            return;
        if (main[x][y].state >= -1 || main[x][y].checked)
        {
            main[x][y].checked = true;
            return;
        }
        if (!main[x][y].walkable)
        {
            main[x][y].state = -1;
            main[x][y].checked = true;
            return;
        }
        //cout << ID << " at (" << x << ", " << y << ")" << endl;
        queue<point> q;
        q.push(point(x, y));
        main[x][y].state = ID;
        main[x][y].checked = true;
        while (q.size() > 0)
        {
            point p = q.front();
            q.pop();
            main[p.x][p.y].state = ID;
            int dx[] = { -1, 1, 0, 0 };
            int dy[] = { 0, 0, -1, 1 };
            for (int i = 0; i < 4; i++)
            {
                int rx = p.x + dx[i];
                int ry = p.y + dy[i];
                if (rx >= 0 && rx < size && ry >= 0 && ry < size)
                {
                    //cout << main[rx][ry].state << endl;
                    if (!main[rx][ry].walkable)
                        main[rx][ry].state = -1;
                    else if (main[rx][ry].state == -10 && !main[rx][ry].checked)
                    {
                        main[rx][ry].checked = true;
                        q.push(point(rx, ry));
                    }
                }
            }
        }
    }
    void setStates() //sets states of all tiles in this map. -10 is undefined, -3 is door, -2 is unrevealed wall, -1 is revealed wall, everything else is walkable space.
    {
        clearStates();
        int state = 0;
        for (int i = 0; i < moving.size(); i++)
        {
            if (moving[i]->gettype() == 4)
                main[moving[i]->getx()][moving[i]->gety()].state = -3;
        }
        for (int i = 0; i < size; i++)
            for (int f = 0; f < size; f++)
                if (!main[i][f].checked && main[i][f].state == -10)
                {
                    if (main[i][f].walkable)
                    {
                        collapse(i, f, state);
                        //cout << "(" << i << ", " << f << ") with " << state << endl;
                        state++;
                    }
                    else
                    {
                        main[i][f].state = -2;
                        main[i][f].checked = true;
                    }
                }
        maxState = state;
    }
    void fullGen()
    {
        caveGen();
        //for (treasure t : hidden)
        //    cout << t.getName() << ": " << t.getValue() << ", (" << t.x << ", " << t.y << ")" << endl;
    }

    void clearStates()
    {
        for (int i = 0; i < size; i++)
            for (int f = 0; f < size; f++)
            {
                main[i][f].state = -10;
                main[i][f].checked = false;
            }
    }

    void genDoor(int r, int g, int b, int ID)
    {
        setStates();
        int rx = 0;
        int ry = 0;
        int dr = rand() % 4;
        int dx[] = { -1, 1, 0, 0 };
        int dy[] = { 0, 0, -1, 1 };
        while (main[rx][ry].state != -1)
        {
            rx = rand() % size;
            ry = rand() % size;
        }
        //cout << "r: (" << rx << ", " << ry << "), with " << main[rx][ry].state << endl;
        for (int i = dr; i < 4 + dr; i++)
        {
            int nx = rx + dx[i % 4];
            int ny = ry + dy[i % 4];
            if (nx >= 0 && nx < size && ny >= 0 && ny < size)
            {
                //cout << "    r: (" << nx << ", " << ny << "), with " << main[nx][ny].state << endl;
                if (main[nx][ny].state > -1)
                {
                    dr = i % 4;
                    break;
                }
            }
        }
        int px = rx + dx[dr];
        int py = ry + dy[dr];
        while (px >= 0 && px < size && py >= 0 && py < size && main[px][py].state > -1)
        {
            door* nd = new door(px, py, r, g, b, ID);
            addEntity(nd);
            px = px + dx[dr];
            py = py + dy[dr];
        }
        //clearStates();
    }

    void depthsGen()
    {
        main = vector<vector<tile>>(size, vector<tile>(size, tile()));
        caveGen2();
        for (int i = 0; i < 99; i++)
            genPath(1, 5, 2, 5, 0, 1, tile());

        for (int i = 0; i < size; i++)
        {
            main[0][i] = tile();
            main[63][i] = tile();
        }
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        //setStates();
        //printStates();
        //clearStates();
        for (int i = 0; i < 15; i++)
            genDoor(r, g, b, 0);
        int rx = rand() % size;
        int ry = rand() % size;
        while (main[rx][ry].state != 0)
        {
            rx = rand() % size;
            ry = rand() % size;
        }
        lever* lr = new lever(rx, ry, r, g, b, 0);
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
        for (int i = 0; i < 10; i++)
            genDoor(r, g, b, 1);
        //setStates();
        //printStates();
        rx = rand() % size;
        ry = rand() % size;
        while (main[rx][ry].state != rand() % maxState)
        {
            rx = rand() % size;
            ry = rand() % size;
        }
        lever* lr2 = new lever(rx, ry, r, g, b, 1);
        addEntity(lr2);
        addEntity(lr);
        for (int i = 0; i < size / 2; i++)
        {
            while (!main[rx][ry].walkable || checkSolid(rx, ry))
            {
                rx = rand() % size;
                ry = rand() % size;
            }
            bubble* bk = new bubble(rx, ry);
            addEntity(bk);
            rx = rand() % size;
            ry = rand() % size;
        }
        for (int i = 0; i < 100; i++)
        {
            while (!main[rx][ry].walkable || checkSolid(rx, ry))
            {
                rx = rand() % size;
                ry = rand() % size;
            }
            tile tr1 = tile("!!");
            tr1.tr = true;
            bubble* bk = new bubble(rx, ry);
            addTreasure(treasure(rx, ry));
            main[rx][ry] = tr1;
            rx = rand() % size;
            ry = rand() % size;
        }
        printAll();
        start();
    }

    vector<tile>& operator[](int i)
    {
        return main[i];
    }

    int fancyMod(int x, int y)
    {
        return x % y;
    }

    void printAll()
    {
        for (int i = 0; i < main.size() + 2; i++)
            cout << "▒▒";
        cout << endl;
        for (int i = 0; i < main.size(); i++)
        {
            cout << "▒▒";
            for (int f = 0; f < main.size(); f++)
                if (i >= 0 && i < size && f >= 0 && f < size)
                    if (main[fancyMod((i), size)][fancyMod((f), size)].getShape() == "  " && checkMoving(i, f) >= 0)
                        cout << moving[checkMoving(i, f)]->getShape();
                    else
                        cout << main[fancyMod((i), size)][fancyMod((f), size)].getShape();
            cout << "▒▒" << endl;
        }
        for (int i = 0; i < main.size() + 2; i++)
            cout << "▒▒";
        cout << endl;
    }
    void printStates()
    {
        cout << endl;
        for (int i = 0; i < size; i++)
        {
            for (int f = 0; f < size; f++)
            {
                tile t = main[i][f];
                if (t.state == -10)
                    cout << rgb(0, 0, 0, "██");
                if (t.state == -3)
                    cout << rgb(128, 64, 0, "██");
                if (t.state == -2)
                    cout << rgb(128, 128, 128, "██");
                if (t.state == -1)
                    cout << rgb(255, 255, 255, "██");
                if (t.state == 0)
                    cout << rgb(255, 0, 0, "██");
                if (t.state == 1)
                    cout << rgb(255, 64, 0, "██");
                if (t.state == 2)
                    cout << rgb(255, 128, 0, "██");
                if (t.state == 3)
                    cout << rgb(255, 192, 0, "██");
                if (t.state == 4)
                    cout << rgb(255, 255, 0, "██");
                if (t.state == 5)
                    cout << rgb(192, 255, 0, "██");
                if (t.state == 6)
                    cout << rgb(128, 255, 0, "██");
                if (t.state == 7)
                    cout << rgb(64, 255, 0, "██");
                if (t.state == 8)
                    cout << rgb(0, 255, 0, "██");
                if (t.state == 9)
                    cout << rgb(255, 0, 0, "██");
                if (t.state == 10)
                    cout << rgb(0, 255, 64, "██");
                if (t.state == 11)
                    cout << rgb(0, 255, 128, "██");
                if (t.state == 12)
                    cout << rgb(0, 255, 192, "██");
                if (t.state == 13)
                    cout << rgb(0, 255, 255, "██");
                if (t.state == 14)
                    cout << rgb(0, 192, 255, "██");
                if (t.state == 15)
                    cout << rgb(0, 128, 255, "██");
                if (t.state == 16)
                    cout << rgb(0, 64, 255, "██");
                if (t.state == 17)
                    cout << rgb(0, 0, 255, "██");
                if (t.state > 17)
                    cout << rgb(255, 0, 255, "██");
            }
            cout << endl;
        }
    }

    void printVisible(double s) //sight radius
    {
        double duh = s;
        s = ceil(s);
        cout << "(" << X << ", " << Y << ")" << endl;
        if (X < size - s)
            cX = X;
        if (Y > s - 1 && Y < size - s)
            cY = Y;
        for (int i = 0; i < (2 * s) + 1; i++)
        {
            for (int f = 0; f < (2 * s) + 1; f++)
            {
                int dY = Y - cY;
                int dX = X - cX;
                double d = sqrt(((s - i + dX) * (s - i + dX)) + ((s - f + dY) * (s - f + dY)));
                if (d == 0)
                    cout << "cↄ";
                else if (d < duh)
                    if (cX + i - s >= 0 && cX + i - s < size && cY + f - s >= 0 && cY + f - s < size)
                        if (main[fancyMod((cX + i - s), size)][fancyMod((cY + f - s), size)].getShape() == "  " && checkMoving(cX + i - s, cY + f - s) >= 0)
                            cout << moving[checkMoving(cX + i - s, cY + f - s)]->getShape();
                        else
                            cout << main[fancyMod((cX + i - s), size)][fancyMod((cY + f - s), size)].getShape();
                    else if (cX + i - s < 0)
                        cout << "  ";
                    else
                        cout << "▒▒";
                else
                    cout << "▒▒";
            }
            cout << endl;
        }
    }
    void start()
    {
        //cout << "start 0" << endl;
        for (entity* e : moving)
            e->start(this);
    }
    void step()
    {
        for (entity* e : moving)
            e->step();
    }
    void toggle()
    {
        for (int i = 0; i < moving.size(); i++)
            moving[i]->trigger();
    }
    void interact(double d)
    {
        for (entity* e : moving)
        {
            e->interact();
            e->trigger();
        }
    }
    bool checkSolid(int x, int y)
    {
        for (int i = 0; i < moving.size(); i++)
            if (moving[i]->solidity() && moving[i]->getx() == x && moving[i]->gety() == y)
                return true;
        return false;
    }
    int checkMoving(int x, int y)
    {
        for (int i = 0; i < moving.size(); i++)
            if (moving[i]->getx() == x && moving[i]->gety() == y)
                return i;
        return -1;
    }
    int checkEntity(int x, int y)
    {
        for (int i = 0; i < moving.size(); i++)
        {
            //cout << 1 << endl;
            if (moving[i]->getx() == x && moving[i]->gety() == y)
            {

                return moving[i]->gettype();
            }
        }
        return -1;
    }
    void addEntity(entity* e)
    {
        //cout << e->gettype() << endl;
        if (e->gettype() == 4 || e->gettype() == 3)
            if (e->getID() >= trigger.size())
                trigger.push_back(false);
        moving.push_back(e);
    }
    void addTreasure(treasure t)
    {
        hidden.push_back(t);
    }
    void swapT(int i)
    {
        hidden[i].t = !hidden[i].t;
    }
    bool raycast(int x1, int y1, int x2, int y2)
    {
        int dx = x2 - x1;
        int dy = y2 - y1;

        int steps = max(abs(dx), abs(dy));

        if (steps == 0)
        {
            return main[x1][y1].walkable;
        }

        double xInc = (double)dx / steps;
        double yInc = (double)dy / steps;


        double x = x1;
        double y = y1;

        for (int i = 0; i <= steps; i++)
        {
            int currentX = floor(x);
            int currentY = floor(y);

            if (currentX >= 0 && currentX < size)
            {
                if (!main[x][y].walkable)
                    return false;
            }

            x += xInc;
            y += yInc;
        }
        return true;
    }
};

int num = 64;
map aworld(num, (int)time(0));
map bworld(num, 10);
map sworld(num, (int)time(0));
void lever::interact()
{
    if (x == X && y == Y)
    {
        sworld.swatch(ID);
        sworld.toggle();
        toggled = !toggled;
        if (toggled)
            shape = "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m//\033[m";
        else
            shape = "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m\\\\\033[m";
    }
}
void door::trigger()
{
    if (sworld.trigger[ID])
    {
        solid = false;
        shape = "\033[38;2;" + to_string(r / 2) + ";" + to_string(g / 2) + ";" + to_string(b / 2) + "m" + doorShape + "\033[m";
    }
    else
    {
        solid = true;
        shape = "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + doorShape + "\033[m";
    }
}
void door::start(map* m) {
    bool n = (m->checkEntity(x - 1, y) == 4);
    bool e = (m->checkEntity(x, y + 1) == 4);
    bool s = (m->checkEntity(x + 1, y) == 4);
    bool w = (m->checkEntity(x, y - 1) == 4);
    if (n)
        if (s)
            if (e)
                if (w)
                    doorShape = "╪╪";
                else
                    doorShape = "╞╪";
            else
                if (w)
                    doorShape = "╪╡";
                else
                    doorShape = "││";
        else
            if (e)
                if (w)
                    doorShape = "╧╧";
                else
                    doorShape = "╘╧";
            else
                if (w)
                    doorShape = "╧╛";
                else
                    doorShape = "╘╛";
    else
        if (s)
            if (e)
                if (w)
                    doorShape = "╤╤";
                else
                    doorShape = "╒╤";
            else
                if (w)
                    doorShape = "╤╕";
                else
                    doorShape = "╒╕";
        else if (!n && !e && !s && !w)
            doorShape = "[]";
        else
            doorShape = "══";
    shape = "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + doorShape + "\033[m";
}

//class vent : public entity {
//private: 
//    int quota;
//    int cnt = 0;
//public:
//    vent(int _x, int _y) : entity(_x, _y)
//    {
//        solid = false;
//        quota = (rand() % 7) + 2;
//        shape = "/\\";
//    }
// int gettype()
    //{
    //    return 2;
    //}
//    void step()
//    {
//
//    }
//};


int fancyMod(int x, int y)
{
    return x % y;
}

string scatter[40] = { " ", " ", " ", "⠁", "⡀", "⠂", "⢀", "⠄", "⠈", "⠐", "⠠", "⠃", "⠅", "⠆", "⠊", "⠌",  "⠑", "⠔", "⠘", "⠡", "⠢", "⠨", "⠰", "⡁", "⡂", "⡄", "⡈", "⡐", "⡠", "⢀", "⢁", "⢂", "⢄", "⢈", "⢐", "⢠", "⠒", "⠤", "⠉", "⣀" };

time_t start = time(0);

string rgbackground(int r, int g, int b, string str)
{
    r = max(min(r, 255), 0);
    g = max(min(g, 255), 0);
    b = max(min(b, 255), 0);
    return "\033[0;48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + str + "\033[m";
}
string scatterTile()
{
    int i = floor(pow(rand() % 40, 3) / 1600.0);
    return scatter[i] + scatter[i];
}
string scatterTile(int seed)
{
    srand(seed);
    int i = floor(pow(rand() % 40, 3) / 1600.0);
    int f = floor(pow(rand() % 40, 3) / 1600.0);
    return scatter[i] + scatter[f];
}
void rgbTest()
{
    cout << endl << "world of more color!" << endl;
    for (int i = 0; i <= 256; i += 16)
    {
        for (int f = 0; f <= 256; f += 16)
        {
            for (int d = 0; d <= 256; d += 16)
            {
                cout << rgb(i, f, d, scatterTile());
            }
            cout << endl;
        }
        cout << endl;
    }
}

bool query(string q)
{
    cout << q << endl << "y/n : ";
    char c;
    cin >> c;
    if (c == 'y')
        return true;
    else
        return false;

}

void printWorld()
{
    //cout << "print:" << endl;
    for (int i = 0; i < num; i++)
    {
        for (int f = 0; f < num; f++)
        {
            cout << sworld[i][f].getShape();
        }
        cout << endl;
    }
    cout << endl;
}
void printTreasures()
{
    for (int i = 0; i < treasureList.size(); i++)
    {
        cout << treasureList[i].getName() << ": \033[38;2;255;215;0m" << treasureList[i].getValue() << "\033[m doubloons" << endl;
    }
}
void printO2()
{
    if (O2 >= maxO2 / 2)
    {
        int p = 510 * (1 - (O2 / (double)maxO2));
        cout << "O2: \033[38;2;" << p << ";255;0m" << O2 << "/" << maxO2 << "\033[m" << endl;
    }
    else
    {
        int p = (510 * (O2 / (double)maxO2));
        cout << "O2: \033[38;2;255;" << p << ";0m" << O2 << "/" << maxO2 << "\033[m" << endl;
    }
}

void printError(int event)
{
    issue = 0;
    if (event == 1)
        cout << "there's a tile here!" << endl;
    if (event == 2)
        cout << "there's something here!" << endl;
    if (event == 3)
        cout << "you can't go any further!" << endl;
}
void UI(int event) // 0 to clear screen
{
    cout << "\033[2J\033[1;1H";
    if (event == 1) // waiting at dock
    {
        cout << "press q to descend, e to relocate, or p to quit." << endl;
        cout << "you have \033[38;2;255;215;0m" << money << "\033[m doubloons, and you need \033[38;2;255;215;0m" << (quota * quota * quotaMultiplier * swindle) << "\033[m doubloons for your next quota." << endl;
        cout << "the pirates will leave in \033[38;2;200;0;0m" << ct << "\033[m attempt(s)." << endl;
        sworld.printVisible(glass);
        if (tutDesc)
        {
            cout << "YARRGH undock NOW [q] and begin getting treasures for us !!!!!!!!!" << endl;
        }
    }
    if (event == 2) // just docked
    {
        cout << "press q to descend, e to relocate, or p to quit." << endl;
        double total = 0;
        cout << "you have resurfaced.\nyour \033[38;2;50;215;50moxygen\033[m has been refilled.\nafter this descent, you got:" << endl;
        for (int i = 0; i < treasureList.size(); i++)
        {
            total += treasureList[i].getValue();
            money += treasureList[i].getValue();
            cout << "\033[38;2;255;215;0m" << treasureList[i].getName() << "\033[m worth \033[38;2;255;215;0m" << treasureList[i].getValue() << "\033[m doubloons." << endl;
        }
        if (treasureList.size() == 0)
        {
            cout << "\033[38;2;255;215;0mnothing lmao\033[m" << endl;
        }
        cout << "total earnings this descent: +\033[38;2;255;215;0m" << total << "\033[m doubloons." << endl;
        treasureList = vector<treasure>();
        cout << "you have \033[38;2;255;215;0m" << money << "\033[m doubloons, and you need \033[38;2;255;215;0m" << (quota * quota * quotaMultiplier * swindle) << "\033[m doubloons for your next quota." << endl;
        cout << "the pirates will leave in \033[38;2;200;0;0m" << ct << "\033[m attempt(s)." << endl;
        sworld.printVisible(glass);
        if (!tutGet && tutDock)
        {
            tutDock = false;
            cout << "YARRGH NOW DO IT AGAIN!! YE NEED 10 DOUBLOONS BEFORE WE LEAVE!!!" << endl;
        }
        else if (!tutDesc && tutDock)
        {
            cout << "YARRGH WHAT'RE YE DOIN WE NEED TREASURES !!!!!!!!!!" << endl;
        }
        if (ct <= 0)
            cout << "\033[38;2;200;0;0mYER TAKIN TOO LONG !!!!!\033[m" << endl;
    }
    if (event == 3) // roaming
    {
        cout << "move: [wasd] | inspect: [i] | list treasure: [t] | surface: [q] | quit: [p]" << endl;
        printO2();
        sworld.printVisible(glass);
        printError(issue);
        if (tutScan)
        {
            cout << "YARRGH use yer TRUSTY SCANNER [i] to locate booty !!! it don't be the most reliable, though." << endl;
        }
        if (tutGet && !tutScan)
        {
            cout << "YARRGH when ye FIND treasure (!! or ??) go DRIVE OVER TO IT!!! WE DONT PICK THINGS UP FOR YE!!!" << endl;
        }
        if (!tutGet && tutDock)
        {
            cout << "YARRGH return to our ship [q] and give us yer treasures !!!" << endl;
        }
        //cout << sworld[X][Y].state << " " << sworld[X][Y].checked << endl;
    }
    if (event == 4) // roaming, treasure found
    {
        cout << "move: [wasd] | inspect: [i] | list treasure: [t] | surface: [q] | quit: [p]" << endl;
        printO2();
        sworld.printVisible(glass);
        printError(issue);
        if (tutScan)
        {
            cout << "YARRGH use yer TRUSTY SCANNER [i] to locate booty !!! it don't be the most reliable, though." << endl;
        }
        if (tutGet && !tutScan)
        {
            cout << "YARRGH when ye FIND treasure (!! or ??) go DRIVE OVER TO IT!!! WE DONT PICK THINGS UP FOR YE!!!" << endl;
        }
        if (!tutGet && tutDock)
        {
            cout << "YARRGH return to our ship [q] and give us yer treasures !!!" << endl;
        }
        cout << "you got \033[38;2;255;215;0m" << treasureList[treasureList.size() - 1].getName() << "\033[m worth \033[38;2;255;215;0m" << treasureList[treasureList.size() - 1].getValue() << "\033[m doubloons!" << endl;
    }
    if (event == 10) // shop
        event; // placeholder for shop UI
}
void restart(int event)
{
    char k;
    UI(0);
    if (event == 0) // ran out of oxygen
        cout << "\033[38;2;200;0;0myou ran out of oxygen and died.\033[m" << endl;

    if (event == 1) // failed to meet quota
        cout << "\033[38;2;200;0;0mYARRR YE BE SLEEPIN WITH THE FISHIESSS !!!!!\033[m" << endl;

    if (event == 2) // killed by traps or pirates
        cout << "\033[38;2;200;0;0myour hull was breached and you were crushed.\033[m" << endl;

    if (event == 3) // died to a door closing while you were inside it
        cout << "\033[38;2;200;0;0mwell, that's embarrassing.\033[m" << endl;

    if (event == 4) // something unexpected happened, causing you to die
        cout << "\033[38;2;200;0;0mhow...?\033[m" << endl;

    cout << "enter anything to restart: ";
    cin >> k;
    //sworld = map(num, (int)time(0));
    //sworld.fullGen();
    quota = 1;
    money = 0;
    maxO2 = 100; //       d1  | maximum oxygen
    O2gen = 0; //         d2  | produce oxygen when not docked at surface
    scuba = 0; //         d3  | distance from surface oxy gen works, oxy gen decreases farther from surface however
    luck = 0.0; //        d4  | liklihood of getting better treasure
    maxLuck = 0.2; //     d5  | highest treasure component proportional to number of components
    prosperity = 0.0; //  d6  | more treasure
    swindle = 1.0; //     d7  | lower quota
    light = 0.0; //       d8  | larger light
    glass = 3.5; //       d9  | farther sight
    faulty = 0.5; //      d10 | chance of detecting false treasures
    dish = 2.5; //        d11 | inspect scan radius
    bargain = 3; //       d12 | number of upgrade choices each island
    tolerance = 5; //     d13 | number of times pirates will let you dock before moving to the next location
    ct = 5;
    sub = 0;
    sworld = aworld;
    UI(1);
}

void ascend()
{
    O2 = maxO2;
    sub = 0;
    ct--;
    UI(2);
    if (ct <= 0)
    {
        if (money >= quota * quota * quotaMultiplier * swindle)
        {
            money -= quota * quota * quotaMultiplier * swindle;
            quota++;
        }
        else
        {
            restart(1);
        }
    }
}
bool a = true;
//bool query(string arg)
//{
//    char k;
//    cout << arg << " y/n: ";
//    cin >> k;
//    if (k == 'y')
//        return true;
//    else if (k == 'n')
//        return false;
//    else
//        return query(arg);
//}
void moveP(int x, int y)
{
    if (x < num && x >= 0 && y < num && y >= 0)
        if (!sworld.checkSolid(x, y))
            if (sworld[x][y].walkable)
            {
                X = x; Y = y;
                if (X == 0 && O2 < maxO2 && a)
                    O2++;
                if (X > 0)
                    O2--;
            }
            else
                issue = 1;
        else
            issue = 2;
    else
        if (X == num - 1 && a)
        {
            if (query("proceed to the depths?"))
            {
                aworld = sworld;
                sworld = bworld;
                X = 0;
                a = !a;
            }
        }
        else if (X == 0 && !a)
        {
            if (query("return to the shallow water?"))
            {
                bworld = sworld;
                sworld = aworld;
                X = num - 1;
                cX = num - 5;
                a = !a;
            }
        }
        else
            issue = 3;
    sworld.step();
}

void subControl(char c)
{
    bool tres = false;
    if (c == 'w')
        moveP(X - 1, Y);
    if (c == 's')
        moveP(X + 1, Y);
    if (c == 'a')
        moveP(X, Y - 1);
    if (c == 'd')
        moveP(X, Y + 1);
    if (sworld[fancyMod(X, num)][fancyMod(Y, num)].tr)
    {
        tutGet = false;
        for (int k = 0; k < sworld.tlist().size(); k++)
        {
            treasure i = sworld.tlist()[k];
            if (i.x == X && i.y == Y && i.t == false)
            {
                treasureList.push_back(i);
                sworld.swapT(k);
                tres = true;
                sworld[X][Y].shape = "..";
                sworld[X][Y].tr = false;
            }
        }
    }
    if (c == 'i')
    {
        tutScan = false;
        sworld.interact(dish);
    }
    //sworld.printVisible(glass);
    if (c == 't')
    {
        printTreasures();
    }
    else if (c == 'q' && X == 0 && a)
        ascend();
    else
        if (tres)
            UI(4);
        else
            UI(3);
}

void shopControl(char c)
{
    if (c == 'q')
    {
        sub = true;
        tutDesc = false;
        subControl(' ');
    }
    else
    {
        if (c == 'e')
        {
            if (money >= quota * quota * quotaMultiplier * swindle)
            {
                money -= quota * quota * quotaMultiplier * swindle;
                quota++;
            }
            else
            {
                restart(1);
            }
            sworld.fullGen();
        }
        UI(1);
    }

}

void intro()
{
    char k;
    cout << "having been cast from your family as a child, you followed a life of crime." << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
    cout << "after being caught and sent to jail for the Nth time, you were to be given \033[38;2;200;0;0mCAPITAL PUNISHMENT\033[m." << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
    cout << "you were let off the hook this last time, however, on the condition that you worked for a shady gang of pirates to help them make \033[38;2;255;215;0mdoubloons\033[m." << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
    cout << "the best way you know how is to find \033[38;2;255;215;0mtreasures\033[m!" << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
    cout << "the pirates \033[38;2;200;0;0mwon't be going easy on you\033[m, however, and will take an amount equal to your current \033[38;2;255;215;0mquota\033[m every time you relocate." << endl;
    cout << "press anything to continue: ";
    cin >> k;
    //cout << "\033[2J\033[1;1H";
    //cout << "when you relocate, the pirates take you to an island, where you can \033[38;2;255;215;0mupgrade\033[m your treasure hunting gear!" << endl;
    //cout << "press anything to continue: ";
    //cin >> k;
    cout << "\033[2J\033[1;1H";
    cout << "and if you don't have enough money to meet your \033[38;2;255;215;0mquota\033[m, the pirates will \033[38;2;200;0;0mfeed you to the fishes\033[m!" << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
    cout << "and so, with enough said, you hop in your rusty submarine to find some treasures." << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
}

int main()
{
    lever* l = new lever(1, 8, 255, 255, 0, 0);
    door* d1 = new door(2, 8, 255, 255, 0, 0);
    door* d2 = new door(2, 7, 255, 255, 0, 0);
    door* d3 = new door(2, 9, 255, 255, 0, 0);
    door* d4 = new door(1, 9, 255, 255, 0, 0);
    door* d5 = new door(1, 10, 255, 255, 0, 0);
    door* d6 = new door(2, 10, 255, 255, 0, 0);
    door* d7 = new door(2, 6, 255, 255, 0, 0);
    bubble* b = new bubble(1, 7);
    bubble* b2 = new bubble(1, 6);
    bubble* b3 = new bubble(30, 33);
    bubble* b4 = new bubble(48, 62);
    bubble* b5 = new bubble(48, 63);
    aworld.addEntity(b);
    aworld.addEntity(b2);
    aworld.addEntity(b3);
    aworld.addEntity(b4);
    aworld.addEntity(b5);
    aworld.addEntity(l);
    aworld.addEntity(d1);
    aworld.addEntity(d2);
    aworld.addEntity(d3);
    aworld.addEntity(d4);
    aworld.addEntity(d5);
    aworld.addEntity(d6);
    aworld.addEntity(d7);
    cout << aworld.moving.size();
    aworld.start();

    Y = num / 2;
    X = 0;
    //cout << (int)time(0) << endl;
    //intro();
    cout << "e" << endl;
    //srand((int)time(0));
    srand(0);
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    cout << "e" << endl;
    aworld.fullGen();

    tile tr1("!!");
    tr1.tr = true;
    for (int i = 0; i < 5; i++)
        for (int f = 0; f < 5; f++)
            aworld[i][f + 6] = tile();
    aworld[32][39] = tile();
    aworld.addTreasure(treasure(3, 8));
    aworld.addTreasure(treasure(3, 7));
    aworld.addTreasure(treasure(3, 9));
    aworld.trigger.push_back(0);
    aworld[3][8] = tr1;
    aworld[3][7] = tr1;
    aworld[3][9] = tr1;
    if (query("generate the depths COMPLETELY randomly? note that you can look at the map before playing, scroll up before entering anything else. you may have to increase window size to fit the whole map."))
    {
        srand(time(0));
    }
    bworld.depthsGen();
    for (int i = 0; i < num; i++)
    {
        aworld[63][i] = tile();
    }
    sworld = aworld;
    //cout << "e" << endl;
    //printWorld();
    Y = 8;
    UI(1);
    //bworld.printStates();
    //bworld.printAll();
    //cout << sworld.trigger[0];
    while (true)
    {
        cout << "action: ";
        char c;
        //if (_kbhit())
        //{
            //c = _getch();
        cin >> c;
        c = (char)tolower(c);
        if (sub)
            subControl(c);
        else
            shopControl(c);
        if (c == 'p')
            break;
        if (O2 <= 0)
        {
            X = 0;
            Y = 8;
            ascend();
            restart(0);
        }
        //sworld.printAll();
        //cout << sworld.tlist().size();
        //}
    }
}