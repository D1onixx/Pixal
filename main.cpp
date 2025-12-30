//g++ main.cpp resource.res -o Pixal.exe -static -static-libgcc -static-libstdc++
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <conio.h>

#include <windows.h>

#include "structures.h"

using namespace std;


static void InitGame(void);
static void UpdateGame(void);
static void DrawFrame(void);




//Map info 
static World map;
static string file;

//Game screen
static string** screen;
static string screen_text;

//Player
static Entity player;
static int coins;

//Game process
static string log;
static string command;
static int placed_block;
static bool placeBlock;







int main(int argc, char** argv) {
	if (argc > 1) file = argv[1];
	else file = "NONE";



	funcs f;
	SetConsoleOutputCP(CP_UTF8);
	f.ClearConsole();

	f.Introduce(1);
	cout << "\nPress any button to play (\"q\" - exit)" << endl;
	cout << "(Press \"T(shift + t)\", to see \"File to Map\" tutorial)";
	command = _getch();

	if(command == "T")
	{
		f.maptutorial();
		cout << "\nPress any button to play (\"q\" - exit)" << endl;
		command = _getch();
	}

	if(command == "q") return 0;
	InitGame();

	while(command != "q")
	{
		UpdateGame();
		DrawFrame();
		command = _getch();
	}

	delete [] map.map;
	delete [] screen;

	map.map = nullptr;
	screen = nullptr;

	return 0;
}







void InitGame()
{
	funcs f;

	command = "";

	placeBlock = false;

	player.texture = "@";

	if (file == "NONE")
	{
		map.SizeX = 16;
		map.SizeY = 16;
		map.map = new Block*[map.SizeY];
		for (int i = 0; i < map.SizeX; i ++)
		{
			map.map[i] = new Block[map.SizeX];
		}
		for (int y = 0; y < map.SizeY; y ++)
			for (int x = 0; x < map.SizeX; x ++)
				map.map[y][x] = f.GetBlock(1);
		player.position.x = 0;
		player.position.y = 0;
	}
	else
	{
		f.OpenWorld(map, player, file, coins);
	}
	delete [] screen;
	screen = nullptr;
	screen = new string*[map.SizeY];
	for (int i = 0; i < map.SizeY; i ++)
	{
		screen[i] = new string[map.SizeX];
	}
}


bool CheckHitbox(Entity& object, int dirX, int dirY)
{
	if (object.position.x+dirX < 0 || object.position.x+dirX > map.SizeX-1 || object.position.y+dirY < 0 || object.position.y+dirY > map.SizeY-1) return false;
	if (map.map[object.position.y + dirY][object.position.x + dirX].walkable) return true;
	return false;
}

void UpdateGame()
{
	funcs f;
	f.ClearConsole();

	log = "";

	if (command == "o")
	{
		f.OpenWorld(map, player, "NONE", coins);
		screen = new string*[map.SizeY];
		for (int i = 0; i < map.SizeY; i ++)	screen[i] = new string[map.SizeX];
	}
	if (command == "d")
		if (CheckHitbox(player, 1, 0))
		{
			if (placeBlock) map.map[player.position.y][player.position.x] = f.GetBlock(placed_block);
			placeBlock = false;
			player.position.x ++;
		}
	if (command == "a")
		if (CheckHitbox(player, -1, 0))
		{
			if (placeBlock) map.map[player.position.y][player.position.x] = f.GetBlock(placed_block);
			placeBlock = false;
			player.position.x --;
		}
	if (command == "s")
		if (CheckHitbox(player, 0, 1))
		{
			if (placeBlock) map.map[player.position.y][player.position.x] = f.GetBlock(placed_block);
			placeBlock = false;
			player.position.y ++;
		}
	if (command == "w")
		if (CheckHitbox(player, 0, -1))
		{
			if (placeBlock) map.map[player.position.y][player.position.x] = f.GetBlock(placed_block);
			placeBlock = false;
			player.position.y --;
		}

	if (command == "1")
	{
		placeBlock = true;
		placed_block = 1;
	}
	if (command == "2")
	{
		placeBlock = true;
		placed_block = 2;
	}
	if (command == "3")
	{
		placeBlock = true;
		placed_block = 3;
	}
	if (command == "4")
	{
		placeBlock = true;
		placed_block = 4;
	}

	if (command == "x")
	{
		int circle[9][2] = {
			{0, 0}, {0, 1}, {1, 0}, {1, 1}, {0, -1},
			{-1, 0}, {1, -1}, {-1, 1}, {-1, -1}
		};
		for (int i = 0; i < 9; i ++)
		{
			if (!(player.position.x+circle[i][0] < 0 || player.position.x+circle[i][0] > map.SizeX-1 || player.position.y+circle[i][1] < 0 || player.position.y+circle[i][1] > map.SizeY-1))
			{
			map.map[player.position.y + circle[i][1]][player.position.x + circle[i][0]] = f.GetBlock(1);
			}
		}
	}
	if (command == "m") f.SaveWorld(map, player, coins);

	if (map.map[player.position.y][player.position.x].id == 3)
	{
		map.map[player.position.y][player.position.x] = f.GetBlock(1);
		cout << "\a";
		log = "Picked up a coin";
		coins ++;
	}

	for (int y = 0; y < map.SizeY; y ++)
		for (int x = 0; x < map.SizeX; x ++)
			screen[y][x] = map.map[y][x].texture;
	screen[player.position.y][player.position.x] = player.texture;
}



void DrawFrame()
{
	bool coins_ready = false;
	bool log_ready = false;
	screen_text = "\n/";
	for (int i = 0; i < map.SizeX * 2 - 1; i ++)
	{
		screen_text += "=";
	}
	screen_text += "\\\n";

	

	for (int y = 0; y < map.SizeY; y ++)
	{
			screen_text += "|";
			for (int x 	= 0; x < map.SizeX; x ++)
			{
					screen_text += screen[y][x] + " ";
			}
			screen_text.pop_back();
			screen_text += "|";
			if (coins_ready && !log_ready)
			{
				log_ready = true;
				screen_text += " " + log;
			}
			if (!coins_ready)
			{
				coins_ready = true;
				screen_text += " COINS: " + to_string(coins);
			}

			screen_text += "\n";
	}



	screen_text += "\\";
	for (int i = 0; i < map.SizeX * 2 - 1; i ++)
	{
		screen_text += "=";
	}
	screen_text += "/\n";



	cout << screen_text;
}