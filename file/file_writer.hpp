#ifndef __FILE_WRITER_H__
#define __FILE_WRITER_H__

#include <fstream>
#include "../packet/frame.hpp"
#include <vector>

namespace file
{
using namespace packet;

class FileWriter
{
  public:
	FileWriter(const char *filename);
	~FileWriter();
	void appendPacket(std::vector<Frame> frames);
	void appendPacket(Frame frame);

  private:
	std::ofstream file;
	const char *filename;
};

int writeFramesToFile(const char *filename, std::vector<Frame> frames);
} // namespace file
#endif