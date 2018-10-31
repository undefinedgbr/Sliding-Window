#ifndef __FILE_READER_H_
#define __FILE_READER_H_

#include <fstream>
#include <vector>
#include "../packet/frame.hpp"

namespace file
{
using namespace packet;

class FileReader
{
  public:
	FileReader(const char *filename);
	FileReader(const char *filename, int maxDataLength);
	~FileReader();
	std::vector<Frame> readAt(int offset, int amount);
	int fileSize;
	std::vector<Frame> toFrames(int maxDataLength);

  private:
	std::ifstream file;
	const char *filename;
	int maxDataLength;
};
} // namespace file
#endif