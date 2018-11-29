#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "solver.h"
#include <string>

void f1(int t) {
	std::cout << "Generated element" << ": " << t << std::endl;
}

void f2(std::string k) {
	std::cout << "Number of generated elements" << ": " << k << std::endl;
}


int main()
{
	auto solver = new Solver(f1, f2);

	solver->run();
	getchar();
}
