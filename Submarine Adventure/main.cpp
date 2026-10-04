#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <windows.h>
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
string matStart[102] = { "Pyro", "Keki", "Cryo", "Bio", "Ura", "Hydro", "Hel", "Oxy", "Rhod", "Rad", "Franc", "Lith", "Beryl", "Tung", "Merc", "Moly", "Poly", "Bi", "Tita", "Carbo", "Alum", "Gall", "Osm", "Irid", "Tant", "Plat", "Lead", "Polo", "Iron", "Photo", "Vita", "Carb", "Pallad", "Sil", "Mecha", "Fort", "Anti", "Trans", "Commu", "Reallyhard", "Orich", "Cob", "Mith", "Myth", "Ada", "Lumin", "Aura", "Crim", "Demon", "Meteor", "Stellar", "Astral", "Thermo", "Tempo", "Manta", "Terra", "Tarra", "Aero", "Aerial", "Peren", "Aqua", "Scor", "Exod", "Nept", "Naut", "Mater", "Voca", "Vox", "Neo", "Signal", "Lum", "Ender", "Prom", "F", "Naqu", "Admin", "Endo", "Extra", "Ultra", "Super", "Vibra", "Aether", "Chemo", "Ferro", "Hema", "Krypto", "Crypto", "Di", "Dura", "Lunar", "Nan", "Nether", "End", "Quant", "Red", "Tele", "San", "Sea", "Tiber", "Trit", "Unob", "Thaum" };

string matMiddle[22] = { "al", "in", "synth", "bde", "l", "a", "man", "un", "eth", "uck", "ah", "istrat", "therm", "kill", "carn", "lith", "ill", "de", "o", "ston", "tai", "ver"};

string matEnd[40] = { "cyte", "cite", "ic", "nite", "gen", "lium", "nium", "ium", "ury", "num", "lite", "osm", "sten", "ten", "n", "on", "con", "icon", "form", "alcum", "alt", "ril", "il", "tite", "ite", "tane", "ia", "ine", "line", "le", "mory", "vox", "ing", "dah", "tine", "t", "ide", "matter", "er", "e"};

class treasure
{
private:
    string name;
    double value;
public:
    treasure()
    {
        value = (rand() / (double)RAND_MAX) * 100;
        setName();
    }

    void setName()
    {
        name = name + matStart[rand() % 102];
        name = name + matMiddle[rand() % 22];
        name = name + matEnd[rand() % 40];
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

int maxO2 = 100;
int O2 = 100;

int attempts = 0;
bool sub = false;

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
    for (int i = 0; i < (int)(pow(log2(num), 2 + drand)); i++)
    {
        int rx = rand() % num;
        int ry = rand() % num;
        while (!sworld[rx][ry].walkable || sworld[rx][ry].tr)
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
    for (int i = 0; i < treasureList.size(); i++)
        money += treasureList[i].getValue();
    treasureList = vector<treasure>();
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

void printVisible(vector<vector<tile>>& w, int s) //world, sight radius
{
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
            else if (d < s)
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
    cout << "move: [wasd] | inspect: [i] | list treasure: [t] | surface (if at top of ocean): [q] | quit: [p]" << endl;
    if (c == 'w')
        if (sworld[fancyMod((X - 1), num)][Y].walkable && X > 0)
        {
            X--;
            if (X > 0 && sub)
                O2--;
            printO2();
        }
        else
            cout << "can't go there!" << endl;
    if (c == 's')
        if (sworld[fancyMod((X + 1), num)][Y].walkable && X < num - 1)
        {
            X++;
            if (X > 0 && sub)
                O2--;
            printO2();
        }
        else
            cout << "can't go there!" << endl;
    if (c == 'a')
        if (sworld[X][fancyMod((Y - 1), num)].walkable && Y > 0)
        {
            Y--;
            if (X > 0 && sub)
                O2--;
            printO2();
        }
        else
            cout << "can't go there!" << endl;
    if (c == 'd')
        if (sworld[X][fancyMod((Y + 1), num)].walkable && Y < num - 1)
        {
            Y++;
            if (X > 0 && sub)
                O2--;
            printO2();
        }
        else
            cout << "can't go there!" << endl;
    if (sworld[fancyMod(X, num)][fancyMod(Y, num)].tr)
    {
        treasureList.push_back(treasure());
        sworld[fancyMod(X, num)][fancyMod(Y, num)] = tile(rand(), "..");
    }
    if (c == 'i')
    {
        int s = 6;
        for (int i = 0; i < (2 * s) + 1; i++)
        {
            for (int f = 0; f < (2 * s) + 1; f++)
            {
                double d = sqrt(((s - i) * (s - i)) + ((s - f) * (s - f)));
                if (d < s)
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
    printVisible(sworld, 6);
    if (c == 't')
    {
        printTreasures();
    }
    if (c == 'q' && X == 0)
    {
        ascend();
        cout << "\033[2J\033[1;1H";
        cout << "press q to descend, e to relocate, or p to quit." << endl;
        cout << "you have \033[38;2;255;215;0m" << money << "\033[m doubloons." << endl;
        printVisible(sworld, 6);
    }
}

void shopControl(char c)
{
    cout << "\033[2J\033[1;1H";
    cout << "press q to descend, e to relocate, or p to quit." << endl;
    cout << "you have \033[38;2;255;215;0m" << money << "\033[m doubloons." << endl;
    if (c == 'q')
    {
        sub = true;
        subControl(' ');
    }
    else
    {
        if (c == 'e')
            regen();
        printVisible(sworld, 6);
    }
    
}

int main()
{
    cout << rgb(23, 48, 233, rgbackground(233, 23, 48, "hello")) << endl;
    srand(start);
    SetConsoleOutputCP(CP_UTF8);
    regen();
    printWorld();
    cout << "press q to descend, e to relocate, or p to quit." << endl;
    cout << "you have \033[38;2;255;215;0m" << money << "\033[m doubloons." << endl;
    printVisible(sworld, 6);
    while (true)
    {
        cout << "action: ";
        char c;
        //if (_kbhit())
        //{
            //c = _getch();
            cin >> c;
            if (sub)
                subControl(c);
            else
                shopControl(c);
            if (c == 'p')
                break;
            if (O2 <= 0)
            {
                cout << "you ran out of oxygen and died." << endl;
                cout << "enter anything to restart: ";
                char k;
                cin >> k;
                money = 0;
                regen();
                ascend();
                cout << "\033[2J\033[1;1H";
                printVisible(sworld, 6);
            }
            Sleep(10); //prevents output from going black when you make fast inputs
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