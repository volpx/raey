#include "circular_buffer.h"

CircularBuffer::CircularBuffer(uint8_t *const buf,const uint8_t size):
  buf_(buf),
  max_size_(size)
  {
  // empty
  reset();
}
void CircularBuffer::reset() {
    head_ = tail_;
    full_ = false;
}
bool CircularBuffer::empty() const {
    //if head and tail are equal, we are empty
    return (!full_ && (head_ == tail_));
}
bool CircularBuffer::full() const {
    //If tail is ahead the head by 1, we are full
    return full_;
}
uint8_t CircularBuffer::capacity() const {
    return max_size_;
}
uint8_t CircularBuffer::size() const {
    uint8_t size = max_size_;

    if(!full_) {
        if(head_ >= tail_) {
            size = head_ - tail_;
        }
        else {
            size = max_size_ + head_ - tail_;
        }
    }

    return size;
}
void CircularBuffer::put(const uint8_t item) {
    buf_[head_] = item;

    if(full_) {
        tail_ = (tail_ + 1) % max_size_;
    }

    head_ = (head_ + 1) % max_size_;

    full_ = head_ == tail_;
}
uint8_t CircularBuffer::get() {
    if(empty()) {
        return 0;
    }

    //Read data and advance the tail (we now have a free space)
    uint8_t val = buf_[tail_];
    full_ = false;
    tail_ = (tail_ + 1) % max_size_;

    return val;
}


// volatile version
CircularBufferV::CircularBufferV(uint8_t *const buf,const uint8_t size):
  buf_(buf),
  max_size_(size)
  {
  // empty
  reset();
}
void CircularBufferV::reset() =CircularBuffer::reset();
bool CircularBuffer::empty() const {
    //if head and tail are equal, we are empty
    return (!full_ && (head_ == tail_));
}
bool CircularBuffer::full() const {
    //If tail is ahead the head by 1, we are full
    return full_;
}
uint8_t CircularBuffer::capacity() const {
    return max_size_;
}
uint8_t CircularBuffer::size() const {
    uint8_t size = max_size_;

    if(!full_) {
        if(head_ >= tail_) {
            size = head_ - tail_;
        }
        else {
            size = max_size_ + head_ - tail_;
        }
    }

    return size;
}
void CircularBuffer::put(const uint8_t item) {
    buf_[head_] = item;

    if(full_) {
        tail_ = (tail_ + 1) % max_size_;
    }

    head_ = (head_ + 1) % max_size_;

    full_ = head_ == tail_;
}
uint8_t CircularBuffer::get() {
    if(empty()) {
        return 0;
    }

    //Read data and advance the tail (we now have a free space)
    uint8_t val = buf_[tail_];
    full_ = false;
    tail_ = (tail_ + 1) % max_size_;

    return val;
}
