// #pragma once

// constexpr size_t EVENT_QUEUE_SIZE = 8;

// class EventQueue {
// public:
//   bool push(Event e) {
//     if (count_ >= EVENT_QUEUE_SIZE) {
//       return false;//队列满
//     }
//     buffer_[tail_] = e;
//     tail_ = (tail_ + 1) % EVENT_QUEUE_SIZE;
//     ++count_;
//     return true;
//   }

//   bool pop(Event& e) {
//     if(count_ == 0){
//       return false; //队列空
//     }
//     e = buffer_[head_];
//     head_ = (head_ + 1) % EVENT_QUEUE_SIZE;
//     --count_;
//     return true;
//   }

// private:
//   Event buffer_[EVENT_QUEUE_SIZE];
//   size_t head_ = 0;
//   size_t tail_ = 0;
//   size_t count_ = 0;
// };