import socket
import struct
import threading
import visualizer

udp_ip = ""
udp_port_pose = 2001
udp_port_chap = 2002
visualizer = visualizer.Vis()

def hex_to_float(hex):
    """Converts a hex string to float"""
    return struct.unpack('!f', str(hex).decode('hex'))[0]

def convert_to_matrix44(m):
    return [
        m[0][0], m[1][0], m[2][0], 0.0,
        m[0][1], m[1][1], m[2][1], 0.0,
        m[0][2], m[1][2], m[2][2], 0.0,
        m[0][3], m[1][3], m[2][3], 1.0
    ]


def visplayer():
    visualizer.on_execute()


class receive_pose(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        t = threading.Thread(target=visplayer)
        # t.start()

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind((udp_ip, udp_port_pose))
        while True:
            data, addr = sock.recvfrom(1024)
            device_num = struct.unpack('i', data[0:4])[0]

            device_type = struct.unpack('i', data[4:8])[0]
            matrix = []

            for i in range(3):
                row = []
                for j in range(4):
                    pointer = 4*4*i + 4*j + 8
                    row.append(struct.unpack('f', data[pointer:pointer+4])[0])
                matrix.append(row)

            matrix44 = convert_to_matrix44(matrix)
            visualizer.set_device(matrix44, device_type, device_num)
            if not t.is_alive():
                t.start()


# Because we may be changing the chaperone we will keep a stream open of where it is
class receive_chaperone(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        print("started")
        sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock2.bind((udp_ip, udp_port_chap))

        while True:
            data, addr = sock2.recvfrom(1024)
            quad_num = struct.unpack('i', data[0:4])[0]
            visualizer.set_num_edges(quad_num * 4)
            for i in range(quad_num):
                for j in range(4):
                    pointer = j*12 + i * 48 + 4
                    x = struct.unpack('f', data[pointer:pointer+4])[0]
                    y = struct.unpack('f', data[pointer+4:pointer+8])[0]
                    z = struct.unpack('f', data[pointer+8:pointer+12])[0]
                    visualizer.update_vert(i*4 + j, x, y, z)

# Create and start our threads
pose_t = receive_pose()
pose_t.start()
chap_t = receive_chaperone()
chap_t.start()


