#pragma once
#include "Lab_types.h"

//Generate entrances 
vector<t_case*> generate_entrances(t_lab& lab, int x, int y);

//
vector<t_room*> generate_rooms(t_lab& lab, int x, int y);

//
int path_counter();

void nth_pass(t_lab& lab);

void carvePath(t_lab& lab, t_path* path, int pathid, int x, int y);