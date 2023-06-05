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
    // int end_time;
    int remaining_time;
    Process(int _id, int _arr, int _bur)
      : id(_id), arrive_time(_arr), burst_time(_bur), remaining_time(burst_time) {};
};

void round_robin(vector<Process*> &processes, int time_quantum) {
  int current_time = 0;
  queue<Process*> ready_processes;

  if (!processes.empty()) ready_processes.push(processes[0]);
  processes.erase(processes.begin());

  while(!processes.empty() || !ready_processes.empty()) {

    if (!ready_processes.empty()) {
      auto current_process = ready_processes.front();
      ready_processes.pop();

      if (current_process->remaining_time - time_quantum > 0) {
        current_time += time_quantum;
        current_process->remaining_time -= time_quantum;
        // FIXME: push after news come in
      } else {
        // finished
        current_time += current_process->remaining_time;
        current_process->remaining_time = 0;
        // current_process->end_time = current_time;
        printf("#%c process is end at %d\n", current_process->id + 'A', current_time);
        delete(current_process);
        current_process = nullptr;
      }

      for(auto it = processes.begin(); it != processes.end() && (*it)->remaining_time > 0;) {
        if ((*it)->arrive_time <= current_time) {
          ready_processes.push(*it);
          it = processes.erase(it);
        } else {
          it++;
        }
      }

      if (current_process != nullptr) ready_processes.push(current_process);

    } else {
      current_time++;
    }
  }
}

void TestRoundRobin() {
  const int time_quantum = 4;
  const int process_count = 11;
  vector<Process*> processes;

  // int arrive_time[] = {0, 1, 2, 3, 5, 6, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 21, 22, 23, 24, 25, 27, 28, 30};
  // int burst_time[] = {4, 7, 5, 3, 6, 2, 9, 1, 8, 5, 3, 6, 7, 2, 4, 6, 3, 5, 2, 8, 4, 9, 7, 5};

  //                   A  B  C  D   E   F   G  H    I   J  K
  int arrive_time[] = {0, 3, 5, 7, 10, 14, 15, 18, 20, 24, 25 };
  int burst_time[] = {4, 7, 5, 3, 6, 2, 9, 1, 8, 5, 3 };

  for (int i = 0; i < process_count; i++) {
    processes.push_back(new Process(i, arrive_time[i], burst_time[i]));
  }

  round_robin(processes, time_quantum);

}

int main() {
  TestRoundRobin();
  return 0;
}
