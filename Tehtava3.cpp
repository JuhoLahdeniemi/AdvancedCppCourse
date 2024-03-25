// Tehtava3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

class GameTask {
public:
    virtual void perform() = 0;
    virtual ~GameTask() {}
};

class Task1 : public GameTask {
public:
    void perform() override {
        for (volatile int i = 0; i < 1000000; ++i) {} 
    }
};

class Task2 : public GameTask {
public:
    void perform() override {
        for (volatile int i = 0; i < 1000000; ++i) {}
    }
};

class Task3 : public GameTask {
public:
    void perform() override {
        for (volatile int i = 0; i < 1000000; ++i) {}
    }
};

int main() {   
    std::vector<GameTask*> tasks;
    for (int i = 0; i < 10; ++i) {
        tasks.push_back(new Task1());
        tasks.push_back(new Task2());
        tasks.push_back(new Task3());
    }
  
    auto startTime = std::chrono::steady_clock::now();
    for (auto& task : tasks) {
        task->perform();
    }
    auto endTime = std::chrono::steady_clock::now();
    auto sequential_duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    startTime = std::chrono::steady_clock::now();
    int num_threads = std::thread::hardware_concurrency() - 1;
    std::vector<std::thread> threads;
    int tasks_per_thread = tasks.size() / num_threads;
    int tasks_remaining = tasks.size() % num_threads;
    auto task_it = tasks.begin();

    for (int i = 0; i < num_threads; ++i) {
        int num_tasks = tasks_per_thread + (i < tasks_remaining ? 1 : 0);
        threads.emplace_back([&tasks, &task_it, num_tasks]() {
            for (int j = 0; j < num_tasks; ++j) {
                (*task_it)->perform();
                std::advance(task_it, 1); 
            }
            });
    }

    for (auto& thread : threads) {
        thread.join();
    }
    endTime = std::chrono::steady_clock::now();
    auto parallel_duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Sequential duration: " << sequential_duration.count() << " milliseconds\n";
    std::cout << "Parallel duration: " << parallel_duration.count() << " milliseconds\n";
    std::cout << "Speedup: " << static_cast<double>(sequential_duration.count()) / parallel_duration.count() << "x\n";

    for (auto& task : tasks) {
        delete task;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
