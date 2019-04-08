#pragma once
#include <string>
#include <iostream>

struct Item
{
public:
	static int counter;

	std::string name = "NoName";
	int id = 0;

	void printItem()
	{
			std::cout << "Item name: " << name;
			std::cout << "\n";
			std::cout << "Item ID: " << id;
	}
};

