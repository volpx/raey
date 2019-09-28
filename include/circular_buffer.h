#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>

//bytes buffer
class CircularBuffer {
  public:
    explicit CircularBuffer(uint8_t *const buf,const uint8_t size);

    void put(const uint8_t data);
    uint8_t get();
    void reset();
    bool empty() const;
    bool full() const;
    uint8_t size() const;
    uint8_t capacity() const;
  private:
    uint8_t *const buf_;
    const uint8_t max_size_;
    uint8_t head_ = 0;
    uint8_t tail_ = 0;
    bool full_ = 0;
};

class CircularBufferV {
  //same but volatile stuff
  public:
    explicit CircularBufferV(uint8_t *const buf,const uint8_t size);

    void put(const uint8_t data);
    uint8_t get() volatile;
    void reset();
    bool empty() const volatile;
    bool full() const volatile;
    uint8_t size() const volatile;
    uint8_t capacity() const;
  private:
    volatile uint8_t *const buf_;
    const uint8_t max_size_;
    volatile uint8_t head_ = 0;
    volatile uint8_t tail_ = 0;
    volatile bool full_ = 0;
};

#endif //CIRCULAR_BUFFER_H
