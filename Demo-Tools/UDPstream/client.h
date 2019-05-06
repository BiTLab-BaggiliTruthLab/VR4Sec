#pragma once


#include <string>
#include <iostream>
#include <SFML\Network.hpp>
#include "viewer.h"
#include "camera.h"
#include <SFML\Graphics.hpp>


const int BUF_LEN = 65540;
const int PACKET_SIZE = 4096;
const int i_width = 612;
const int i_height = 460;

sf::RenderWindow window;
sf::Image image;
sf::Texture texture;
sf::Sprite sprite;

//string server_ip;


uint8_t buffer[BUF_LEN];
size_t recieved;
sf::IpAddress rIp;

uint8_t* p_rBuff;
bool done = false;		//Loop control once data transfer begins, currently unused