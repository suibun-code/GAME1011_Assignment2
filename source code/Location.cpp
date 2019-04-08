#include "Location.h"

Location::Location() : name("Place"), description("Description") {}

Location::~Location() {}

void Location::addLoc(Location &location)
{
	nextLocations.push_back(&location);
}
void Location::addItem(Item item)
{
	items.push_back(item);
}

void Location::removeItem(int index)
{
	items.erase(items.begin() + index);
}
