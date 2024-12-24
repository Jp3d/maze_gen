#pragma once
#include <vector>

using namespace std;

struct t_case {
	int x;
	int y;
	bool open_right = false;
	bool open_top = false;
	bool open_bot = false;
	bool open_left = false;
	bool visited = false;
	bool isEntrance = false;
	int path_id = -1;
};

struct t_path {
	vector<t_case*> case_ptr;              // pointer to the maze cell this path is at
};

struct t_lab {
	vector<vector<t_case>> cases;  // Use std::vector
	vector<t_case> entrances;
	vector<t_path*> paths;
};
