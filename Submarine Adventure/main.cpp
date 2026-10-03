#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <windows.h>
#include <conio.h>

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
    bool sell = false;
    bool buy = false;
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

int X = 0;
int Y = 0;
double money = 0;

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
    return "\033[0;38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + ";49m" + str + "\033[m";
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
    return scatter[i] + scatter[i];
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
    //cout << "print visible";
    X = fancyMod(X, num);
    Y = fancyMod(Y, num);
    cout << "(" << X+1 << ", " << Y << ")" << endl;
    for (int i = 0; i < (2 * s) + 1; i++)
    {
        for (int f = 0; f < (2 * s) + 1; f++)
        {
            double d = sqrt(((s - i) * (s - i)) + ((s - f) * (s - f)));
            if (d == 0)
                cout << "웃";
            else if (d < s)
                cout << w[fancyMod((X + i - s), num)][fancyMod((Y + f - s), num)].getShape();
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
        cout << treasureList[i].getName() << endl;
    }
}

int main()
{
    srand(start);
    X = num / 2;
    Y = num / 2;
    while (!sworld[X][Y].walkable)
    {
        //cout << "(" << X << ", " << Y << ")" << endl;
        X = rand() % num;
        Y = rand() % num;
    }
    //cout << rand() << endl;
    SetConsoleOutputCP(CP_UTF8);
    //while (true)
    //{
        int seed = rand();
        /*out << "enter seed, or 0 to stop or -1 to draw one big circle: ";
        cin >> seed;
        cout << seed;
        if (seed == 0)
        {
            break;
        }*/
        tile t = tile(rand(), "██");
        //if (seed > 0)
        //{
            srand(seed);
            srand(rand());
            srand(rand());
            srand(rand());
            srand(rand());
            srand(rand());
            srand(rand());
            double irand = rand();
            double drand = irand / (double)RAND_MAX;
            //cout << irand << "/" << RAND_MAX << " = " << drand << endl;
            for (int i = 0; i < (int)(pow(log2(num), 2 + drand)); i++)
            {
                //cout << i << endl;
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
                sworld[rx][ry] = t;
            }
            cout << "wasd to move, t to see treasures, q to return to surface (only around your spawnpoint). p to quit." << endl;
        //}
        //else genCircle(sworld, num/2, num/2, num/2 - 1, 0, 1, t);
            printVisible(sworld, 6);
            while (true)
            {
                //cout << "move (wasd): ";
                char c;
                if (_kbhit())
                {
                    c = _getch();
                    //cin >> c;
                    if (c == 'w' && sworld[fancyMod((X - 1), num)][Y].walkable)
                        X--;
                    if (c == 's' && sworld[fancyMod((X + 1), num)][Y].walkable)
                        X++;
                    if (c == 'a' && sworld[X][fancyMod((Y - 1), num)].walkable)
                        Y--;
                    if (c == 'd' && sworld[X][fancyMod((Y + 1), num)].walkable)
                        Y++;
                    system("cls");
                    if (sworld[fancyMod(X, num)][fancyMod(Y, num)].tr)
                    {
                        treasureList.push_back(treasure());
                        sworld[X][Y] = tile(rand(), "..");
                    }
                    printVisible(sworld, 6);
                    if (c == 't')
                    {
                        printTreasures();
                    }
                    Sleep(10);
                }
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