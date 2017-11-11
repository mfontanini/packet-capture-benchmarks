/**
 * File taken from Pcap++'s repo:
 * https://github.com/seladb/PcapPlusPlus/blob/master/Examples/PcapPlusPlus-benchmark/benchmark.cpp
 */

#include <Packet.h>
#include <DnsLayer.h>
#include <PcapFileDevice.h>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <numeric>

using namespace pcpp;

size_t count = 0;

bool handle_dns(Packet& packet) {
    if (!packet.isPacketOfType(DNS))
    	return true;

    DnsLayer* dnsLayer = packet.getLayerOfType<DnsLayer>();

	DnsQuery* query = dnsLayer->getFirstQuery();
    while (query != NULL)
    {
    	count++;
    	query = dnsLayer->getNextQuery(query);
    }

    DnsResource* answer = dnsLayer->getFirstAnswer();
    while (answer)
    {
    	count++;
    	answer = dnsLayer->getNextAnswer(answer);
    }

    return true;
}

bool handle_packet(Packet& packet) {
    count++;
    return true;
}

int main(int argc, char *argv[]) { 
    if(argc != 4) {
        std::cout << "Usage: " << *argv << " <input-file> <dns|packet> <repetitions>\n";
        return 1;
    }
    std::chrono::high_resolution_clock myClock;
    std::string input_type(argv[2]);
    int total_runs = std::stoi(argv[3]);
    size_t total_packets = 0;
    std::vector<std::chrono::high_resolution_clock::duration> durations;
    for(int i = 0; i < total_runs; ++i) {
        count = 0;
        PcapFileReaderDevice reader(argv[1]);
        reader.open();
        std::chrono::high_resolution_clock::time_point start;
        if(input_type == "dns") {
            start = std::chrono::high_resolution_clock::now();
            RawPacket rawPacket;
            while (reader.getNextPacket(rawPacket))
            {
            	Packet packet(&rawPacket);
            	handle_dns(packet);
            }
        }
        else {
            start = std::chrono::high_resolution_clock::now();
            RawPacket rawPacket;
            while (reader.getNextPacket(rawPacket))
            {
            	Packet packet(&rawPacket);
            	handle_packet(packet);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        durations.push_back(end - start);
        total_packets += count;
        reader.close();
    }
    auto total_time = std::accumulate(
        durations.begin(),
        durations.end(),
        std::chrono::high_resolution_clock::duration(0)
    );
    
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    auto total_time_in_ms = duration_cast<milliseconds>(total_time).count();
    std::cout << (total_packets / total_runs) << " " << (total_time_in_ms / durations.size()) << std::endl;
}
