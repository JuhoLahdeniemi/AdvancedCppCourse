// Tehtava2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


class BankAccount
{
private:
    double balance;
    std::mutex mtx;
    std::condition_variable cv;
    bool withdrawPossible;

public:
    
    BankAccount() : balance(0), withdrawPossible(false){}

    void deposit(double amount)
    {
        std::lock_guard<std::mutex> lock(mtx);
        balance += amount;
        withdrawPossible = true;
        cv.notify_one();
    }

    bool withdraw(double amount)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return withdrawPossible; });
        if (balance >= amount)
        {
            balance -= amount;
            return true;
        }
        else
        {
            return false;
        }
        withdrawPossible = false;
    }

    double getBalance()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return balance;
    }

};

void depositMoneyTimes(BankAccount& account, double amount, int times)
{
    for (int i = 0; i < times; i++)
    {
        account.deposit(amount);
    }
}

void withdrawMoneyTimes(BankAccount& account, double amount, int times)
{
    for (int i = 0; i < times; i++)
    {
        account.withdraw(amount);
    }
}

int main()
{
    BankAccount account;

    std::thread depositThread(depositMoneyTimes, std::ref(account), 20.0, 2000);
    std::thread withdrawThread(withdrawMoneyTimes, std::ref(account), 10.0, 2000);

    depositThread.join();
    withdrawThread.join();

    std::cout << "Balance: " << account.getBalance() << std::endl;
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
