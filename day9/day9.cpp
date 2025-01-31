// day9.cpp
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

typedef struct {
    int y, x;
} point;

vector<point> low;
vector<vector<uint8_t>> map;
set<point> basin;
vector<unsigned> basins;
int w, h;

bool operator<(point const& a, point const& b) {
    if (a.x < b.x)
        return true;
    if (a.x > b.x)
        return false;
    return a.y < b.y;
}

bool is_low(int y, int x) {
    int mapyx = map[y][x];
    if ((x > 0) && (mapyx >= map[y][x - 1]))
        return false;
    if ((x + 1 < w) && (mapyx >= map[y][x + 1]))
        return false;
    if ((y > 0) && (mapyx >= map[y - 1][x]))
        return false;
    if ((y + 1 < h) && (mapyx >= map[y + 1][x]))
        return false;
    return true;
}

unsigned part1(void) {
    string s;
    ifstream file("day9.txt");
    for (;;) {
        string line;
        getline(file, line);
        if (file.eof())
            break;
        stringstream ss(line);
        vector<uint8_t> row;
        for (;;) {
            uint8_t n;
            ss >> n;
            if (ss.eof())
                break;
            row.push_back(n - '0');
        }
        map.push_back(row);
    }
    w = map[0].size();
    h = map.size();
    int risk = 0;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (is_low(y, x)) {
                risk += (int)map[y][x] + 1;
                point p = {y, x};
                low.push_back(p);
            }
    return risk;
}

void scan_basin(int y, int x);

void add_basin_point(int y, int x) {
    point p = {y, x};
    if (basin.find(p) != basin.end())
        return;
    basin.insert(p);
    scan_basin(y, x);
}

void scan_basin(int y, int x) {
    int mapyx = map[y][x];
    add_basin_point(y, x);
    if ((x > 0) && (mapyx < map[y][x - 1]) && (map[y][x - 1] != 9))
        add_basin_point(y, x - 1);
    if ((x + 1 < w) && (mapyx < map[y][x + 1]) && (map[y][x + 1] != 9))
        add_basin_point(y, x + 1);
    if ((y > 0) && (mapyx < map[y - 1][x]) && (map[y - 1][x] != 9))
        add_basin_point(y - 1, x);
    if ((y + 1 < h) && (mapyx < map[y + 1][x]) && (map[y + 1][x] != 9))
        add_basin_point(y + 1, x);
}

unsigned part2(void) {
    for (auto& p : low) {
        basin.clear();
        scan_basin(p.y, p.x);
        basins.push_back(basin.size());
    }
    sort(basins.begin(), basins.end());
    return basins[basins.size() - 1] * basins[basins.size() - 2] * basins[basins.size() - 3];
}

int main() {
    cout << "Part 1 - " << part1() << endl;
    cout << "Part 2 - " << part2() << endl;
    return 0;
}
