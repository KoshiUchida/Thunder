#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>

using namespace std;

constexpr int MapSize = 50;
constexpr int Rage = 2;
constexpr int SubLife = 5;

template <typename T>
inline T clamp(const T& val, const T& low, const T& high) {
    return std::max(low, std::min(val, high));
}

struct Position {
    int x, y;
};

class Map2D {
public:
    Map2D(int size) : size(size), map(size, vector<int>(size, 0)) {}

    void clear() {
        for (auto& row : map)
            fill(row.begin(), row.end(), 0);
    }

    void mark(int x, int y) {
        if (inBounds(x, y))
            map[y][x] = 1;
    }

    void draw() const {
        cout << string(size * 2, '-') << '\n';
        for (const auto& row : map) {
            for (int cell : row)
                cout << (cell ? "Œû" : "@");
            cout << '\n';
        }
        cout << string(size * 2, '-') << '\n';
    }

    int getSize() const { return size; }

private:
    int size;
    vector<vector<int>> map;

    bool inBounds(int x, int y) const {
        return x >= 0 && x < size && y >= 0 && y < size;
    }
};

struct Mouse {
    Position pos;
    Position rota;
    int life;

    void step() {
        pos.x += rota.x;
        pos.y += rota.y;
        life--;
    }

    bool isAlive() const {
        return life > 0;
    }
};

class ThunderSimulator {
public:
    ThunderSimulator(Map2D& map, mt19937& rng)
        : map(map), rng(rng), mainPos{ map.getSize() / 2, 0 }, mainDir{ 0, 1 } {}

    void simulate() {
        map.mark(mainPos.x, mainPos.y);

        while (mainPos.y < map.getSize() - 1) {
            advanceMain();
            updateBranches();
        }
    }

private:
    Map2D& map;
    mt19937& rng;
    Position mainPos;
    Position mainDir;
    vector<Mouse> branches;

    int randInt(int min, int max) {
        uniform_int_distribution<> dist(min, max);
        return dist(rng);
    }

    void advanceMain() {
        mainPos.x += mainDir.x;
        mainPos.y += mainDir.y;

        map.mark(mainPos.x, mainPos.y);
        map.mark(mainPos.x - 1, mainPos.y);
        map.mark(mainPos.x + 1, mainPos.y);

        // ƒ‰ƒ“ƒ_ƒ€•Ï‰»
        if (randInt(0, Rage - 1) == 0) {
            mainDir.x += (randInt(0, 1) == 0) ? 1 : -1;
            mainDir.x = clamp(mainDir.x, -1, 1);

            if (randInt(0, 1) == 0) {
                int branchX = clamp(mainDir.x + ((randInt(0, 2) == 0) ? 1 : -1), -1, 1);
                branches.push_back(Mouse{ mainPos, { branchX, 1 }, SubLife });
            }
        }
    }

    void updateBranches() {
        for (auto& m : branches) {
            if (m.isAlive()) {
                m.step();
                map.mark(m.pos.x, m.pos.y);
            }
        }

        branches.erase(remove_if(branches.begin(), branches.end(),
            [](const Mouse& m) { return !m.isAlive(); }),
            branches.end());
    }
};

int main() {
    random_device rd;
    mt19937 rng(rd());

    Map2D map(MapSize);

    while (true) {
        map.clear();
        ThunderSimulator thunder(map, rng);
        thunder.simulate();
        map.draw();

        string input;
        cout << "[Enter] ‚ÅŽŸ / n ‚ÅI—¹ > ";
        getline(cin, input);
        if (input == "n") break;
    }

    return 0;
}
