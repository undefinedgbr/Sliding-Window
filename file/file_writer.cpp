#include "file_writer.hpp"
#include <vector>
#include <fstream>

namespace file
{
using namespace packet;

FileWriter::FileWriter(const char *filename)
{
	this->filename = filename;
	this->file.open(filename, std::ofstream::binary | std::ofstream::out);
}

FileWriter::~FileWriter()
{
	this->file.close();
}

void FileWriter::appendPacket(Frame &frame)
{
	printf("APPENDING SEQNUM %d SIZE %d\n", frame.getSeqNum(), frame.getDataLength());
	this->file.write(frame.getData(), frame.getDataLength());
	printf("%x\n", this->file.rdstate());
}

void FileWriter::appendPacket(std::vector<Frame> &frames)
{
	for (auto &frame : frames)
	{
		if (frame.getDataLength() == 0)
		{
			continue;
		}
		this->appendPacket(frame);
	}
	this->file.flush();
}

int writeFramesToFile(const char *filename, std::vector<Frame> &frames)
{
	std::ofstream file(filename, std::ofstream::out | std::ofstream::binary);

	for (auto &frame : frames)
	{
		file.write(frame.getData(), frame.getDataLength());
	}

	file.close();
}
} // namespace file