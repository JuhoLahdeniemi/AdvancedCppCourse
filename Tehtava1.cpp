// Tehtava1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

const int numOfThreads = 4;
std::mutex mtx;

void partialSum(const std::vector<int>& array, int start, int end, long long& result)
{
    long long pSum = 0;
    for (int i = start; i < end; i++)
    {
        pSum += array[i];
    }

    std::lock_guard<std::mutex> lock(mtx);
    result += pSum;
}

int main()
{
    long long test = 0;
    const int arraySize = 5000;
    std::vector<int> numbers(arraySize);
    for (int i = 0; i < arraySize; i++)
    {
        numbers[i] = i + 1;
        test += numbers[i];
    }

    long long sum = 0;

    std::thread threads[numOfThreads];
    int partSize = arraySize / numOfThreads;
    for (int i = 0; i < numOfThreads; i++)
    {
        int start = i * partSize;
        int end = (i == numOfThreads - 1) ? arraySize : (i + 1) * partSize;
        threads[i] = std::thread(partialSum, std::ref(numbers), start, end, std::ref(sum));

    }

    for (int i = 0; i < numOfThreads; i++)
    {
        threads[i].join();

    }

    std::cout << "Test: " << test << std::endl;
    std::cout << "Sum of array: " << sum << std::endl;
    

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
