#include <stdio.h>
#include <vector>
#include "file/file_reader.hpp"
#include "file/file_writer.hpp"
#include "packet/frame.hpp"

using namespace packet;
using namespace file;

void print_packet(Frame *p)
{
    printf("PACKET HEADER %02d SEQNUM %-4d LEN %d\n", p->getSOH(), p->getSeqNum(), p->getDataLength());
    for (int i = 0; i < p->getDataLength(); i++)
    {
        printf("%02x ", p->getData()[i]);
    }
    printf("\n");
}

int main()
{
    std::vector<Frame> packets;
    FileReader reader("main_client.cpp", 1024);

    packets = reader.readAt(0, 10);
    for (auto &p : packets)
    {
        print_packet(&p);
    }

    FileWriter f("out");
    f.appendPacket(packets);
}