#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>

using namespace std;

constexpr int MapSize = 50;
constexpr int Rage = 2;
constexpr int SubLife = 5;

struct Position {
    int x, y;
};

struct Mouse {
    Position pos;
    Position rota;
    int life;

    inline void step() {
        pos.x += rota.x;
        pos.y += rota.y;
        life--;
    }

    [[nodiscard]]
    inline bool isAlive() const {
        return life > 0;
    }
};

// 型定義で可読性強化
using Map = vector<vector<int>>;

void draw(const Map& map);
void thunder(Map& map, mt19937& rng);

inline int randInt(mt19937& rng, int min, int max) {
    uniform_int_distribution<> dist(min, max);
    return dist(rng);
}

int main() {
    random_device rd;
    mt19937 rng(rd());

    Map map(MapSize, vector<int>(MapSize));

    while (true) {
        for (auto& row : map)
            fill(row.begin(), row.end(), 0);

        map[0][MapSize / 2] = 1;

        thunder(map, rng);
        draw(map);

        string input;
        cout << "[Enter] で次 / n で終了 > ";
        getline(cin, input);
        if (input == "n") break;
    }

    return 0;
}

void draw(const Map& map) {
    cout << string(MapSize * 2, '-') << endl;
    for (const auto& row : map) {
        for (int cell : row)
            cout << (cell ? "口" : "　");
        cout << '\n';
    }
    cout << string(MapSize * 2, '-') << endl;
}

template <typename T>
inline T clamp(const T& val, const T& low, const T& high) {
    return std::max(low, std::min(val, high));
}


void thunder(Map& map, mt19937& rng) {
    Position mainMouse{ MapSize / 2, 0 };
    Position rotate{ 0, 1 };

    vector<Mouse> mouses;

    while (mainMouse.y < MapSize - 1) {
        mainMouse.x += rotate.x;
        mainMouse.y += rotate.y;

        if (mainMouse.x >= 0 && mainMouse.x < MapSize) {
            if (mainMouse.x > 0)
                map[mainMouse.y][mainMouse.x - 1] = 1;
            map[mainMouse.y][mainMouse.x] = 1;
            if (mainMouse.x < MapSize - 1)
                map[mainMouse.y][mainMouse.x + 1] = 1;
        }

        // ランダムな方向変化と分岐
        if (randInt(rng, 0, Rage - 1) == 0) {
            rotate.x += (randInt(rng, 0, 1) == 0) ? 1 : -1;
            rotate.x = clamp(rotate.x, -1, 1);

            if (randInt(rng, 0, 1) == 0) {
                int branchX = rotate.x + ((randInt(rng, 0, 2) == 0) ? 1 : -1);
                branchX = clamp(branchX, -1, 1);
                mouses.emplace_back(Mouse{ mainMouse, {branchX, 1}, SubLife });
            }
        }

        for (auto& m : mouses) {
            if (m.isAlive()) {
                m.step();
                if (m.pos.x >= 0 && m.pos.x < MapSize && m.pos.y < MapSize)
                    map[m.pos.y][m.pos.x] = 1;
            }
        }

        mouses.erase(remove_if(mouses.begin(), mouses.end(),
            [](const Mouse& m) { return !m.isAlive(); }),
            mouses.end());
    }
}
