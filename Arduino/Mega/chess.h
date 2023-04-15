#pragma once
#include <Arduino.h>


class giveTime {
	public:
		int forwardTime_x(int current_x);
		int forwardTime_y(int current_y);
		int backTime_x(int next_x);
		int backTime_y(int next_y);
	private:
	    int forward_x[8] = {430,400,375,360,340,320,330,400};
		int forward_y[10] = {225,225,225,200,195,196,210,235,230.260};
		int back_x[11] = {1,2,3,4,5,6,7,8,9.10,11};
		int back_y[11] = {1,2,3,4,5,6,7,8,9.10,11};
};