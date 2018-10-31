#include "file_writer.hpp"
#include <vector>
#include <fstream>

namespace file {
	using namespace packet;

	int writeFramesToFile(const char *filename, std::vector<Frame> frames)
	{
	    std::ofstream file(filename, std::ofstream::out | std::ofstream::binary);

	    for (auto frame : frames)
	    {
	        file.write(frame.getData(), frame.getDataLength());
	    }

	    file.close();
	}
}