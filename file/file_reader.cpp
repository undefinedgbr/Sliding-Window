#include "file_reader.hpp"

#include <fstream>
#include <cstdio>

namespace file {
    using namespace packet;

    FileReader::FileReader(const char *filename)
    {
        this->filename = filename;
        this->file.open(filename, std::ifstream::binary | std::ifstream::ate | std::fstream::in);
        this->fileSize = this->file.tellg();
        this->file.seekg(0, std::ios_base::beg);
        this->file.close();
    }

    std::vector<Frame> FileReader::toFrames(int maxDataLength)
    {
        std::vector<Frame> result;
        int seqnum = 0;

        this->file.open(filename, std::ifstream::binary | std::fstream::in);

        while ((this->file.rdstate() & std::ifstream::eofbit) == 0)
        {
            char data[maxDataLength];
            this->file.read(data, maxDataLength);

            if (this->file.gcount() == 0)
                break;

            Frame p;
            p.setDataLength(this->file.gcount());
            p.setSOH(0x1);
            p.setSeqNum(seqnum);
            p.setData(data);

            result.push_back(p);

            seqnum += 1;
        }

        if (result.size() == 0)
        {
            Frame p;
            p.setDataLength(0);
            p.setSOH(0x4);
            p.setSeqNum(0);

            result.push_back(p);
        }

        result[result.size() - 1].setSOH(0x4);

        this->file.close();

        return result;
    }

    void print_packet(Frame *p)
    {
        printf("PACKET HEADER %02d SEQNUM %-4d LEN %d\n", p->getSOH(), p->getSeqNum(), p->getDataLength());
        for (int i = 0; i < p->getDataLength(); i++)
        {
            printf("%02x ", p->getData()[i]);
        }
        printf("\n");
    }
}