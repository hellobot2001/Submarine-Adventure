#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#ifdef _WIN32
#include <windows.h>
#endif
//#include <conio.h>

using namespace std;

//list of tiles. mostly for copy pasting, i doubt ill ever uncomment it
//string tiles[8] = { "██", "▓▓", "▒▒", "░░", "  ", "??", "$>", "$<",  };
//RAND_MAX

class tile
{
private:
    int seed;
    string shape;
public:
    string text = "";
    bool walkable = true;
    bool tr = false;
    bool ft = false;
    tile(int s, string h)
    {
        shape = h;
        seed = s;
    }

    tile()
    {
        shape = "  ";
        seed = rand();
    }

    int getSeed()
    {
        return seed;
    }

    string getShape()
    {
        return shape;
    }
};
//funny random metal name generation because funny
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
class treasure
{
private:
    string name;
    double value;
public:
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
        return floor(maxLuck * pow(r % t, 1 / n));
    }

    void setName()
    {
        int st = brand() % chance(20, 4-luck);
        int ms = brand() % chance(102, 4-luck);
        int mm = brand() % chance(22, 4-luck);
        int me = brand() % chance(40, 4-luck);
        int ty = brand() % chance(33, 4-luck);
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
//what's this?? a 3D VECTOR!?!
//functions like level of detail
//a vector of 2D vectors, with the 2D vectors being square and the vector of those 2D vectors having a length equal to your current depth
//a 3x3 square is generated based on their respective tiles' seeds, which in turn give a bunch of tiles their own seeds
//I might need to give up on this for now...
vector<vector<vector<tile>>> world = { { {} } }; 

//temp regular world of size num
int num = 256;
vector<vector<tile>> sworld = vector<vector<tile>>(num, vector<tile>(num, tile()));
vector<vector<tile>> tworld = vector<vector<tile>>(num, vector<tile>(num, tile()));

int X = 0;
int Y = 0;
int cX = 0;
int cY = 0;

double money = 0;
int quota = 1;
const int quotaMultiplier = 100;
/*
class upgrade {
private:
    double cost;
    string name;
    int d1;
    int d2;
    int d3;
    i
public:
    upgrade(int c, int n, int d1, int d2, int d3, double d4, double d5, double d6, double d7, double d8, double d9, double d10, double d11, int d12, int d13) //cost, name, maxO2, O2gen, scuba, treasure$, reassure$+, treasure+, quota, light, sight, faulty, scan, choice, tolerance
    {

    }
};*/

int maxO2 = 100; //       d1  | maximum oxygen
int O2gen = 0; //         d2  | produce oxygen when not docked at surface
int scuba = 0; //         d3  | distance from surface oxy gen works, oxy gen decreases farther from surface however
double luck = 0; //       d4  | liklihood of getting better treasure
double maxLuck = 0.2; //  d5  | highest treasure component proportional to number of components
double prosperity = 0; // d6  | more treasure
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
bool sub = false;

bool tutDesc = true;
bool tutScan = true;
bool tutGet = true;
bool tutDock = true;

vector<treasure> treasureList;

//list of characters that can appear in scatter tiles.
string scatter[40] = { " ", " ", " ", "⠁", "⡀", "⠂", "⢀", "⠄", "⠈", "⠐", "⠠", "⠃", "⠅", "⠆", "⠊", "⠌",  "⠑", "⠔", "⠘", "⠡", "⠢", "⠨", "⠰", "⡁", "⡂", "⡄", "⡈", "⡐", "⡠", "⢀", "⢁", "⢂", "⢄", "⢈", "⢐", "⢠", "⠒", "⠤", "⠉", "⣀" };

//time at the start of the program. this will be used to ensure that the original map is able to be loaded again.
time_t start = time(0);

//return a colored string.
string rgb(int r, int g, int b, string str)
{
    r = max(min(r, 255), 0);
    g = max(min(g, 255), 0);
    b = max(min(b, 255), 0);
    return "\033[0;38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + str + "\033[m";
}

string rgbackground(int r, int g, int b, string str)
{
    r = max(min(r, 255), 0);
    g = max(min(g, 255), 0);
    b = max(min(b, 255), 0);
    return "\033[0;48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + str + "\033[m";
}

//return a scatter tile.
string scatterTile()
{
    int i = floor(pow(rand() % 40, 3)/1600.0);
    return scatter[i] + scatter[i];
}

string scatterTile(int seed)
{
    srand(seed);
    int i = floor(pow(rand() % 40, 3) / 1600.0);
    int f = floor(pow(rand() % 40, 3) / 1600.0);
    return scatter[i] + scatter[f];
}

//print out a variety of rgb scatter tiles.
void rgbTest()
{
    cout << endl << "world of more color!" << endl;
    for (int i = 0; i <= 256; i+=16)
    {
        for (int f = 0; f <= 256; f+=16)
        {
            for (int d = 0; d <= 256; d+=16)
            {
                cout << rgb(i, f, d, scatterTile());
            }
            cout << endl;
        }
        cout << endl;
    }
}

void setTile(double c, int d, int x, int y, tile t)
{
    if (c < rand()/(double)RAND_MAX)
    world[d][x][y] = t;
}

int fancyMod(int x, int y)
{
    if (x < 0 && y > 0)
    {
        return fancyMod(y + x, y);
    }
    else
        return x % y;
}
void setTile(vector<vector<tile>>& w, double c, int x, int y, tile t)
{
    //cout << "    create tile at (" << x << ", " << y << ") with chance " << c*100 << "%" << endl;
    if (c < rand() / (double)RAND_MAX)
    {
        w[fancyMod(x, num)][fancyMod(y, num)] = t;
        //cout << "        tile made! " << w[x][y].getShape() << endl;
    }
}

void line(int d, int x1, int y1, int x2, int y2, tile t)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = max(abs(dx), abs(dy));

    if (steps == 0)
    {
        setTile(1, d, x1, y1, t);
        return;
    }

    double xInc = (double)dx / steps;
    double yInc = (double)dy / steps;
    

    double x = x1;
    double y = y1;

    for (int i = 0; i <= steps; i++)
    {
        int currentX = floor(x);
        int currentY = floor(y);

        if (currentX >= 0 && currentX < 48)
        {
            setTile(1, d, currentX, currentY, t);
        }

        x += xInc;
        y += yInc;
    }
}

void genCircle(vector<vector<tile>>& w, int x, int y, int r, double ci, double co, tile t) //world, start x, start y, radius, inner chance, outer chance, tile
{
    int px = x - r;
    int py = y - r;
    for (int i = 0; i < (2 * r) + 1; i++)
    {
        for (int f = 0; f < (2 * r) + 1; f++)
        {
            double d = sqrt(((px + i - x) * (px + i - x)) + ((py + f - y) * (py + f - y)));
            if (((px + i > 0) && (py + f > 0)) && ((px + i < num) && (py + f < num)))
            {
                if (d <= r)
                {
                    double cot = ((d * co) / r) + ((1 - (d / r)) * ci);
                    setTile(w, cot, (px + i), (py + f), t);
                }
            }
        }
        py = y - r;
    }
}

void genPath(vector<vector<tile>>& w, int nr, int xr, int jmp, int l, double ci, double co, tile t) //world, min radius, max radius, max jump distance, # of circles created, inner chance, outer chance, tile
{
    int sx = rand() % num;
    int sy = rand() % num;
    for (int i = 0; i < l; i++)
    {
        genCircle(w, sx, sy, (rand() % (xr - nr)) + nr, ci, co, t);
        sx = (sx + (rand() % ((2 * jmp) + 1)) - jmp) % num; //jumps anywhere in a square of +- jmp
        sy = (sy + (rand() % ((2 * jmp) + 1)) - jmp) % num;
    }
}

void regen()
{
    ct = tolerance;
    sworld = vector<vector<tile>>(num, vector<tile>(num, tile()));
    attempts++;
    X = 0;
    Y = num / 2;
    int seed = rand();
    tile t;
    srand(seed + attempts);
    double irand = rand();
    double drand = irand / (double)RAND_MAX;
    for (int i = 0; i < (int)(pow(log2(num), 1.75 + drand)); i++)
    {
        t = tile(rand(), "██");
        t.walkable = false;
        genPath(sworld, rand() % 3, (rand() % 8) + 3, rand() % 8, 50, 0, 1, t);
    }
    irand = rand();
    drand = irand / (double)RAND_MAX;
    for (int i = 0; i < (int)(pow(log2(num), 2 + drand + prosperity)); i++)
    {
        int rx = rand() % num;
        int ry = rand() % num;
        while (!sworld[rx][ry].walkable || tworld[rx][ry].tr)
        {
            rx = rand() % num;
            ry = rand() % num;
        }
        t = tile(rand(), "!!");
        t.tr = true;
        tworld[rx][ry] = t;
    }
    for (int i = 0; i < num; i++)
    {
        sworld[0][i] = tile(rand(), "~~");
    }
    while (!sworld[X][Y].walkable)
    {
        Y = rand() % num;
    }
}

void ascend()
{
    O2 = maxO2;
    sub = false;
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
    ct--;
    if (ct <= 0)
    {
        cout << "\033[2J\033[1;1H";
        cout << "\033[38;2;200;0;0mYER TAKIN TOO LONG !!!!!\033[m" << endl;
        if (money >= quota * quota * quotaMultiplier * swindle)
        {
            money -= quota * quota * quotaMultiplier * swindle;
            quota++;
        }
        else
        {
            O2 = 0;
            cout << "\033[38;2;200;0;0mYARRR YE BE SLEEPIN WITH THE FISHIESSS !!!!!\033[m" << endl;
            quota = 1;
        }
        regen();
    }
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

void printVisible(vector<vector<tile>>& w, double s) //world, sight radius
{
    double duh = s;
    s = ceil(s);
    cout << "(" << X << ", " << Y << ")" << endl;
    if (X < num - s)
        cX = X;
    if (Y > s - 1 && Y < num - s)
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
                if (cX + i - s >= 0 && cX + i - s < num && cY + f - s >= 0 && cY + f - s < num)
                    cout << w[fancyMod((cX + i - s), num)][fancyMod((cY + f - s), num)].getShape();
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

void printTreasures()
{
    for (int i = 0; i < treasureList.size(); i++)
    {
        cout << treasureList[i].getName() << ": \033[38;2;255;215;0m" << treasureList[i].getValue() << "\033[m doubloons" << endl;
    }
}

void printO2()
{
    if (X > 0 && sub)
        O2 -= 5;
    if (X <= scuba && O2gen - X > 0)
        O2 += O2gen - X;
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
void subControl(char c)
{
    cout << "\033[2J\033[1;1H";
    cout << "move: [wasd] | inspect: [i] | list treasure: [t] | surface: [q] | quit: [p]" << endl;
    if (c == 'w')
        if (sworld[fancyMod((X - 1), num)][Y].walkable && X > 0)
        {
            X--;
            printO2();
        }
        else
            cout << "can't go there!" << endl;
    if (c == 's')
        if (sworld[fancyMod((X + 1), num)][Y].walkable && X < num - 1)
        {
            X++;
            printO2();
        }
        else
            cout << "can't go there!" << endl;
    if (c == 'a')
        if (sworld[X][fancyMod((Y - 1), num)].walkable && Y > 0)
        {
            Y--;
            printO2();
        }
        else
            cout << "can't go there!" << endl;
    if (c == 'd')
        if (sworld[X][fancyMod((Y + 1), num)].walkable && Y < num - 1)
        {
            Y++;
            printO2();
        }
        else
            cout << "can't go there!" << endl;
    if (sworld[fancyMod(X, num)][fancyMod(Y, num)].tr)
    {
        tutGet = false;
        treasureList.push_back(treasure());
        cout << "you got \033[38;2;255;215;0m" << treasureList[treasureList.size() - 1].getName() << "\033[m worth \033[38;2;255;215;0m" << treasureList[treasureList.size() - 1].getValue() << "\033[m doubloons!" << endl;
        sworld[fancyMod(X, num)][fancyMod(Y, num)] = tile(rand(), "..");
    }
    if (c == 'i')
    {
        tutScan = false;
        int s = ceil(dish);
        for (int i = 0; i < (2 * s) + 1; i++)
        {
            for (int f = 0; f < (2 * s) + 1; f++)
            {
                double d = sqrt(((s - i) * (s - i)) + ((s - f) * (s - f)));
                if (d < dish)
                    if (X + i - s >= 0 && X + i - s < num && Y + f - s >= 0 && Y + f - s < num)
                    {
                        if (tworld[X + i - s][Y + f - s].tr)
                        {
                            sworld[X + i - s][Y + f - s] = tworld[X + i - s][Y + f - s];
                            tworld[X + i - s][Y + f - s] = tile();
                        }
                    }
            }
        }
    }
    if (tutScan)
    {
        cout << "YARRGH use yer TRUSTY SCANNER [i] to locate goodies !!! it ain't the most reliable, though." << endl;
    }
    if (tutGet && !tutScan)
    {
        cout << "YARRGH when ye FIND treasure (!! or ??) go DRIVE OVER TO IT!!! WE DONT PICK THINGS UP FOR YE!!!" << endl;
    }
    if (!tutGet && tutDock)
    {
        cout << "YARRGH return to our ship and give us yer treasures !!!" << endl;
    }
    printVisible(sworld, glass);
    if (c == 't')
    {
        printTreasures();
    }
    if (c == 'q' && X == 0)
    {
        cout << "\033[2J\033[1;1H";
        if (!tutGet && tutDock)
        {
            tutDock = false;
            cout << "YARRGH NOW DO IT AGAIN!! YE NEED 100 DOUBLOONS BEFORE WE LEAVE!!!" << endl;
        }
        else if (!tutDesc && tutDock)
        {
            cout << "YARRGH WHAT'RE YE DOIN WE NEED TREASURES !!!!!!!!!!" << endl;
        }
        ascend();
        cout << "press q to descend, e to relocate, or p to quit." << endl;
        cout << "you have \033[38;2;255;215;0m" << money << "\033[m doubloons, and you need \033[38;2;255;215;0m" << (quota * quota * quotaMultiplier * swindle) << "\033[m doubloons for your next quota." << endl;
        cout << "the pirates will leave in \033[38;2;200;0;0m" << ct << "\033[m attempt(s)." << endl;
        printVisible(sworld, glass);
    }
}

void shopControl(char c)
{
    cout << "\033[2J\033[1;1H";
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
                O2 = 0;
                cout << "\033[38;2;200;0;0mYARRR YE BE SLEEPIN WITH THE FISHIESSS !!!!!\033[m" << endl;
                quota = 1;
            }
            regen();
        }
        cout << "press q to descend, e to relocate, or p to quit." << endl;
        cout << "you have \033[38;2;255;215;0m" << money << "\033[m doubloons, and you need \033[38;2;255;215;0m" << (quota * quota * quotaMultiplier * swindle) << "\033[m doubloons for your next quota." << endl;
        cout << "the pirates will leave in \033[38;2;200;0;0m" << ct << "\033[m attempt(s)." << endl;
        if (tutDesc)
        {
            cout << "YARRGH undock NOW [q] and begin getting treasures for us !!!!!!!!!" << endl;
        }
        printVisible(sworld, glass);
    }
    
}

//upgrades: max O2+, Oxy gen, more treasure, better treasure, lower quota, more sight, better light, better inspect



int main()
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
    cout << "\033[2J\033[1;1H";
    cout << "when you relocate, the pirates take you to an island, where you can \033[38;2;255;215;0mupgrade\033[m your treasure hunting gear!" << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
    cout << "and if you don't have enough money to meet your \033[38;2;255;215;0mquota\033[m, the pirates will \033[38;2;200;0;0mfeed you to the fishes\033[m!" << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
    cout << "and so, with enough said, you hop in your rusty submarine to find some treasures." << endl;
    cout << "press anything to continue: ";
    cin >> k;
    cout << "\033[2J\033[1;1H";
    srand(start);
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    regen();
    //printWorld();
    cout << "press q to descend, e to relocate, or p to quit." << endl;
    cout << "you have \033[38;2;255;215;0m" << money << "\033[m doubloons, and you need \033[38;2;255;215;0m" << (quota * quota * quotaMultiplier * swindle) << "\033[m doubloons for your next quota." << endl;
    cout << "the pirates will leave in \033[38;2;200;0;0m" << ct << "\033[m attempt(s)." << endl;
    printVisible(sworld, glass);
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
                cout << "\033[38;2;200;0;0myou ran out of oxygen and died.\033[m" << endl;
                cout << "enter anything to restart: ";
                cin >> k;
                ascend();
                regen();
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
                cout << "\033[2J\033[1;1H";
                printVisible(sworld, glass);
            }
        //}
    }
        
        //sworld = vector<vector<tile>>(num, vector<tile>(num, tile()));
    //}
}

/*
                                                      ▒▒▒▒▒▒▒▒░░░░▒▒▒▒░░
                                                      ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
                                                      ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓
                                                      ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
                                                      ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓
                                                      ░░░░░░░░  ▒▒▒▒▒▒▒▒▒▒▒▒▓▓
                                                                  ▓▓▒▒▒▒▒▒▒▒▓▓
                                                                    ▒▒▒▒▒▒▒▒▓▓
                                                                    ▒▒▒▒▒▒▒▒▓▓
                                                                    ▒▒▒▒▒▒▒▒▓▓
                                                                    ▒▒▒▒▒▒▒▒▓▓
                                                                    ▒▒▒▒▒▒▒▒▓▓
                                                  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
                                                  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
                                                ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒
                                              ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒
                                            ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓▒▒▓▓
                                            ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▒▒▓▓
                                            ▓▓▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓
                                            ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓
                                            ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓
                                            ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓
                                            ▓▓▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓
                                            ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓
                                            ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓
                                            ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓
                            ░░▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒
                        ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
                    ▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓                            ▓▓▓▓▓▓▓▓
                  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒                      ▒▒▒▒▒▒▒▒▒▒▓▓
                ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓░░                ▓▓▒▒▒▒▓▓▒▒▒▒▒▒▓▓
              ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒              ▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓
            ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓          ▓▓▒▒▒▒▒▒▓▓▒▒▒▒▓▓
          ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒      ▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓
        ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒    ▓▓▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓
      ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▓▓▓▓▓▓██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓
      ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████████████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████▓▓████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▓▓
    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██▒▒▒▒▒▒▒▒▒▒██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████▒▒▒▒▒▒▒▒████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████▒▒▒▒▒▒████▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒░░░░
    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████▓▓░░░░░░░░░░░░██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████░░░░░░░░░░░░████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████░░░░░░░░░░██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓
  ▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████░░░░      ░░░░░░████▒▒▒▒▒▒▒▒▒▒▒▒████████░░  ░░░░░░░░██░░██▓▓▓▓▒▒▒▒▒▒▒▒▒▒████░░    ░░░░░░██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▒▒
  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██░░░░    ░░░░  ░░░░░░████▒▒▒▒▒▒▒▒▒▒██████░░░░  ░░░░░░░░░░████▓▓▒▒▒▒▒▒▒▒▒▒████░░    ░░░░░░░░░░██████▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▒▒
  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██░░░░  ░░░░░░░░  ░░░░████▒▒▒▒▒▒▒▒▒▒████░░░░  ░░░░░░░░░░░░████▓▓▒▒▒▒▒▒▒▒▒▒████░░  ░░░░░░░░░░░░░░████▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒
  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██░░░░  ░░░░░░░░░░  ░░████▒▒▒▒▒▒▒▒▒▒████░░░░  ░░░░░░░░░░░░████▓▓▒▒▒▒▒▒▒▒▒▒████░░  ░░░░░░░░░░░░░░████▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒░░
  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██░░░░░░░░░░░░░░░░░░░░████▒▒▒▒▒▒▒▒▒▒████▒▒░░░░░░░░░░░░░░▒▒████▓▓▒▒▒▒▒▒▒▒▒▒████░░░░░░░░░░░░░░░░▓▓████▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒
  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██░░░░░░░░░░░░░░░░████▓▓▒▒▒▒▒▒▒▒▓▓▓▓████░░░░░░░░░░░░░░████▓▓▓▓▒▒▒▒▒▒▒▒▒▒████▓▓░░░░░░░░░░░░░░████▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒░░
  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██▓▓░░░░░░░░░░░░▓▓████▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████░░░░░░░░░░▓▓████▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓████▓▓██░░░░░░▓▓▓▓████▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒
  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████████████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████████████████▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████████████████▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▓▓
    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████████████████▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██████████████▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██████████████▓▓▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▒▒▒▒  ▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒
    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▒▒▒▒      ▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓
    ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒        ▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒
      ░░▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒          ▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒
          ▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒                ▓▓▓▓▓▓▒▒▓▓
          ░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒                  ░░░░░░░░░░
              ▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒
              ░░▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒▒▒▓▓
                  ░░▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
                    ░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░
from https://textart.sh/topic/submarine
*/