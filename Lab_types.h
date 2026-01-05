#pragma once
#include <vector>

using namespace std;

struct t_case {
	int x{0};
	int y{0};
	bool open_right = false;
	bool open_top = false;
	bool open_bot = false;
	bool open_left = false;
	bool visited = false;
	bool isEntrance = false;
	bool isRoom = false;
	int path_id = -1;
	int depth = 0;
};

struct t_path {
	vector<t_case*> case_ptr; // list of cell pointers of this path
	int id; //stores an identifier for paths.
	int max;

};

struct t_room {
	int length;
	int height;
	vector<t_case*> roomCases;

};

struct t_lab {
	vector<vector<t_case>> cases;  // Use std::vector
	vector<t_case*> entrances;
	vector<t_path*> paths;
	vector<t_room*> rooms;
	int sizeX{ 1 };
	int sizeY{ 1 };

};
