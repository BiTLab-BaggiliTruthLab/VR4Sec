#include "client.h"
#include <Windows.h>


using namespace std;

int main(int argc, char* argv[]) {
	sf::UdpSocket socket;
	char* server_ip = "192.168.1.5";
	std::string port_str = "111";
	unsigned short port = 2000;
	unsigned short port_pose = 2001;
	unsigned short port_chap = 2002;
	char connectiontype = 'c';		//User input to determine if server or client
	std::string arg;
	int num_devices = 5;

	if (argc < 3) {
		std::cout << "CameraClient Usage:\n"
			"-s : act as the server, recieve the video stream\n"
			"-c : client, the VR system (default)\n"
			"-i : server ip (required)\n"
			"-p : server port (default: 2000)\n"
			"-pp : port to send pose information (default: 2001)\n"
			"-ip : ip address to send pose info (default to -i)\n"
			"-pc : port to send chaperone info\n"
			"-ic : address to send chaperon info\n";
		std::cin.get();
		exit(0);
	}
	else
	{
		std::cout << argv[0] << endl;
		for (int i = 1; i < argc; i++) { 
			if (i + 1 <= argc)
				arg = argv[i];
				if (arg == "-i") {
					server_ip = argv[i + 1];
					i += 1;
				}
				else if (arg == "-p") {
					port_str = argv[i + 1];
					port = (unsigned short) strtoul(argv[i+1], NULL, 0);
					i += 1;
				}
				else if (arg == "-s") {
					connectiontype = 's';
				}
				else if (arg == "-c")
				{
					connectiontype = 'c';
				}
			//	else if (arg == "-pp") {
			//		port_pose = (unsigned short) stroul(argv[i + 1], NULL, 0);
			//		i += 1;
			//	}
				else if (arg == "-ip") {
					pose_ip = argv[i + 1];
					i += 1;
				}
			//	else if (arg == "-pc") {
				//	port_chap = (unsigned short)std::stroul(argv[i + 1], NULL, 0);
				//	i += 1;
			//	}
				else if (arg == "-ic") {
					chap_ip = argv[i + 1];
					i += 1;
				}
				else {
					std::cout << "Not enough or invalid arguments, please try again.\n";
					//exit(0);
				}
		}
	}
	char* pose_ip = server_ip;
	char* chap_ip = server_ip;

	std::cout << "Info Server: " << server_ip << "  Port:  " << port << "  Conn:  " << connectiontype << endl;
	
	
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::IpAddress sendIP(server_ip);
	sf::IpAddress poseIP(pose_ip);
	sf::IpAddress chapIP(chap_ip);
	clock_t last_cycle = clock();

	uint8_t *recBuf = new uint8_t[275 * PACKET_SIZE];

	//cout << "Enter s for server c for client:  " << endl;
	//cin >> connectiontype;
	//connectiontype = 'c';

	string text = "Connected to";

	socket.bind(port);

	if (connectiontype == 's')
	{
		std::cout << "Waiting for client";
		while (true)
		{
			socket.receive(buffer, sizeof(buffer), recieved, rIp, port);
			if (recieved > 0)
			{
				std::cout << "Connected to " << rIp << "Port: " << port << endl;

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
		std::cout << "Connecting" << endl;
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
				std::memcpy(&recBuf[sequence_number * (PACKET_SIZE - 4)], buffer, PACKET_SIZE - 4);
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
			std::cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACKET_SIZE * total_pack / duration / 1024 * 8) << endl;

			std::cout << next_cycle - last_cycle;
			last_cycle = next_cycle;
		}

		else {
			ShowWindow(GetConsoleWindow(), SW_HIDE);
			camera cam;
			char headBuff[PACKET_SIZE];

			while (1) {
				if (cam.getBuff()) {
					int numPack = 1 + (cam.m_nCameraFrameBufferSize - 1) / (PACKET_SIZE - 4);
					int ibuf[1];		//Hold sequence number
					ibuf[0] = numPack;
					socket.send(ibuf, sizeof(int), sendIP, port);

					for (int i = 0; i < numPack - 1; i++) {
						std::memcpy(&headBuff[0], &cam.m_pCameraFrameBuffer[i * (PACKET_SIZE - 4)], PACKET_SIZE - 4);
						std::memcpy(&headBuff[PACKET_SIZE - 4 - 1], &i, 4);		//Add sequence number to end of segment
						socket.send(&headBuff, PACKET_SIZE, sendIP, port);
					}
				}
				
				//Send a packet sending a devices pose info.
				for (int i = 0; i < num_devices; i++)
				{
					camera::pose_info pose;
					pose = cam.getPose(i);
					socket.send(&pose, sizeof(pose), poseIP, port_pose);
				}
				
				// Send a packet containing the chaperone information
				camera::chap_info chap = cam.getChap();
				
				// It takes a few frames to get the chaperone to load
				// Wait for a valid Chaperone
				if (chap.num_quads > 0) {
					socket.send(&chap, sizeof(chap), chapIP, port_chap);
				}
			}
		}
	}
}