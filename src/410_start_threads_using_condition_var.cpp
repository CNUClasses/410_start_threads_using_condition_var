//============================================================================
// Name        : 410_start_threads_using_condition_var.cpp
// Author      : 
// Version     :
// Copyright   : Steal this code!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;
const int BIG_NUMBER = 3000;

std::vector<thread> thds;
int i=0;

bool bGo = false;
mutex m;
condition_variable cv;

//use the cv variable versions of 
void fun1(string pad){
	//wait for everyone
	{
	unique_lock<mutex> lck(m);
	while(!bGo)
		cv.wait(lck);
	}
	
	for (int j=0;j<BIG_NUMBER;j++){
		cout<<pad<<"thread:"<<this_thread::get_id()<<endl;
		i++;
	}
}

void fun2(string pad){
	{//wait for everyone
	unique_lock<mutex> lck(m);
	while(!bGo)
		cv.wait(lck);
	}

	for (int j=0;j<BIG_NUMBER;j++){
		cout<<pad<<"thread:"<<this_thread::get_id()<<endl;
		i--;
	}
}



int main() {
	string pad;
	for (int j=0;j<10;j++){		
		thds.push_back( thread(fun1,pad));
		thds.push_back( thread(fun2,pad));
		pad = pad +"     ";
	}

	cout<<"Above threads wait until user enters a character (cin.get() below)"<<endl;
	cout<<"So go ahead and press a key "<<endl;
	
	cin.get();
	{
		lock_guard<mutex> lck(m);
		bGo = true;
	}
	cv.notify_all();

	
	for (auto& thd:thds)
		thd.join();
	
	cout << "!!!Hello World!!!--i=" <<i<<endl;
	cout << "Are there problems with the threads couts? or the value of i when all threads are joined?" <<i<<endl;
	return 0;
}
