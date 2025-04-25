#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <string>

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
};

void Draw(const vector<vector<int>>& map);
void Thunder(vector<vector<int>>& map, mt19937& rng);

int main() {
    random_device rd;
    mt19937 rng(rd());

    vector<vector<int>> map(MapSize, vector<int>(MapSize));

    while (true) {
        fill(map.begin(), map.end(), vector<int>(MapSize, 0));
        map[0][MapSize / 2] = 1;

        Thunder(map, rng);
        Draw(map);

        string input;
        cout << "[Enter] で次 / n で終了 > ";
        getline(cin, input);
        if (input == "n") break;
    }

    return 0;
}

void Draw(const vector<vector<int>>& map) {
    cout << string(MapSize * 2, '-') << endl;
    for (const auto& row : map) {
        for (int cell : row)
            cout << (cell ? "口" : "　");
        cout << endl;
    }
    cout << string(MapSize * 2, '-') << endl;
}

void Thunder(vector<vector<int>>& map, mt19937& rng) {
    uniform_int_distribution<> rageDist(0, Rage - 1);
    uniform_int_distribution<> dirDist(0, 1);
    uniform_int_distribution<> branchChance(0, 2);

    Position mainMouse = { MapSize / 2, 0 };
    Position rotate = { 0, 1 };

    vector<Mouse> mouses;

    while (mainMouse.y < MapSize - 1) {
        mainMouse.x += rotate.x;
        mainMouse.y += rotate.y;

        if (mainMouse.x > 0) map[mainMouse.y][mainMouse.x - 1] = 1;
        map[mainMouse.y][mainMouse.x] = 1;
        if (mainMouse.x < MapSize - 1) map[mainMouse.y][mainMouse.x + 1] = 1;

        // ランダム分岐
        if (rageDist(rng) == 0) {
            rotate.x += (dirDist(rng) == 0) ? 1 : -1;
            if (rotate.x > 1) rotate.x = -1;
            else if (rotate.x < -1) rotate.x = 1;

            if (dirDist(rng) == 0) {
                int branchX = rotate.x + ((branchChance(rng) == 0) ? 1 : -1);
                branchX = max(-1, min(1, branchX));
                mouses.push_back(Mouse{ mainMouse, {branchX, 1}, SubLife });
            }
        }

        // サブ雷の更新
        for (auto& m : mouses) {
            if (m.life > 0) {
                m.pos.x += m.rota.x;
                m.pos.y += m.rota.y;

                if (m.pos.x >= 0 && m.pos.x < MapSize && m.pos.y < MapSize)
                    map[m.pos.y][m.pos.x] = 1;

                m.life--;
            }
        }

        // 消滅処理
        mouses.erase(remove_if(mouses.begin(), mouses.end(), [](const Mouse& m) {
            return m.life <= 0;
            }), mouses.end());
    }
}
