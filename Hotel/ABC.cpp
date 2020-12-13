#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

bool hotel[30];
int hotelSize = 30, numFreeRooms;
mutex getIn, getOut, getIndex, writing;

/// <summary>
/// Method to write output in console.
/// </summary>
/// <param name="output"></param>
inline void write(string output) {
	writing.lock();
	cout << output;
	writing.unlock();
}

/// <summary>
/// Find free room.
/// </summary>
/// <returns></returns>
int getFreeRoomIndex() {
	getIndex.lock();
	for (int i = 0; i < hotelSize; i++) {
		if (!hotel[i])
		{
			getIndex.unlock();
			return i;
		}
	}
	getIndex.unlock();
	return -1;
}

/// <summary>
/// Method for entering room.
/// </summary>
/// <returns></returns>
int getRoom() {
	int num;
	// while can't find free room stay in cycle.
	do {
		num = getFreeRoomIndex();
	} while (num == -1);
	hotel[num] = true;
	write(to_string(num + 1) + string("(    ") + to_string((int)(this_thread::get_id))
		+ string(" guest entered ") + to_string(num + 1) + " room.\n");
	return num;
}

/// <summary>
/// Method for leaving room.
/// </summary>
/// <param name="num"></param>
void leaveRoom(int num) {
	hotel[num] = false;
	write(to_string(num + 1) + string(")    ") + to_string((int)(this_thread::get_id))
		+ string(" guest left ") + to_string(num + 1) + " room.\n");
}


void guestAlgorithm() {

	getIn.lock();
	int num = getRoom();
	getIn.unlock();

	this_thread::sleep_for(chrono::milliseconds(2000 + rand() % 4000));

	getOut.lock();
	leaveRoom(num);
	getOut.unlock();
}

int main() {
	// Set seed to time.
	srand(static_cast<unsigned int>(time(0)));

	numFreeRooms = hotelSize;
	thread* thr = new thread[100];

	for (int i = 0; i < rand() % 180 + 20; i++)
	{
		thr[i] = thread(guestAlgorithm);
		thr[i].detach();
	}

	this_thread::sleep_for(chrono::milliseconds(40000));
}