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
const int BIG_NUMBER = 3000000;

std::vector<thread> thds;
int i=0;

bool bGo = false;
mutex m;
condition_variable cv;

//comment/uncomment out following line
//#define USE_CONDITION_VAR
#ifdef USE_CONDITION_VAR
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

#else
void fun1(string pad){
	//spinning waiting to start
	while(!bGo){;}
	
	for (int j=0;j<BIG_NUMBER;j++){
		cout<<pad<<"thread:"<<this_thread::get_id()<<endl;
		i++;
	}
}
void fun2(string pad){
	//spinning waiting to start
	while(!bGo){;}
	
	for (int j=0;j<BIG_NUMBER;j++){
		cout<<pad<<"thread:"<<this_thread::get_id()<<endl;
		i--;
	}
}
#endif

int main() {
	string pad;
	for (int j=0;j<10;j++){		
		thds.push_back( thread(fun1,pad));
		thds.push_back( thread(fun2,pad));
		pad = pad +"     ";
	}

	cout<<"uncomment  USE_CONDITION_VAR statement above to see the difference"<<endl;
	cout<<"between using busy waiting(spinning) and condition variables"<<endl;
	cout<<"show the cpu usage for both cases"<<endl;
	cout<<"press any key when done"<<endl;
	
	cin.get();
	
#ifdef USE_CONDITION_VAR
	{
		lock_guard<mutex> lck(m);
		bGo = true;
	}
	cv.notify_all();
#else
	//use this one first
	bGo=true;	
#endif

	
	for (auto& thd:thds)
		thd.join();
	
	cout << "!!!Hello World!!!--i=" <<i<<endl; // prints !!!Hello World!!!
	return 0;
}
