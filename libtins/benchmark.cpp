#include <tins/tins.h>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>

using namespace Tins;

using clock_type = std::chrono::high_resolution_clock;

size_t count = 0;

bool handle_dns(const PDU &pdu) {
    DNS dns = pdu.rfind_pdu<RawPDU>().to<DNS>();
    for(const auto &query : dns.queries())
        count++;
    for(const auto &answer : dns.answers())
        count++;
    return true;
}

bool handle_packet(PDU &pdu) {
    count++;
    return true;
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
        FileSniffer sniffer(argv[1]);
        clock_type::time_point start;
        if(input_type == "dns") {
            start = clock_type::now();
            sniffer.sniff_loop(handle_dns);
        }
        else {
            start = clock_type::now();
            sniffer.sniff_loop(handle_packet);
        }
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

