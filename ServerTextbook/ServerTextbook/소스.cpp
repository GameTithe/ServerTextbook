#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>

using namespace std;

const int MaxCount = 150000;
const int ThreadCount = 4;

bool IsPrimeNumber(int number)
{
	if (number == 1)
		return false;

	for (int i = 2; i < number; i++)
	{
		if (number % i == 0)
			return false;
	}

	return true;
}

void PrintNumbers(const vector<int>& primes)
{
	for (int e : primes)
		cout << e << endl;
}

void main()
{
	int num = 1;
	recursive_mutex num_mutex;

	vector<int> primes;
	auto t0 = chrono::system_clock::now();

	vector<shared_ptr<thread>> threads;
	recursive_mutex primes_mutex;

	for (int i = 1; i <= ThreadCount; i++)
	{
		shared_ptr<thread> thread1((new thread([&]() {
			while (true)
			{
				int n;
				{
					lock_guard<recursive_mutex> num_lock(num_mutex);
					n = num;
					num++;
				}
				if (n >= MaxCount)
					break;

				if (IsPrimeNumber(i))
				{
					lock_guard<recursive_mutex> primes_lock(primes_mutex);
					primes.push_back(i);
				}
			}}))

		);

		threads.push_back(thread1);
	}

	for (auto thread : threads)
	{
		thread->join();
	}

	auto t1 = chrono::system_clock::now();

	auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();

	cout << "Took " << duration << "milliseconds" << endl;

	//PrintNumbers(primes);

	return;

}