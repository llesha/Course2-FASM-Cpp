#include <iostream>
#include <string>
#include <omp.h>

using namespace std;
int A[10000], B[10000], arrSize, res[10000], primes[150], primeSize, resSize;

/// <summary>
/// Compute prime numbers from 2 to sqrt(20000).
/// </summary>
void calculatePrimes()
{
	primes[0] = 2;
	primeSize = 1;
	bool isPrime = true;
	// root from 20000 is smaller than 150.
	for (int i = 3; i < 150; i++)
	{
		for (int j = 0; j < primeSize; j++)
		{
			if (i % primes[j] == 0)
			{
				isPrime = false;
				break;
			}
		}
		if (isPrime)
		{
			primes[primeSize] = i;
			primeSize++;
		}
		isPrime = true;
	}
}

/// <summary>
/// Randomize array A and B.
/// </summary>
void createArrays()
{
	arrSize = rand() % 9991 + 10;
	for (int i = 0; i < arrSize; i++)
	{
		A[i] = rand() % 10000 + 1;
		B[i] = rand() % 10000 + 1;
	}
}

/// <summary>
/// Method to check whether sum or difference of A[i] and B[i] is prime.
/// </summary>
/// <param name="threadInd">index of current thread.</param>
/// <param name="numOfThreads">total number of threads.</param>
void isPrime(int threadInd, int numOfThreads)
{

	for (int i = threadInd; i < arrSize; i += numOfThreads)
	{
		bool sumComposite = false;
		bool subComposite = false;
		int j = 0;
		for (j = 0; primes[j] * primes[j] <= A[i] - B[i]; j++)
		{
			if ((A[i] - B[i]) % primes[j] == 0)
			{
				subComposite = true;
				break;
			}
		}
		for (j = 0; primes[j] * primes[j] <= A[i] + B[i]; j++)
		{
			if ((A[i] + B[i]) % primes[j] == 0)
			{
				sumComposite = true;
				break;
			}

		}
		if (!subComposite && (A[i] - B[i] > 1)) {
			res[resSize] = -i;
			resSize++;
		}
		else if (!sumComposite) {
			res[resSize] = i;
			resSize++;
		}
	}
}

/// <summary>
/// Method of creating threads.
/// </summary>
/// <param name="numOfThreads">number of threads to create.</param>
void computeMapOfI(int numOfThreads)
{
	// Use less threads if A.size is smaller than numOfThreads.
	if (numOfThreads > arrSize)
		numOfThreads = arrSize;

	omp_set_num_threads(numOfThreads);

#pragma omp parallel for
	for (int i = 0; i < numOfThreads; i++)
	{
		isPrime(i, numOfThreads);
	}
}

int main()
{
	// Set seed to time.
	srand(static_cast<unsigned int>(time(0)));

	int numOfThreads = 0;
	resSize = 0;
	createArrays();
	calculatePrimes();

	// Read input.
	cout << "Type in number of threads: ";
	string num;
	cin >> num;
	try {
		numOfThreads = stoi(num);
	}
	catch (exception ex)
	{
		cout << "Problems with input. Probably NaN in input. ";
	}
	if (numOfThreads < 1 || numOfThreads > 10000)
	{
		cout << "Impossible to create less than 1 thread or more than 10000. Will be working with 10 threads.\n";
		numOfThreads = 10;
	}
	computeMapOfI(numOfThreads);

	cout << "Map of indexes:\n";
	// Write all indexes in res.
	int threadNum = 0;
	cout << "Thread 0:\n";
	for (int i = 0; i < resSize; i++)
	{
		if ((res[i] > 0 ? res[i] : -res[i]) % numOfThreads != threadNum)
		{
			threadNum = (res[i] > 0 ? res[i] : -res[i]) % numOfThreads;
			cout << "Thread " << threadNum << ":\n";
		}
		if (res[i] < 0)
		{
			cout << "A[" << -res[i] << "] - B[" <<
				-res[i] << "] = " << A[-res[i]] << " - " <<
				B[-res[i]] << " = " << A[-res[i]] - B[-res[i]] << "\n";
		}
		else cout << "A[" << res[i] << "] + B[" <<
			res[i] << "] = " << A[res[i]] << " + " <<
			B[res[i]] << " = " << A[res[i]] + B[res[i]] << "\n";
	}
}