#include "Functions.h"
using namespace std;

int path_number = -1;
vector<t_case> direction_options;
static bool isInit = false;

//
int randomDirection() {
	return rand() % 4;
}
//
void init_directionOptions_vector(){
	direction_options.reserve(4);
}

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
void print_all_cases_pathIDs(t_lab& lab, int maxX, int maxY) {
	for (int x = 0; x < maxX; x++) {
		for (int y = 0; y < maxY; y++) {
			printf("PathID at [%d][%d]: %d \n", x, y, lab.cases[x][y].path_id);
		}
	}
}

//
t_path* createPath_ptr() {
	t_path* new_path = new t_path();
	int pathid = path_counter();
	new_path->id = pathid;
	return new_path;
}
//
//
void carvePath(t_lab& lab, t_path* path, int pathid, int x, int y, int sizeX, int sizeY) {
	int fails = 0;
	if ((path->case_ptr.size() > 1) && lab.cases[x][y].isEntrance) return; // stop if you reach another entrance. fuck it.
	path->case_ptr.push_back(&lab.cases[x][y]);
	lab.cases[x][y].visited = true;
	lab.cases[x][y].depth++;
	//draw_matrix(lab);
	//print_path(*path);

	while (fails < 12) {
		int rand_direction = randomDirection();

		//North
		if (rand_direction == 0 && ((y + 1) < sizeY) && lab.cases[x][(y + 1)].visited == false &&
			lab.cases[x][(y + 1)].isEntrance == false) {

			lab.cases[x][y].open_top = true;
			carvePath(lab, path, pathid, x, y + 1, sizeX, sizeY);
			return;
		}
		//East
		else if (rand_direction == 1 && ((x + 1) < sizeX) && lab.cases[(x + 1)][y].visited == false &&
			lab.cases[(x + 1)][y].isEntrance == false) {
			lab.cases[x][y].open_right = true;
			carvePath(lab, path, pathid, x + 1, y, sizeX, sizeY);
			return;
		}
		//South
		else if (rand_direction == 2 && (y - 1 >= 0) && lab.cases[x][(y - 1)].visited == false &&
			lab.cases[x][(y - 1)].isEntrance == false) {
			lab.cases[x][y].open_bot = true;
			carvePath(lab, path, pathid, x, y - 1, sizeX, sizeY);
			return;
		}
		//West
		else if (rand_direction == 3 && (x - 1 >= 0) && lab.cases[(x - 1)][y].visited == false &&
			lab.cases[(x - 1)][y].isEntrance == false) {
			lab.cases[x][y].open_left = true;
			carvePath(lab, path, pathid, x - 1, y, sizeX, sizeY);
			return;
		}
		else {
			fails++;
		}
	}
}

//overload without sizeXY
void carvePath(t_lab& lab, t_path* path, int pathid, int x, int y) {
	//let calculate sizes just once even if that makes the call excruciatingly long
	int sizeX = lab.cases.size();
	int sizeY = lab.cases[x].size();
	carvePath(lab, path, pathid, x, y, sizeX, sizeY);
}

void carvePath_fromAvailable(t_lab& lab, t_path* path, int pathid, int x, int y, int previous_depth) {
	direction_options.clear();
	path->case_ptr.push_back(&lab.cases[x][y]);
	lab.cases[x][y].visited = true;
	previous_depth += 1;
	lab.cases[x][y].depth = previous_depth;
	int sizeX = lab.cases.size();
	int sizeY = lab.cases[x].size();
	int directions;
	
	switch (isInit) {
	case(true):	break;
	case(false): init_directionOptions_vector();
	}

	directions = 0;
	int rand_direction = rand() % 4;
	//check what directions are available and compile them into directionOptions
	if ((y + 1) < sizeY && lab.cases[x][(y + 1)].visited == false && lab.cases[x][(y + 1)].isEntrance == false) {
		direction_options.push_back(lab.cases[x][y + 1]);
		directions++;
	}
	if ((x + 1) < sizeX && lab.cases[x + 1][y].visited == false && lab.cases[x + 1][y].isEntrance == false) {
		direction_options.push_back(lab.cases[x + 1][y]);
		directions++;
	}
	if ((y - 1) > 0 && lab.cases[x][y - 1].visited == false && lab.cases[x][y - 1].isEntrance == false) {
		direction_options.push_back(lab.cases[x][y - 1]);
		directions++;
	}
	if ((x - 1) > 0 && lab.cases[x - 1][y].visited == false && lab.cases[x - 1][y].isEntrance == false) {
		direction_options.push_back(lab.cases[x - 1][y]);
		directions++;
	}
		
	if (directions > 0) {
		rand_direction %= directions;
		if (direction_options[rand_direction].y > y) {
			lab.cases[x][y].open_top = true;
		}
		else if (direction_options[rand_direction].y < y) {
			lab.cases[x][y].open_bot = true;
		}
		else if (direction_options[rand_direction].x > x) {
			lab.cases[x][y].open_right = true;
		}
		else if (direction_options[rand_direction].x < x) {
			lab.cases[x][y].open_left = true;
		}


		carvePath_fromAvailable(lab, path, pathid, direction_options[rand_direction].x, direction_options[rand_direction].y, previous_depth);
	}
}
//
void cellMergingProcess(t_lab& lab) {
	size_t leftovers = countUnvisitedCells(lab, lab.sizeX, lab.sizeY);
}
//
void mergeLoneCell(t_lab& lab, int x, int y, int max_x, int max_y) {
	size_t direction{ 0 };
	int plottedX{}, plottedY{};
	int maxDepth = 0; // retains minimal depth from N E S W directions
	//search for the best adjacent path to be added to
	//best is shortest
	//int randomDir = randomDirection();
	if (y - 1 >= 0) {
		//check N
		if (lab.cases[x][y - 1].depth > maxDepth && lab.cases[x][y - 1].visited) {
			maxDepth = lab.cases[x][y - 1].depth;
			plottedX = x, plottedY = y - 1;
			direction = 1;
		}
	}
	if (x + 1 <= max_x - 1) {
		//E = true;
		if (lab.cases[x + 1][y].depth > maxDepth && lab.cases[x + 1][y].visited) {
			maxDepth = lab.cases[x + 1][y].depth;
			plottedX = x + 1, plottedY = y;
			direction = 2;
		}
	}
	if (y + 1 <= max_y - 1) {
		//S = true;
		if (lab.cases[x][y + 1].depth > maxDepth && lab.cases[x][y + 1].visited) {
			maxDepth = lab.cases[x][y + 1].depth;
			plottedX = x, plottedY = y + 1;
			direction = 3;
		}
	}
	if (x - 1 >= 0) {
		//W = true;
		if (lab.cases[x - 1][y].depth > maxDepth && lab.cases[x - 1][y].visited) {
			maxDepth = lab.cases[x - 1][y].depth;
			plottedX = x - 1, plottedY = y;
			direction = 4;
		}
	}

	t_path* new_path = createPath_ptr();
	/*switch (direction) {
	case 1:
		lab.cases[plottedX][plottedY].open_bot = true;
		break;
	case 2:
		lab.cases[plottedX][plottedY].open_left = true;
		break;
	case 3:
		lab.cases[plottedX][plottedY].open_top = true;
		break;
	case 4:
		lab.cases[plottedX][plottedY].open_right = true;
	}*/

	carvePath_fromAvailable(lab, new_path, path_counter(), plottedX, plottedY, lab.cases[x][y].depth);
	//lab.cases[x][y].visited = true;
	//new_path->case_ptr.push_back(&lab.cases[x][y]);
	//new_path->case_ptr.push_back(&lab.cases[plottedX][plottedY]);
	//lab.paths.push_back(new_path);
}
//
void update_allPathIDs(t_lab& lab) {
	size_t pathSize = lab.paths.size();
	for (size_t i = 0; i < pathSize; i++) {
		size_t caseArraySize = lab.paths[i]->case_ptr.size();
		for (size_t o = 0; o < caseArraySize; o++) {
			lab.paths[i]->case_ptr[o]->path_id = i;
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
int countUnvisitedCells(t_lab& lab, int x, int y) {
	int count = 0;
	for (int i=0; i < x; i++) {
		for (int o=0; o< y; o++){
			
			if (lab.cases[i][o].visited == false) count++;

		}
	}
	return count;
}

//
void processAllUnvisitedCells(t_lab& lab) {
	for (int i = 0; i < lab.sizeX; i++) {
		for (int o = 0; o < lab.sizeY; o++) {
			if (lab.cases[i][o].visited == false) mergeLoneCell(lab, i, o, lab.sizeX, lab.sizeY);
		}
	}
}

//
void nth_pass(t_lab& lab) {
	int o = 0; //iter for t_cases to check
	int opening = 0; //boolean checking for opening
	int nb_paths = lab.paths.size();
	int local_path_size = 0;
	int caseNum;
	quickSort_paths(lab.paths, 0, nb_paths - 1);
	//study shortest paths to add them some extensions.
	for (int i = 0; i < nb_paths; i++) {
		local_path_size = lab.paths[i]->case_ptr.size();
		o = 0;
		opening = false;
		while (!opening && o < local_path_size) {
			caseNum = local_path_size - o - 1;
			opening = check_if_stuck(lab, lab.paths[i]->case_ptr[caseNum]->x, lab.paths[i]->case_ptr[caseNum]->y);
			o++;
		}
		if (opening) {
			printf("found opening on path: %d", lab.paths[i]->id);
			t_path* new_path = createPath_ptr();
			carvePath_fromAvailable(lab, new_path, new_path->id, lab.paths[i]->case_ptr[caseNum]->x, lab.paths[i]->case_ptr[caseNum]->y, lab.paths[i]->case_ptr[caseNum]->depth);
			lab.paths.push_back(new_path);
		}
	}


	
	//print_paths(lab);
}

