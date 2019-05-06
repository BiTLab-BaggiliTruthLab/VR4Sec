#include "viewer.h"



viewer::viewer()
{
}


viewer::~viewer()
{
}

void viewer::displayImage() {
	sf::RenderWindow window;
	window.create(sf::VideoMode(ScreenWidth, ScreenHeight), "Camera Feed");
	window.setKeyRepeatEnabled(false);

	sf::Texture pTexture;
	sf::Sprite pImage;

	if (!pTexture.loadFromFile("grumpy.png")) {
		std::cout << "Could not load picture from file" << std::endl;
	}

	pImage.setTexture(pTexture);

	while (window.isOpen())
	{
		sf::Event winEvent;
		switch (window.pollEvent(winEvent))
		{
		case sf::Event::Closed:
			window.close();
			break;
		default:
			break;
		}
		window.draw(pImage);
		window.display();	
	}

}