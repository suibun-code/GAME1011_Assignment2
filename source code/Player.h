#pragma once
#include <list>
#include "Item.h"

class Player
{
private:
	int position;

public:
	Player();
	~Player();

	std::list<Item>inventory;
	std::list<Item>::iterator it;

	void setPos(int position) { this->position = position; }

	const int getPos() { return position; }

	void addItem(Item item, std::fstream &file);
	void displayInv();
};

std::ostream& operator<<(std::ostream& out, const Item &item);


