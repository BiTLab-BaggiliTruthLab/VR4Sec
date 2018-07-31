#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define ScreenWidth 800
#define ScreenHeight 600

#define ShapeWidth 5
#define ShapeHeight 5

class viewer
{
public:
	~viewer();
	static void displayImage();
private:
	viewer();
};

