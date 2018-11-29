#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "solver.h"
#include <string>
#include <sstream>


void f1(int t) {
	std::stringstream ss;
	ss << "Generated element" << ": " << t << std::endl;
	std::cout << ss.str();
}

void f2(std::string k) {
	std::stringstream ss;
	ss << "Number of generated elements" << ": " << k << std::endl;
	std::cout << ss.str();
}


int main()
{
	auto solver = new Solver(f1, f2);

	solver->run();
	getchar();
}
