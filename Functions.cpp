#include "Functions.h"


using namespace std;

int path_number = -1;



//
bool check_if_stuck(t_lab& lab, int x, int y) {

	int sizeX = lab.cases.size();
	int sizeY = lab.cases[x].size();
	//North 
	if ((y + 1) < sizeY && lab.cases[x][y + 1].visited == false) return true;
	else if ((x + 1) < sizeX && lab.cases[x + 1][y].visited == false) return true;
	else if ((y - 1) > 0 && lab.cases[x][y - 1].visited == false) return true;
	else if ((x - 1) > 0 && lab.cases[x - 1][y].visited == false) return true;
	else return false;
}

//
void split_path_from_tip(t_lab& lab, int pathId, int max_index) {

	t_case* temp_tcase = lab.paths[pathId]->case_ptr[max_index];
	if (!check_if_stuck(lab, temp_tcase->x, temp_tcase->y)) {
		split_path_from_tip(lab, pathId, max_index - 1);
	}
	else {
		//carve path
	}

}

//
void quickSort_paths(vector<t_path*>& paths, int low, int high) {
	if (low < high) {
		// Use the last element's case_ptr size as pivot
		int pivot = paths[high]->case_ptr.size();
		int i = low - 1;

		for (int j = low; j <= high - 1; j++) {
			if (paths[j]->case_ptr.size() <= pivot) {
				i++;
				// Swap the pointers
				t_path* temp = paths[i];
				paths[i] = paths[j];
				paths[j] = temp;
			}
		}
		// Put pivot in its correct position
		t_path* temp = paths[i + 1];
		paths[i + 1] = paths[high];
		paths[high] = temp;

		int pi = i + 1;

		// Recursively sort the two partitions
		quickSort_paths(paths, low, pi - 1);
		quickSort_paths(paths, pi + 1, high);
	}
}

//
void print_paths(t_lab& lab) {
	int id = 0;
	int pathSize = lab.paths.size();
	for (int i = 0; i < pathSize; i++) {
		id = lab.paths[i]->id;
		printf("path id: %d\n", id);
		int pathdepth = lab.paths[i]->case_ptr.size();
		printf("Depth: %d\n", pathdepth);
	}
}

//
void carvePath(t_lab& lab, t_path* path, int pathid, int x, int y) {
	int fails = 0;
	if ((path->case_ptr.size() > 1) && lab.cases[x][y].isEntrance) return; // stop if you reach another entrance. fuck it.
	path->case_ptr.push_back(&lab.cases[x][y]);
	lab.cases[x][y].visited = true;
	int sizeX = lab.cases.size();
	int sizeY = lab.cases[x].size();
	//draw_matrix(lab);
	//print_path(*path);

	while (fails < 12) {
		int rand_direction = rand() % 4;

		//North
		if (rand_direction == 0 && ((y + 1) < sizeY) && lab.cases[x][(y + 1)].visited == false &&
			lab.cases[x][(y + 1)].isEntrance == false) {

			lab.cases[x][y].open_top = true;
			carvePath(lab, path, pathid, x, y + 1);
			return;
		}
		//East
		else if (rand_direction == 1 && ((x + 1) < sizeX) && lab.cases[(x + 1)][y].visited == false &&
			lab.cases[(x + 1)][y].isEntrance == false) {
			lab.cases[x][y].open_right = true;
			carvePath(lab, path, pathid, (x + 1), y);
			return;
		}
		//South
		else if (rand_direction == 2 && (y - 1 >= 0) && lab.cases[x][(y - 1)].visited == false &&
			lab.cases[x][(y - 1)].isEntrance == false) {
			lab.cases[x][y].open_bot = true;
			carvePath(lab, path, pathid, x, (y - 1));
			return;
		}
		//West
		else if (rand_direction == 3 && (x - 1 >= 0) && lab.cases[(x - 1)][y].visited == false &&
			lab.cases[(x - 1)][y].isEntrance == false) {
			lab.cases[x][y].open_left = true;
			carvePath(lab, path, pathid, (x - 1), y);
			return;
		}
		else {
			fails++;
		}
	}
}

//
vector<t_case*> generate_entrances(t_lab& lab, int x, int y) {
	vector<t_case*> entrancelist;

	int rand_numX = (rand() % 2)+1;
	int rand_numY = (rand() % 2)+1;

	if (x > 12) rand_numX *= 2;
	else if (x > 24) rand_numX *= 3;
	else if (x > 40) rand_numX *= 4;

	if (y > 12)	rand_numY *= 2;
	else if (y > 24)rand_numY *= 3;
	else if (y > 40)rand_numY *= 4;

	int stepx = x / rand_numX;
	int stepy = y / rand_numY;
	for (int i = rand_numX-1; i > 0; i--) {
		entrancelist.push_back(&lab.cases[x-(i * stepx)][0]);
		lab.cases[x - (i * stepx)][0].isEntrance = true;
		entrancelist.push_back(&lab.cases[x-(i * stepx)][y-1]);
		lab.cases[x - (i * stepx)][y - 1].isEntrance = true;
	}
	for (int o = rand_numY-1; o > 0; o--) {
		entrancelist.push_back(&lab.cases[0][y-(o* stepy)]);
		lab.cases[0][y - (o * stepy)].isEntrance = true;
		entrancelist.push_back(&lab.cases[x-1][y-(o * stepy)]);
		lab.cases[x - 1][y - (o * stepy)].isEntrance = true;
	}
	return entrancelist;
}

//
vector<t_room*> generate_rooms(t_lab& lab, int x, int y) {
	
	vector<t_room*> roomList;
	t_room* room = new t_room;

	int maxRoom_X = (x / 8);
	int maxRoom_Y = (x / 8);
	maxRoom_X = (rand() % maxRoom_X);
	maxRoom_Y = (rand() % maxRoom_Y);
	if (maxRoom_X == 0) maxRoom_X = 1;
	if (maxRoom_Y == 0) maxRoom_Y = 1;
	maxRoom_X *= 2;
	maxRoom_Y *= 2;

	room->height = maxRoom_X;
	room->length = maxRoom_Y;
	

	int start_x = (x / 2) - (maxRoom_X / 2);
	int start_y = (y / 2) - (maxRoom_Y / 2);
	
	for (int i = start_x; i < (start_x + maxRoom_X); i++) {
		for (int j = start_y; j < (start_y + maxRoom_Y); j++) {
			//add rooms to room
			lab.cases[i][j].visited = true;
			lab.cases[i][j].isRoom = true;
			room->roomCases.push_back(&lab.cases[i][j]);
		}
	}
	roomList.push_back(room);
	return roomList;
}

//
int path_counter() {
	path_number++;
	return path_number;
}

//
void nth_pass(t_lab& lab) {
	int nb_paths = lab.paths.size(); 
	quickSort_paths(lab.paths, 0, nb_paths - 1);
	print_paths(lab);
	
}

