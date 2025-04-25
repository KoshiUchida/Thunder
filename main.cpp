#include <iostream>
#include <vector>
#include <Windows.h>
#include <time.h>
#include <random>

using namespace std;

void Draw(const vector<vector<int>>& map, int size);

void Thunder(vector<vector<int>>& map, int size);

int main(void)
{
	srand(time(NULL));

	static constexpr int MapSize{ 50 };

	vector<vector<int>> map;
	while (true)
	{

		for (int i{ 0 }; i < MapSize; i++)
		{
			vector<int> width;
			for (int j{ 0 }; j < MapSize; j++)
				width.push_back(0);
			map.push_back(width);
		}

		map[0][MapSize / 2] = 1;
		//map[1][MapSize / 2] = 1;

		//int count{};
		 
		
		//count++;

		Thunder(map, MapSize);
		Draw(map, MapSize);
		
		//Sleep(500);

		string input;
		cin >> input;

		//if (count % 20 == 0)
		//	Draw(map, MapSize);

		//for (int i{ 0 }; i < MapSize; i++)
		//{
		//	if (map[MapSize - 1][i])
		//		count = 300;
		//}

		if (input == "n")
			break;

		map.clear();
	}

	return 0;
}

void Draw(const vector<vector<int>>& map, int size)
{
	for (int i{ 0 }; i < size; i++)
		cout << "--";
	cout << endl;

	for (int i{ 0 }; i <  size; i++)
	{
		for (int j{ 0 }; j < size; j++)
		{
			if (map[i][j])
				cout << "口";
			else
				cout << "　";
		}
		cout << endl;
	}
	cout << endl;

	for (int i{ 0 }; i < size; i++)
		cout << "--";
	cout << endl;
}

void Thunder(vector<vector<int>>& map, int size)
{
	struct Position
	{
		int x;
		int y;
	};

	Position mainMouse{};
	Position rotate{0, 1};

	for (int i{ 0 }; i < size; i++)
	{
		for (int j{ 0 }; j < size; j++)
		{
			if (map[i][j])
			{
				mainMouse = { j, i };
				i = size;
				break;
			}
		}
	}

	struct Mouse
	{
		Position pos;
		Position rota;
		int life;
	};

	vector<Mouse> mouses;


	// メインが地面に付くまでループ
	while (mainMouse.y < size - 1)
	{
		mainMouse.x += rotate.x;
		mainMouse.y += rotate.y;

		if (0 <= mainMouse.x && mainMouse.x < size)
		{
			if (0 < mainMouse.x)
				map[mainMouse.y][mainMouse.x - 1] = 1;
			map[mainMouse.y][mainMouse.x] = 1;
			if (mainMouse.x < size - 1)
				map[mainMouse.y][mainMouse.x + 1] = 1;
		}

		// 衝突する確率
		static constexpr int Rages{ 2 };
		if (rand() % Rages == 0)
		{
			rotate.x += (rand() % 2 == 0) ? 1 : -1;
			if (rotate.x > 1)
				rotate.x = -1;
			else if (rotate.x < -1)
				rotate.x = 1;

			if (rand() % 2 == 0)
			{
				Mouse newMouse{ mainMouse, {rotate.x += (rand() % 3 == 0) ? 1 : -1, rotate.y}, 5 };
				mouses.push_back(newMouse);
			}
		}

		for (int i{ 0 }; i < mouses.size(); i++)
		{
			if (mouses[i].life > 0)
			{
				mouses[i].pos.x += mouses[i].rota.x;
				if (mouses[i].rota.x > 1)
					mouses[i].rota.x = -1;
				else if (mouses[i].rota.x < -1)
					mouses[i].rota.x = 1;
				mouses[i].pos.y += mouses[i].rota.y;

				if (0 <= mouses[i].pos.x && mouses[i].pos.x < size && mouses[i].pos.y < size)
					map[mouses[i].pos.y][mouses[i].pos.x] = 1;

				mouses[i].life--;

				//if (rand() % 6 == 0)
				//{
				//	mouses[i].rota.x += (rand() % 2 == 0) ? 1 : -1;
				//	if (mouses[i].rota.x > 1)
				//		mouses[i].rota.x = -1;
				//	else if (mouses[i].rota.x < -1)
				//		mouses[i].rota.x = 1;

				//	Mouse newMouse{ mouses[i].pos, {mouses[i].rota.x += (rand() % 3 == 0) ? 1 : -1, mouses[i].rota.y}, 5};
				//	mouses.push_back(newMouse);
				//}
			}
			else
			{
				mouses.erase(mouses.begin() + i);
				i--;
			}
		}
	}

	mouses.clear();
}

//void Thunder(vector<vector<int>>& map, int size)
//{
//	while (true)
//	{
//		int count{};
//
//		for (int i{ 0 }; i < size; i++)
//		{
//			for (int j{ 0 }; j < size; j++)
//			{
//				if (map[i][j])
//					count++;
//
//				int OneCount{ 1 };
//
//				if (i - 1 >= 0 && map[i - 1][j])
//					OneCount += 16;
//				if (i - 2 >= 0 && map[i - 2][j])
//					OneCount -= 10;
//				if (i + 1 <= size - 1 && map[i + 1][j])
//					OneCount -= 10;
//				if (i + 2 <= size - 2 && map[i + 2][j])
//					OneCount -= 5;
//				if (i - 1 >= 0 && j - 1 >= 0 && map[i - 1][j - 1])
//					OneCount += 5;
//				if (i - 1 >= 0 && j + 1 <= size - 1 && map[i - 1][j + 1])
//					OneCount += 5;
//				if (i + 1 <= size - 1 && j - 1 >= 0 && map[i + 1][j - 1])
//					OneCount++;
//				if (i + 1 <= size - 1 && j + 1 <= size - 1 && map[i + 1][j + 1])
//					OneCount++;
//
//				if (!map[i][j] && OneCount > 1 && rand() % (size * size / OneCount) == 0)
//				{
//					map[i][j] = 1;
//					return;
//				}
//			}
//		}
//
//		if (count == size * size)
//			break;
//	}
//}
