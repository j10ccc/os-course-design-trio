/*
 * Create a page access sequence (page number range 0-18)
 * using a random function. The sequence length is 54 and
 * assume that the number of main memory frames allocated
 * to the thread is 6, that is, M = 6.
 *
 * Implement the LRU algorithm for page replacement on the
 * above access sequence.
 *
 * Output the page replacement sequence and the page fault rate.
 *
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <queue>

using namespace std;

/**
 * Check if a page is in the memory
 *
 * @param pages Page seqence in memory
 * @param page The page to be checked
 */
bool is_page_existed(list<int> &pages, int page) {
  list<int>::iterator page_it = pages.begin();
  while(page_it != pages.end()) {
    if (*page_it == page) {
      return true;
    }
    page_it++;
  }
  return false;
}

/**
 * LRU algorithm implement, print the page replacement
 * sequence, when read frames
 *
 * @param seq Access seqence
 * @param memory_size The number of main memory frames allocated
 *
 * @return the number of page fault
 */
int lru(vector<int> seq, int memory_size) {
  vector<int>::iterator seq_it = seq.begin();
  list<int> pages;
  list<int> cache;
  int page_fault_count = 0;

  while(seq_it != seq.end()) {

    bool existed = is_page_existed(pages, *seq_it);

    if (!existed) {
      if (pages.size() < memory_size) {
        pages.push_back(*seq_it);
        cache.push_back(*seq_it);
      } else {
        int to_be_replaced = cache.front();
        cache.pop_front();

        list<int>::iterator replace_it = pages.begin();
        while(*replace_it != to_be_replaced) {
          replace_it++;
        }
        auto new_it = pages.erase(replace_it);
        pages.insert(new_it, *seq_it);
        cache.push_back(*seq_it);
      }

      page_fault_count++;
    } else {
      cache.remove(*seq_it);
      cache.push_back(*seq_it);
    }

    seq_it++;

  }
  return page_fault_count;
}

void TestLRU() {
  const int page_bound = 18;
  const int sequence_length = 54;
  const int memory_size = 6;

  vector<int> access_seq;
  printf("[access sequence]");
  for (int i = 0; i < sequence_length; i++) {
    int number = rand() % (page_bound + 1);
    access_seq.push_back(number);
    printf(" %d", number);
  }
  printf("\n");
  int page_fault_count = lru(access_seq, memory_size);
  printf("[page fault rate] %.2f", 1.0 * page_fault_count / sequence_length);
}

int main() {
  TestLRU();
  return 0;
}
