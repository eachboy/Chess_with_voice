#pragma once
#include <Arduino.h>


class giveTime {
	public:
		int forwardTime_x(int current_x);
		int forwardTime_y(int current_y);
		int backTime_x(int next_x);
		int backTime_y(int next_y);
	private:
	    int forward_x[8] = {350,350,320,320,310,310,320,390};
		int forward_y[10] = {215,210,215,190,190,190,200,210,210,215};
		int back_x[11] = {1,2,3,4,5,6,7,8,9,10,11};
		int back_y[11] = {1,2,3,4,5,6,7,8,9,10,11};
};
