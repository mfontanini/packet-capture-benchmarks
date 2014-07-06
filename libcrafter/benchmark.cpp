#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <crafter.h>
#include <pcap.h>

using namespace Crafter;

using clock_type = std::chrono::high_resolution_clock;

size_t count = 0;

void handle_dns(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    Packet sniffed_packet;
    sniffed_packet.PacketFromEthernet(packet, header->caplen);
    DNS dns;
    dns.FromRaw(*GetRawLayer(sniffed_packet));
    for(const auto &q : dns.Queries)
        count++;
    for(const auto &a : dns.Answers)    
        count++;
}

void handle_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    Packet sniffed_packet;
    sniffed_packet.PacketFromEthernet(packet, header->caplen);
    count++;
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        std::cout << "Usage: " << *argv << " <input-file> <dns|packet> <repetitions>\n";
        return 1;
    }
    std::string input_type(argv[2]);
    int total_runs = std::stoi(argv[3]);
    size_t total_packets = 0;
    std::vector<clock_type::duration> durations;
    for(int i = 0; i < total_runs; ++i) {
        count = 0;
        char error[PCAP_ERRBUF_SIZE];
        pcap_t *phandle = pcap_open_offline(argv[1], error);
        if(!phandle) {
            std::cout << error << std::endl;
        }
        decltype(handle_dns)* handler;
        if(input_type == "dns")
            handler = handle_dns;
        else
            handler = handle_packet;
        clock_type::time_point start = clock_type::now();
        pcap_loop(phandle, 0, handler, 0);
        auto end = clock_type::now();
        durations.push_back(end - start);
        total_packets += count;
    }
    
    auto total_time = std::accumulate(
        durations.begin(),
        durations.end(),
        clock_type::duration(0)
    );
    
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    auto total_time_in_ms = duration_cast<milliseconds>(total_time).count();
    std::cout << (total_packets / total_runs) << " " << (total_time_in_ms / durations.size()) << std::endl;
}
