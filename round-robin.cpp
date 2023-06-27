/**
 * Simulate the operation of the round-robin algorithm
 * for process scheduling.
 *
 * Create at least 15 processes and output their scheduling
 * situation under the scheduling algorithm mentioned above
 * and output it to the terminal to check the execution of
 * the algorithm.
 * 
 * The output should include the **arrival time** of the
 * processes, **the end time**, and the average execution time.
 *
 */

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Process {
  public:
    int id;
    int arrive_time;
    int burst_time;
    int remaining_time;
    Process(int _id, int _arr, int _bur)
      : id(_id), arrive_time(_arr), burst_time(_bur), remaining_time(burst_time) {};
};

int round_robin(vector<Process*> &processes, int time_quantum) {
  int current_time = 0;
  int time_in_quantum = 0;
  int total_time = 0;
  queue<Process*> ready_processes;

  if (!processes.empty()) ready_processes.push(processes[0]);
  printf("#%d process arrives at %d\n", processes[0]->id, current_time);
  processes.erase(processes.begin());

  while(!processes.empty() || !ready_processes.empty()) {

    auto current_process = ready_processes.front();
    ready_processes.pop();

    while (time_in_quantum < time_quantum) {

      for(auto it = processes.begin(); it != processes.end();) {
        if ((*it)->arrive_time <= current_time) {
          printf("#%d process arrives at %d\n", (*it)->id, current_time);
          ready_processes.push(*it);
          it = processes.erase(it);
        } else {
          it++;
        }
      }

      current_time++;
      time_in_quantum++;

      if (--current_process->remaining_time == 0) {
        time_in_quantum = 0;
        total_time += current_time - current_process->arrive_time;
        printf("#%d process ends at %d\n", current_process->id, current_time);
        delete(current_process);
        current_process = nullptr;
        break;
      }

    }
    if (current_process != nullptr) ready_processes.push(current_process);

    time_in_quantum = 0;
  }

  return total_time;
}

void TestRoundRobin() {
  const int time_quantum = 4;
  const int process_count = 15;
  vector<Process*> processes;

  int arrive_time[] = {0, 3, 5, 7, 10, 14, 15, 18, 20, 24, 25, 27, 30, 32, 33 };
  int burst_time[] = {4, 7, 5, 3, 6, 2, 9, 1, 8, 5, 3, 7, 3, 5, 4 };

  for (int i = 0; i < process_count; i++) {
    processes.push_back(new Process(i, arrive_time[i], burst_time[i]));
  }

  int total_time = round_robin(processes, time_quantum);
  printf("average execution time: %.3f", 1.0 * total_time / process_count);

}

int main() {
  TestRoundRobin();
  return 0;
}
