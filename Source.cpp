#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <math.h>
#include <iostream>
#include <vector>

#include "Functions.h"
#include "Lab_types.h"

using namespace std;
using namespace sf;

constexpr int windowWidth{ 800 }, windowHeight{ 600 };

#define MAX_X 16
#define MAX_Y 12
#define CELL 18


void print_path(t_path path) {
	int size = path.case_ptr.size();
	for (int i = 0; i < size; i++) {
		printf("[%d][%d]\n", path.case_ptr[i]->x, path.case_ptr[i]->y);
	}
}

void draw_matrix(t_lab& lab) {
	for (int y = 0; y < lab.cases[0].size(); y++) {
		for (int x = 0; x < lab.cases.size(); x++) {
			std::cout << "[";

			if (!lab.cases[x][y].visited) {
				std::cout << "0";
			}
			else {
				std::string cell = "";
				if (lab.cases[x][y].open_top) cell += "T";
				if (lab.cases[x][y].open_right) cell += "R";
				if (lab.cases[x][y].open_bot) cell += "B";
				if (lab.cases[x][y].open_left) cell += "L";
				std::cout << cell;
			}

			std::cout << "]";
		}
		std::cout << std::endl;  // new line after each row
	}
}

void carvePath(t_lab& lab, t_path* path, int x, int y) {
	int fails = 0;
	if ((path->case_ptr.size() > 1) && lab.cases[x][y].isEntrance) return; // stop if you reach another entrance. fuck it.
	path->case_ptr.push_back(&lab.cases[x][y]);
	lab.cases[x][y].visited = true;
	int sizeX = lab.cases.size();
	int sizeY = lab.cases[x].size();
	draw_matrix(lab);
	print_path(*path);

	while(fails < 5) {
		int rand_direction = rand() % 4;
		

		//North
		if (rand_direction == 0 && ((y + 1) < sizeY) && lab.cases[x][(y + 1)].visited ==false) {
			lab.cases[x][y].open_top = true;
			printf("reached NORTH\n");
			carvePath(lab, path, x, y + 1);
			return;
		}
		//East
		else if (rand_direction == 1 && ((x + 1) < sizeX) && lab.cases[(x + 1)][y].visited ==false) {
			lab.cases[x][y].open_right = true;
			printf("reached EAST\n");
			carvePath(lab, path, (x+1) , y);
			return;
		}
		//South
		else if (rand_direction == 2 && (y - 1 >= 0) && lab.cases[x][(y - 1)].visited == false) {
			lab.cases[x][y].open_bot = true;
			printf("reached SOUTH\n");
			carvePath(lab, path, x, (y-1));
			return;
		}
		//West
		else if (rand_direction == 3 && (x - 1 >= 0) && lab.cases[(x - 1)][y].visited == false) {
			lab.cases[x][y].open_left = true;
			printf("reached WEST\n");
			carvePath(lab, path, (x - 1), y);
			return;
		}
		else { 
			fails++; 
		}
	}
}

void draw_labyrinth(t_lab& lab, RenderWindow& window) {
	int x_cases = lab.cases.size();
	int y_cases = lab.cases[0].size();
	int halfcell = (CELL / 2);

	float pos_left = (800 / 2) - (MAX_X * CELL) / 2;
	float pos_bot = (600 / 2) - (MAX_Y * CELL) / 2;
	sf::RectangleShape caserect({ CELL - 2, CELL - 2 });
	caserect.setFillColor(sf::Color::Black);
	for (int x = 0; x < x_cases; x++) {
		for (int y = 0; y < y_cases; y++) {
			float xpos = (CELL * x) + pos_left;
			float ypos = (CELL * y) + pos_bot;
			if (lab.cases[x][y].visited) {
				caserect.setPosition({ xpos , ypos });
				window.draw(caserect);
			}
			if (lab.cases[x][y].isEntrance) {
				//wall is east
				if (x == (MAX_X - 1)) {
					caserect.setPosition({ xpos + halfcell, ypos});
					window.draw(caserect);
				}
				//wall is west
				else if (x == 0) {
					caserect.setPosition({ xpos - halfcell, ypos });
					window.draw(caserect);
				}
				//wall is north
				else if (y == 0) {
					caserect.setPosition({ xpos, ypos- halfcell });
					window.draw(caserect);
				}
				//wall is south
				else {
					caserect.setPosition({ xpos, ypos + halfcell });
					window.draw(caserect);
				}
			}
			if (lab.cases[x][y].open_top) {
				caserect.setPosition({xpos, ypos + halfcell });
				window.draw(caserect);
			}
			if (lab.cases[x][y].open_right) {
				caserect.setPosition({ (xpos + halfcell), (ypos) });
				window.draw(caserect);
			}
			if (lab.cases[x][y].open_bot) {
				//caserect.setSize({ 20 - 2, 22 });
				caserect.setPosition({ xpos , ypos- halfcell });
				window.draw(caserect);
			}
			if (lab.cases[x][y].open_left) {
				//caserect.setSize({ 22 , 20 - 2 });
				caserect.setPosition(Vector2f(xpos- halfcell, ypos ));
				window.draw(caserect);
			}
		}
	}
}

	void generate_labyrinth(RenderWindow& window) {
	srand(static_cast<unsigned int>(time(0)));
	t_lab lab;

	// First resize for Y rows (vertical)
	lab.cases.resize(MAX_X);  // Changed to nb_cases_X

	// Then resize each row to have Y columns
	for (int i = 0; i < MAX_X; i++) {  // Changed to nb_cases_X
		lab.cases[i].resize(MAX_Y);     // Changed to nb_cases_Y
	}

	//set every t_case its own coordinates.
	for (int y = 0; y < MAX_Y; y++) { // Outer loop for rows (Y-coordinate)
		for (int x = 0; x < MAX_X; x++) { // Inner loop for columns (X-coordinate)
			lab.cases[x][y].y = y;
			lab.cases[x][y].x = x;
			lab.cases[x][y].visited = false;
		}
	}

	//Randomize entrances by size and factor
	vector<t_case*> entrance;
	entrance = generate_entrances(lab, MAX_X, MAX_Y);

	int entrance_size = entrance.size();

	// Range-based for loop (modern C++)
	for (int i = 0; i < entrance_size; i++) {
		t_path* new_path = new t_path();
		carvePath(lab, new_path, entrance[i]->x, entrance[i]->y);
		lab.paths.push_back(new_path);
	}
	//fill empty cells

	//render
	draw_labyrinth(lab, window);
}

void draw_contour(RenderWindow &window) {
	//centering the labyrinth on screen.
	float pos_left = (800 / 2) - (MAX_X * CELL) / 2;
	float pos_top = (600 / 2) - (MAX_Y * CELL) / 2;
	sf::RectangleShape rect({ MAX_X * CELL, MAX_Y * CELL });
	rect.setPosition({ pos_left, pos_top });
	rect.setFillColor(sf::Color::White);
	rect.setOutlineThickness(2);

	window.draw(rect);
}

// Draw vertical lines
int main() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	//debugs
	Text posText;
	Font myFont;
	myFont.loadFromFile("Rubik-Bold.ttf");
	posText.setCharacterSize(18);
	posText.setPosition(windowWidth - 220, 20);
	posText.setFillColor(Color::Black);
	posText.setStyle(Text::Bold);
	posText.setFont(myFont);

	RenderWindow window{ {windowWidth, windowHeight }, "CPP" };
	window.setFramerateLimit(30);
	window.setVerticalSyncEnabled(false);

	//GameLoop
	while (window.isOpen())
	{



		window.clear(sf::Color::Black); // Use sf::
		draw_contour(window);
		generate_labyrinth(window);
		window.display(); // Now display everything that has been drawn
		sf::Event event;
		while (window.waitEvent(event)) {  // waitEvent instead of game loop
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			}
		}

		return 0;
	}
}