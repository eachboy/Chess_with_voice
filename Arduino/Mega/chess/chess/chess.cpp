#include <chess.h>

int giveTime::forwardTime_x(int current_x){
	return forward_x[current_x-2];
}
	
int giveTime::forwardTime_y(int current_y){
	return forward_y[current_y-2];
}

int giveTime::backTime_x(int next_x){
	return back_x[next_x-2];
}	
	
	
int giveTime::backTime_y(int next_y){
	return back_y[next_y-2];
}
	