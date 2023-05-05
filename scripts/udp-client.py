import socket
import struct
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-m","--multicast", default="224.0.0.0")
parser.add_argument("-p","--port", type=int, default=1)
parser.add_argument("-s","--struct", default="HQffffffffffffffff")
args = parser.parse_args()

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(('', args.port))
mreq = struct.pack("4sl", socket.inet_aton(args.multicast), socket.INADDR_ANY)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
while True:
    res = sock.recv(10240)
    try:
        frame = struct.unpack(args.struct, res)
        print(frame)
    except:
        print(res)