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

#define MAX_X 40
#define MAX_Y 28
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

			if (lab.cases[x][y].isRoom) {
				std::cout << "M";
			}
			else {
				std::string cell = "";
				//if (lab.cases[x][y].isRoom) cell += "M";
				if (lab.cases[x][y].open_top) cell += "T";
				if (lab.cases[x][y].open_right) cell += "R";
				if (lab.cases[x][y].open_bot) cell += "B";
				if (lab.cases[x][y].open_left) cell += "L";
				if (!lab.cases[x][y].visited) cell += "V";
				std::cout << cell;
			}

			std::cout << "]";
		}
		std::cout << std::endl;  // new line after each row
	}
}

//
void draw_labyrinth(t_lab& lab, RenderWindow& window) {
	Color pathColor;
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
			size_t colorValue = lab.cases[x][y].depth % 255;
			caserect.setFillColor(Color(colorValue,colorValue,colorValue));
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

//
void draw_room(RenderWindow& window, t_lab& lab) {

	float room_height = (lab.rooms[0]->height * CELL)-2; //pixel size
	float room_lenght = (lab.rooms[0]->length * CELL)-2;
	
	float pos_left = (800 / 2) - (room_height/2);
	float pos_bot = (600 / 2) - (room_lenght/2);


	sf::RectangleShape caserect({ room_height, room_lenght });
	caserect.setFillColor(sf::Color::Black);
	caserect.setPosition(Vector2f(pos_left-1, pos_bot-1));
	window.draw(caserect);

}

//
void generate_labyrinth(RenderWindow& window) {
	int casesLeft;
	srand(static_cast<unsigned int>(time(0)));
	t_lab lab;
	lab.sizeX = MAX_X, lab.sizeY = MAX_Y;

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
			lab.cases[x][y].isEntrance = false;
			lab.cases[x][y].isRoom = false;
		}
	}

	//Randomize entrances by size and factor
	lab.entrances = generate_entrances(lab, MAX_X, MAX_Y);
	lab.rooms = generate_rooms(lab,MAX_X,MAX_Y);
	int entrance_size = lab.entrances.size();

	//initial go
	for (int i = 0; i < entrance_size; i++) {
		t_path* new_path = createPath_ptr();
		carvePath(lab, new_path, new_path->id, lab.entrances[i]->x, lab.entrances[i]->y);
		lab.paths.push_back(new_path);
	}
	casesLeft = countUnvisitedCells(lab, MAX_X - 1, MAX_Y - 1);
	printf("Casesleft: %d\n", casesLeft);
	while(casesLeft > 20){
		printf("Casesleft: %d\n", casesLeft);
		nth_pass(lab);
		casesLeft = countUnvisitedCells(lab, MAX_X - 1, MAX_Y - 1);
	}
	processAllUnvisitedCells(lab);
	update_allPathIDs(lab);
	//print_all_cases_pathIDs(lab, MAX_X - 1, MAX_Y - 1);
	
	draw_labyrinth(lab, window);
	draw_room(window, lab);
	//draw_matrix(lab);
	//print_paths(lab);
	
}

//
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

	RenderWindow window{ {windowWidth, windowHeight }, "mazegenCPP" };
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