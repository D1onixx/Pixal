#pragma once

#include <windows.h>
#include <fstream>
#include <deque>

using namespace std;

struct Vector2 {
	int x;
	int y;
};

struct Block {
	int id;
	char texture;
	string name;
	bool walkable;
};
struct Entity {
	Vector2 position;
	string texture;
	string name;
	int health;
};
struct World
{
	int SizeX;
	int SizeY;
	Block** map;
};

class funcs
{
public:

	Block error = {0, '!', "error", true};
	Block floor = {1, ' ', "floor", true};
	Block wall  = {2, '#', "wall", false};
	Block coin  = {3, 'o', "coin", true};
	Block x  = {4, '.', "x", true};

	Block GetBlock(int id)
	{
		switch (id)
		{
			case 0: return error;
			case 1: return floor;
			case 2: return wall;
			case 3: return coin;
			case 4: return x;
		}
		return error;
	}
	void ClearConsole()
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD written;
		if (!GetConsoleScreenBufferInfo(h, &csbi)) return;

		DWORD size = csbi.dwSize.X * csbi.dwSize.Y;

		FillConsoleOutputCharacter(h, L' ', size, {0, 0}, &written);

		SetConsoleCursorPosition(h, {0, 0});
	}



	void OpenWorld(World& map, Entity& player, string argument_world_name, int& coins)
	{
		delete [] map.map;
		map.map = nullptr;
		if (!(argument_world_name == "NONE"))
		{
			ifstream fin(argument_world_name);

			fin >> map.SizeX;
			fin >> map.SizeY;

			cout << argument_world_name << " from arguments" << endl;

			int current_id;
			map.map = new Block*[map.SizeY];
			for (int y = 0; y < map.SizeY; y ++)
			{
				map.map[y] = new Block[map.SizeX];
				for (int x = 0; x < map.SizeX; x ++)
				{
					fin >> current_id;
					map.map[y][x] = GetBlock(current_id);
				}
			}
			fin >> player.position.x;
			fin >> player.position.y;
			fin >> coins;
			fin.close();
		}
		else
		{
			string world_name;
			cout << "===Loading map from file===\n";
			cout << "Input file name(without \".txt\"): ";
			cin >> world_name;
			world_name += ".txt";
			ifstream fin(world_name);

			fin >> map.SizeX;
			fin >> map.SizeY;

			int current_id;
			map.map = new Block*[map.SizeY];
			for (int y = 0; y < map.SizeY; y ++)
			{
				map.map[y] = new Block[map.SizeX];
				for (int x = 0; x < map.SizeX; x ++)
				{
					fin >> current_id;
					map.map[y][x] = GetBlock(current_id);
				}
			}
			fin >> player.position.x;
			fin >> player.position.y;
			fin >> coins;
			fin.close();
		}
	}



	void SaveWorld(World& map, Entity& player, int& coins)
	{
		string world_name;
		cout << "===Saving map to file===\n";
		cout << "Input file name(without \".txt\"): ";
		cin >> world_name;
		world_name += ".txt";

		ofstream fout(world_name);

		fout << map.SizeX << " " << map.SizeY << endl;
		for (int y = 0; y < map.SizeY; y ++)
		{
			for (int x = 0; x < map.SizeX; x ++)
			{
				fout << map.map[y][x].id << " ";
			}
			fout << endl;
		}
		fout << player.position.x << " " << player.position.y << "\n" << coins;
		
		fout.close();
	}



	void Introduce(int decision)
	{
		if (decision == 1)
		{
			cout << "================================TUTORIAL================================" << endl;
			
			cout << "Symbols:" << endl;
			cout << "	@ - YOU" << endl;
			cout << "	  - floor" << endl;
			cout << "	# - wall" << endl;
			cout << "	o - coin" << endl;
			cout << "	. - nothing" << endl;
			cout << "	! - error block(generation issue)" << endl;
			cout << "Binds: " << endl;

			cout << "	awsd - movement" << endl;
			cout << "	1, ..., 4(numbers) - place block on player's position:" << endl;
			cout << "		1 - \".\"" << endl;
			cout << "		2 - \"#\"" << endl;
			cout << "		3 - \"o\"" << endl;
			cout << "		4 - \"X\"" << endl;
			cout << "		(blocks are listed above)" << endl;
			cout << "	x - remove block around the player" << endl;
			cout << "	o - open map from file from game's directory" << endl;
			cout << "	m - save map in file in game's directory" << endl;
		}
	}
	void maptutorial()
	{
		ClearConsole();
		cout << "**************************************MAP TUTORIAL**************************************\n";
		cout << "To create map with file, at first create a text file:\n";
		cout << "Every space means beginning of new number(part of information)\n";
		cout << "First two numbers - Size of map in Х and У directions\n";
		cout << "Then X*Y(map size) amount of numbers - blocks on map\n";
		cout << "Last two numbers - player's starting position\n";
		cout << "Map Example(Comments're in brackets, don't write them in file!):\n";
		cout << "========================================================================================\n";
		cout << "4 4 (Map size is 4х4, then next 16 numbers - are blocks on map)\n";
		cout << "1 1 1 1(blocks)\n";
		cout << "1 1 1 1(blocks)\n";
		cout << "1 1 1 1(blocks)\n";
		cout << "1 1 1 1(blocks)\n";
		cout << "0 0(player will spawn in the top left corner)\n";
		cout << "========================================================================================\n";
		cout << "Using \"Enter\" means the same as using \"space\"";
		cout << "That is, you can write everything in one row, or divide every number with \"Enter\".\n";
		cout << "Player starting coordinates can't be higher than map size or lower from 0\n";
		cout << "All others - Block IDs, ID defines which block will be in game:\n";
		cout << "1 - floor, 2 - wall, 3 - coin, 4 - nothing.\n";
	}
};