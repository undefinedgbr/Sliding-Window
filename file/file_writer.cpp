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
	this->file.write(frame.getData(), frame.getDataLength());
}

void FileWriter::appendPacket(std::vector<Frame> &frames)
{
	for (auto &frame : frames)
	{
		this->appendPacket(frame);
	}
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