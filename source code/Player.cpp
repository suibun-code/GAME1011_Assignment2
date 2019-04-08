#include "Player.h"
#include <iostream>
#include <fstream>

Player::Player() : position(0) {}

Player::~Player() {}


void Player::addItem(Item item, std::fstream &file)
{
	std::string line;
	inventory.push_back(item);

	if (!file.is_open())
		file.open("playerdata.txt");

	if (file.is_open())
	{
		while (file >> line)
		{
			if (line.compare("inventory") == 0)
			{
				file.seekp(0, file.end);
				file << " ";
				file << item.id;
				file << " ";
				file << item.name;
				break;
			}
		}
		file.close();
	}
	else
		std::cout << "\nERROR: CANNOT WRITE TO playerdata.txt";

}

void Player::displayInv()
{
	if (inventory.size() != 0)
		for (it = inventory.begin(); it != inventory.end(); it++)
			std::cout << *it << std::endl;
	else
		std::cout << "Empty!\n";
}

std::ostream& operator<<(std::ostream &out, const Item &item)
{
	out << "Name: " << item.name << "\n";
	out << "ID: " << item.id;
	return out;
}
