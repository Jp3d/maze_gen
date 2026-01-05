#pragma once
#include "Lab_types.h"

//Generate entrances 
vector<t_case*> generate_entrances(t_lab& lab, int x, int y);

//
void print_paths(t_lab& lab);
//
void print_all_cases_pathIDs(t_lab& lab, int maxX, int maxY);
//
void update_allPathIDs(t_lab& lab);
//
vector<t_room*> generate_rooms(t_lab& lab, int x, int y);
//
int randomDirection();
//
void processAllUnvisitedCells(t_lab& lab);
//
void mergeLoneCell(t_lab& lab, int x, int y, int max_x, int max_y);
//
int path_counter();
//
void carvePath(t_lab& lab, t_path* path, int pathid, int x, int y, int sizeX, int sizeY);

t_path* createPath_ptr();

void nth_pass(t_lab& lab);

void carvePath(t_lab& lab, t_path* path, int pathid, int x, int y, int sizeX, int sizeY);
void carvePath(t_lab& lab, t_path* path, int pathid, int x, int y);

int countUnvisitedCells(t_lab& lab, int x, int y);