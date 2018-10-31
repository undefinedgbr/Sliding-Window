#ifndef __FILE_WRITER_H__
#define __FILE_WRITER_H__

#include "../packet/frame.hpp"
#include <vector>

namespace file {
	using namespace packet;
	int writeFramesToFile(const char *filename, std::vector<Frame> frames);
}
#endif