#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <windows.h>

using namespace std;

//list of tiles. mostly for copy pasting, i doubt ill ever uncomment it
//string tiles[8] = { "██", "▓▓", "▒▒", "░░", "  ", "??", "  ", "██" };
//RAND_MAX



//line of code I stole from my "world" project
vector<vector<double>> world;

int depth = 0;
int X = 0;
int Y = 0;
double money = 0;

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

class tile
{
private:
    int seed;
    string shape;
public:
    string vis = "";
    bool walkable = false;
    tile(int s, string h)
    {
        shape = h;
        seed = s;
    }
};

int main()
{
    srand(start);
    int size;
    cin >> size;
    world = vector(size, vector<double>(size, 0));
    SetConsoleOutputCP(CP_UTF8);
    rgbTest();
}

