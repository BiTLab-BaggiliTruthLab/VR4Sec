// HumanJoystickPathVisualizer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "HumanJoystickPathVisualizer.h"

using namespace sf;

int main()
{
	min_x = 0;
	min_y = 0;
	max_x = 0;
	max_y = 0;

	int scale = 100;
	int shift = 300;

	std::ifstream file;
	std::string file_path = "player_6_raw";
	file.open(file_path + ".txt");

	if (!file) {
		std::cout << "No luck opening file" << std::endl;
		return 0;
	}

	int i = 0;
	while (file >> x >> y) {

		vertices.push_back(sf::Vertex(sf::Vector2f(x * scale + shift, y * scale + shift), Color(0,0,0,255)));

		if (x < min_x) min_x = x;
		if (x > max_x) max_x = x;
		if (y < min_y) min_y = y;
		if (y > max_y) max_y = y;
	
		i++;
	}
	
	printf_s("Min X: %f, Max x: %f, Min Y: %f, Max Y: %f, Dif X %f,  Dif y:  %f,", min_x, max_x, min_y, max_y, max_x - min_x, max_y - min_y);

	RenderWindow window(VideoMode(600, 600), "Path Visualizer", Style::Default);
	window.setFramerateLimit(10);
	

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		

		//Update


		//Draw
		window.clear(Color(255,255,255,255));

		RectangleShape origin_x = RectangleShape(Vector2f(30,2));
		origin_x.setOrigin(0, 1);
		origin_x.setPosition(5, shift);
		origin_x.setFillColor(Color(0, 0, 0, 255));

		RectangleShape origin_x2 = RectangleShape(Vector2f(30, 2));
		origin_x2.setOrigin(0, 1);
		origin_x2.setPosition(600 - 35, shift);
		origin_x2.setFillColor(Color(0, 0, 0, 255));

		RectangleShape origin_y = RectangleShape(Vector2f(2, 30));
		origin_y.setOrigin(0, 1);
		origin_y.setPosition(shift, 600-35);
		origin_y.setFillColor(Color(0, 0, 0, 255));

		RectangleShape origin_y2 = RectangleShape(Vector2f(2, 30));
		origin_y2.setOrigin(0, 1);
		origin_y2.setPosition(shift, 5);
		origin_y2.setFillColor(Color(0, 0, 0, 255));

		RectangleShape circle_center = RectangleShape(Vector2f(2, 2));
		circle_center.setOrigin(1, 1);
		circle_center.setPosition(shift, shift - 1.9 * scale);
		circle_center.setFillColor(Color(255, 0, 0, 255));

		//CircleShape circle = CircleShape(.20*scale);
		//circle.setOutlineThickness(1);
		//circle.setOutlineColor(Color(255, 0, 0, 255));
		//circle.setFillColor(Color(255, 0, 0, 10));
		//circle.setOrigin(Vector2f(circle.getRadius(), circle.getRadius()));
		//circle.setPosition(shift, shift - 1.9*scale);

		window.draw(origin_x);
		window.draw(origin_x2);
		window.draw(origin_y);
		window.draw(origin_y2);
	//	window.draw(circle);
		window.draw(circle_center);
		window.draw(&vertices[0], vertices.size(), sf::LinesStrip);
		window.display();
		
		sf::Texture texture;
		
		//sf::Vector2u windowSize = window.getSize();
		//texture.create(windowSize.x, windowSize.y);
		//texture.update(window);
		//sf::Image screen_shot;
		//screen_shot.create(600, 600, sf::Color(100, 100, 100, 255));
		////screen_shot.saveToFile("adfasdf.png");
		//
	

	}


    return 0;
}

