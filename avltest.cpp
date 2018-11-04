#include<iostream>
#include"avl.h"
#include<chrono>
#include<thread>
#include<fstream>
#include<sstream>
#include<string>


int main(void)
{
	using elem_type = char;

	std::ofstream file("out.txt");
	do
	{
		std::cout << "Characters: " << std::endl;
		if (std::cin.get() == '\n')
			break;
		std::cin.unget();

		std::string s;
		getline(std::cin, s);

		std::istringstream is(s);
		elem_type e;
		tree <elem_type> tr;

		while (is >> e)
		{
			tr.insert(e);
		}

		tr.print(file, true);
		file << std::endl;

		tr.preorder();
		std::cout << std::endl;
		tr.inorder();
		std::cout << std::endl;
		tr.postorder();
		std::cout << std::endl;

		std::cout << "Press Enter to continue, others to exit." << std::endl;
	} while (std::cin.get() == '\n');
	return 0;
}