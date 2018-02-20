#include "client.h"

using namespace std;


int main(){

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::IpAddress sendIP("171.19.38.76");
	clock_t last_cycle = clock();
	
	uint8_t *recBuf = new uint8_t[275 * PACKET_SIZE];
	
	cout << "Enter s for server c for client:  " << endl;
	cin >> connectiontype;

	string text = "Connected to";

	socket.bind(port);

	if (connectiontype == 's')
	{
		while(true)
		{
			socket.receive(buffer, sizeof(buffer), recieved, rIp, port);
			if (recieved > 0)
			{
				cout << "Connected to " << rIp << "Port: " << port << endl;
			
				window.create(sf::VideoMode(i_width, i_height), "Camera Feed");
				window.setKeyRepeatEnabled(false);
				window.setPosition(sf::Vector2i(10, 50));

				image.create(i_width, i_width, sf::Color::Black);
				texture.loadFromImage(image);
				sprite.setTexture(texture);

				window.draw(sprite);
				window.display();
				break;
			}
		} 
	}

	else {
		string sIp;	
		cout << "Connecting" << endl;
		socket.send(text.c_str(), text.length() + 1, sendIP, port);
	}


	while (!done)
	{
		if (connectiontype == 's')
		{
			sf::IpAddress tempIp;
			unsigned short tempPort;

			//Listen until receive the packet that contains the number of packets to expect
			do {
				socket.receive(buffer, sizeof(buffer), recieved, tempIp, tempPort);
			//	cout << "Recieved First Packet: " << recieved << endl;
			} while (recieved > sizeof(int));
			int total_pack = ((int *)buffer)[0];

			//cout << "expecting length of packs:" << total_pack << endl;
			
			for (int i = 0; i < total_pack - 1; i++)
			{
				socket.receive(buffer, sizeof(buffer), recieved, tempIp, tempPort);

				if (recieved != PACKET_SIZE) {
					//cerr << "Received unexpected size pack:" << recieved << endl;
					continue;
				}
				
				int sequence_number = buffer[PACKET_SIZE - 4 - 1] + buffer[PACKET_SIZE - 3 - 1] * 256;
				memcpy(&recBuf[sequence_number * (PACKET_SIZE - 4)], buffer, PACKET_SIZE - 4);
			}
			
			p_rBuff = recBuf;
			for (int y = 0; y < i_height; y++) {
				for (int x = 0; x < i_width; x++)
				{
					image.setPixel(x, y, sf::Color(p_rBuff[0], p_rBuff[1], p_rBuff[2]));
					p_rBuff += 4;
				}
			}

			texture.loadFromImage(image);
			sprite.setTexture(texture);
			window.draw(sprite);
			window.display();

			clock_t next_cycle = clock();
			double duration = (next_cycle - last_cycle) / (double)CLOCKS_PER_SEC;
			cout << "\tFPS:" << (1 / duration) << " \tkbps:" << (PACKET_SIZE * total_pack / duration / 1024 * 8) << endl;

			cout << next_cycle - last_cycle;
			last_cycle = next_cycle;
		}

		else {
			camera cam; 
			char headBuff[PACKET_SIZE];

			while (1) {
				if (cam.getBuff()) {
					int numPack = 1 + (cam.m_nCameraFrameBufferSize - 1) / (PACKET_SIZE - 4);
					int ibuf[1];		//Hold sequence number
					ibuf[0] = numPack;
					socket.send(ibuf, sizeof(int), sendIP, 2000);
						
					for (int i = 0; i < numPack - 1; i++) {
						memcpy(&headBuff[0], &cam.m_pCameraFrameBuffer[i * (PACKET_SIZE - 4)], PACKET_SIZE - 4);
						memcpy(&headBuff[PACKET_SIZE - 4 - 1], &i, 4);		//Add sequence number to end of segment
						socket.send(&headBuff, PACKET_SIZE, sendIP, port);
					}
				}
			}
		}
	}
}