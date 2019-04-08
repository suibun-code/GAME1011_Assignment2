#pragma once

#include <string>
#include <vector>

#include "Item.h"

class Location
{
private:
	std::string name, description;
	int id;

public:
	Location();
	~Location();

	std::vector<Location*> nextLocations;
	std::vector<Item> items;

	void setName(std::string name) { this->name = name; }
	void setDesc(std::string description) { this->description = description; }
	void setID(int id) { this->id = id; }

	std::string getName() { return name; }
	std::string getDesc() { return description; }
	int getID() { return id; }

	void addLoc(Location &location);
	void addItem(Item item);
	void removeItem(int index);
};

