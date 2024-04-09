#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

using namespace std;
using namespace std::chrono;

unsigned long long calculateFactorial(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

void calculateFactorialRange(int start, int end, vector<unsigned long long>& results) {
    results[start] = calculateFactorial(start);
    for (int i = start + 1; i <= end; ++i) {
        results[i] = results[i - 1] * i;
    }
}

int main() {
    const int number = 65;
    const int numThreads = 6;
    vector<unsigned long long> results(number + 1);
    vector<thread> threads;
    int range = number / numThreads;

    for (int numThreads = 1; numThreads <= 6; ++numThreads) {
        auto start = high_resolution_clock::now();
        atomic<int> threadsRunning(0);

        for (int i = 0; i < numThreads; ++i) {
            int threadStart = i * range + 1;
            int threadEnd = (i == numThreads - 1) ? number : (i + 1) * range;
            threads.emplace_back([threadStart, threadEnd, &results, &threadsRunning] {
                calculateFactorialRange(threadStart, threadEnd, results);
                threadsRunning.fetch_add(1, memory_order_relaxed);
                });
        }

        for (auto& t : threads) {
            t.join();
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        cout << "Number of threads: " << numThreads << endl;
        cout << "Factorial of " << number << " is " << results[number] << endl;
        cout << "Time taken: " << duration.count() << " microseconds" << endl;
        cout << "-----------------------------" << endl;
        cout << endl;

        threads.clear();
    }

    return 0;
}
