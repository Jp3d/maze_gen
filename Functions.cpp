#include "Functions.h"


using namespace std;

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
