#include <tins/tins.h>
#include <cstdlib>
#include <cassert>
#include <iostream>

using namespace Tins;

void dump_to_file(PacketWriter &writer, int packet_count, PDU &packet) {
    while(packet_count--) {
        writer.write(packet);
    }
}

int main(int argc, char *argv[]) {
    int packet_count, packet_size, test_case;
    if(argc != 5) {
        std::cout << "Usage: " << *argv << " <output_file> <test_case> <num_packets> <payload_size>" << std::endl;
        return 1;
    }
    else {
        test_case = atoi(argv[2]);
        packet_count = atoi(argv[3]);
        packet_size = atoi(argv[4]);
    }
    assert(packet_count > 0);
    assert(packet_size >= 0);
    PacketWriter writer(argv[1], PacketWriter::ETH2);
    EthernetII packet;
    
    if(test_case == 1 || test_case == 2) {
        packet = EthernetII() / IP() / TCP() / RawPDU(std::string(packet_size, 'A'));
        if(test_case == 2) {
            TCP::sack_type sack;
            sack.push_back(1234);
            sack.push_back(5678);
            sack.push_back(91011);
            TCP *tcp = packet.find_pdu<TCP>();
            tcp->mss(1234); 
            tcp->winscale(123);
            tcp->sack_permitted();
            tcp->sack(sack);
            tcp->timestamp(1, 2);
            tcp->altchecksum(TCP::CHK_TCP);
        }
    }
    else if(test_case == 3) {
        const uint8_t dns_data[] = {
            98, 243, 129, 128, 0, 1, 0, 1, 0, 0, 0, 0, 6, 103, 111, 111, 
            103, 108, 101, 3, 99, 111, 109, 0, 0, 2, 0, 1, 192, 12, 0, 2, 
            0, 1, 0, 0, 84, 96, 0, 6, 3, 110, 115, 52, 192, 12
        };
        
        packet = EthernetII() / IP() / UDP(52, 53) / DNS(dns_data, sizeof(dns_data));
    }
    dump_to_file(writer, packet_count, packet);
}
