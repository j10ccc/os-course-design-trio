/**
 * A data set is shared among several concurrent processes:
 * Readers – only read the data set; they do not perform any updates.
 * Writers – can both read and write.
 *
 * Problem – allow multiple readers (at most 8) to read at the same time.
 * Only one single writer can access the shared data at the same time.
 */

#include <iostream>
#include <thread>
#include <future>

#include <map>
#include <vector>
#include <deque>
#include <string>
#include <chrono>

#include <mutex>
#include <condition_variable>
#include <semaphore>

using namespace std::literals;

int most_readers = 8;
int readers_count = 20;
int read_count = 0;

std::binary_semaphore rw_mutex(1);
std::counting_semaphore reader_smph(8);

std::mutex reader_count_mutex; int reader_count = 0;

int number = 0;

void thread_write() {
  do {
    rw_mutex.acquire();
    {
      printf("[writer] writer is writing %d\n", number);
      number++;
    }
    rw_mutex.release();

    int second = rand() % 3;
    std::this_thread::sleep_for(std::chrono::seconds(second));
  } while(true);
}

void thread_read(int id) {
  do {
    int second = rand() % 2;
    std::this_thread::sleep_for(std::chrono::seconds(second));

    reader_count_mutex.lock();
    {
      if (readers_count == 0) {
        rw_mutex.acquire();
      }
      readers_count++;
    }
    reader_count_mutex.unlock();

    reader_smph.acquire();
    printf("[reader] %d is reading %d\n", id, number);
    reader_smph.release();

    reader_count_mutex.lock();
    {
      readers_count--;
      if (readers_count == 0) {
        rw_mutex.release();
      }
    }
    reader_count_mutex.unlock();

  } while(true);
}

void TestReaderWriter() {
  std::thread writer(thread_write);

  std::thread readers[readers_count];

  for (int i = 0; i < readers_count; i++) {
    readers[i] = std::thread(thread_read, i);
  }

  writer.join();
  for (int i = 0; i < readers_count; i++) {
    readers[i].join();
  }

}

int main () {
  TestReaderWriter();
  return 0;
}
