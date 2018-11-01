#include <vector>
#include <functional>
#include "../packet/frame.hpp"
#include "../packet/ack.hpp"

#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

namespace sw
{
using namespace packet;

class SlidingWindow
{
  private:
	std::vector<ACK> acks;
	int size;
	int start;
	int end;
	int availableFrame;
	std::function<void(Frame &)> windowForwardCallback;

  public:
	std::vector<Frame> frames;
	SlidingWindow() : SlidingWindow(0) {}
	SlidingWindow(int size);
	SlidingWindow(const SlidingWindow &window);
	bool addFrame(Frame frame);
	bool addACK(ACK ack);

	Frame &getFrame(int seqNum);
	ACK &getACK(int seqNum);

	void setSize(int size);
	void setStart(int start);
	void setEnd(int end);
	void setWFCallback(std::function<void(Frame &)> func);
	void incrementAvailableFrame();

	int getSize() const;
	int getStart() const;
	int getEnd() const;
	void decrementAvailableFrame();
};
} // namespace sw
#endif