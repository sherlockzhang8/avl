#include<iostream>
#include"avl.h"
#include<chrono>
#include<thread>
#include<fstream>
#include<sstream>
#include<string>


int main(void)
{
	do
	{
		std::cout << "Characters: " << std::endl;
		if (std::cin.get() == '\n')
			break;
		std::cin.unget();

		std::string s;
		getline(std::cin, s);

		std::istringstream is(s);
		char c;
		tree <char> tr;

		while (is >> c)
		{
			tr.insert(c);
		}
		tr.out(std::cout, 2, false);

		std::cout << "Press Enter to continue, others to exit." << std::endl;
	} while (std::cin.get() == '\n');
	return 0;
}