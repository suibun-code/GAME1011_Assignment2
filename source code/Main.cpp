#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include <map>

#include "Player.h"
#include "Location.h"

void play(Player &player, std::vector<Location> &locations, std::vector<Item> &items, std::fstream &file);
void load_data(std::fstream &file, Player &player);
void load_level(std::fstream &file, Player &player, std::vector<Location> &locations, std::vector<Item> &items);
void printItems(std::vector<Location> locations, std::vector<Item> items, Player player);
void printLocs(std::vector<Location> locations, Player player);

int main()
{
	//VARIABLES
	Player player;
	std::vector<Item> items;
	std::vector<Location> locations(4);
	std::string ok;

	std::fstream gameFile("gamedata.txt", std::ios::in || std::ios::out);
	std::fstream dataFile("playerdata.txt", std::ios::in || std::ios::out);

	//LOAD FILES
	load_data(dataFile, player);
	load_level(gameFile, player, locations, items);
	std::cout << "\n";
	Sleep(1500);
	system("CLS");

	std::cout << "<<<<READ ME>>>>\n";

	std::cout << "\nImportant notes:\n";
	std::cout << "1. Input is case-sensitive.\n";
	std::cout << "2. Enter 'Use Inv' (mind the capital i) any-time during the game to show your inventory.\n";
	std::cout << "3. Possible commands are: Go, Take, Use. Go for locations, Take for items, Use for inventory.\n";
	std::cout << "4. Actions you do, such as the 3 above, will be shown at the top of the console after you do them.\n";
	std::cout << "5. All input is bullet-proofed.\n";
	std::cout << "6. Output may look jarring at first. I attempted to make it as readable as possible.\n";

	std::cout << "\nMisc notes:\n";
	std::cout << "1. It is simple to create a new world, or cheat by adding items directly into playerdata.txt.\n";
	std::cout << "2. The inventory is maintained via an STL list.\n";
	std::cout << "3. Items obtained are written into playerdata.txt. This is experimental.\n";
	std::cout << "4. I recommend looking through gamedata.txt and playerdata.txt to see how things are organized.\n";

	std::cout << "\nEnter 'OK' to begin.\n";

	while (ok.compare("OK") != 0)
		std::cin >> ok;

	std::cin.clear();
	std::cin.ignore(1000, '\n');

	system("CLS");

	//GAME LOOP
	play(player, locations, items, dataFile);

	std::cout << "\n";
	system("pause");
	return 0;
}

void play(Player &player, std::vector<Location> &locations, std::vector<Item> &items, std::fstream &file)
{
	std::string input;
	std::string buff;
	std::vector<std::string> tokens;
	enum command { Go = 1, Take = 2, Use = 3 };
	std::map<std::string, command> commands;
	bool badInput = true;

	commands["Go"] = Go;
	commands["Take"] = Take;
	commands["Use"] = Use;

	std::cout << "Position " << player.getPos() << " - Location \"" << locations.at(player.getPos()).getName() << "\" ID " << locations.at(player.getPos()).getID() << "\n\n"; //show players current position
	std::cout << locations.at(player.getPos()).getDesc() << "\n\n";

	printItems(locations, items, player); //PRINT ALL ITEMS
	std::cout << "\n\n";
	printLocs(locations, player); //PRINT ALL EXITS

	do
	{
		std::cout << "\n\nWhat do you want to do?\n";
		std::getline(std::cin, input);

		std::stringstream ss(input); //Split string
		while (ss >> buff)
			tokens.push_back(buff);

		if (tokens.size() > 2)
		{
			std::cout << "Invalid input: Too many words.";
			tokens.clear();
		}
		else if (tokens.size() < 2)
		{
			std::cout << "Invalid input: Too few words.";
			tokens.clear();
		}
	} while (tokens.size() > 2 || tokens.size() < 2 || tokens.empty() == true);


	switch (commands[tokens.at(0)])
	{
	case Go:

		badInput = true;

		//CALCULATE WHICH EXIT IS CHOSEN
		for (int i = 0; i < locations.at(player.getPos()).nextLocations.size(); i++)
		{
			if (tokens.at(1).compare(locations.at(player.getPos()).nextLocations.at(i)->getName()) == 0)
			{
				badInput = false;
				player.setPos(locations.at(player.getPos()).nextLocations.at(i)->getID());
				system("CLS");
			}
		}

		if (badInput == true)
		{
			system("CLS");
			std::cout << "Room not found.\n\n";
		}


		break;

	case Take:

		badInput = true;

		for (int i = 0; i < locations.at(player.getPos()).items.size(); i++)
		{
			if (tokens.at(1).compare(locations.at(player.getPos()).items.at(i).name) == 0)
			{
				system("CLS");
				badInput = false;
				player.addItem(locations.at(player.getPos()).items.at(i), file);
				std::cout << "You took " << locations.at(player.getPos()).items.at(i).name << ".\n\n";
				locations.at(player.getPos()).removeItem(i);
			}
		}

		if (badInput == true)
		{
			system("CLS");
			std::cout << "Can not find that item to take.\n\n";
		}

		break;

	case Use:

		badInput = true;

		if (tokens.at(1).compare("Inv") == 0)
		{
			badInput = false;
			//DISPLAY PLAYER INVENTORY
			system("CLS");
			std::cout << "Player Inventory\n---------------------\n";
			player.displayInv();
			std::cout << "---------------------\n\n";
		}

		break;

	default:
		system("CLS");
		std::cout << "Please only enter the following: 'Go (location)', 'Take (item)', or 'Use Inv'.\n\n";
	}

	badInput = true;

	play(player, locations, items, file); //RECURSION
}

void load_data(std::fstream &file, Player &player)
{
	//OPEN FILE
	if (!file)
	{
		std::cout << "Error opening playerdata file. Please make sure the file is in the root directory.\n";
		throw 0;
		return;
	}
	else
		std::cout << "player data file opened successfully.\n";
	//END OPEN FILE

	std::string line;
	char ch;

	while (!file.eof())
	{
		file >> line;
		if (line.compare("position") == 0)
		{
			file.seekg(1L, file.cur);
			file.get(ch);
			player.setPos(ch - '0');
		}
		if (line.compare("inventory") == 0)
		{
			Item item;
			while (file.peek() != '\n' && !file.eof())
			{
				file >> line;
				item.id = stoi(line);
				file >> line;
				item.name = line;
				player.inventory.push_back(item);
			}
		}
	}
	file.close();
}

void load_level(std::fstream &file, Player &player, std::vector<Location> &locations, std::vector<Item> &items)
{
	//OPEN FILE
	if (!file)
	{
		std::cout << "Error opening gamedata file. Please make sure the file is in the root directory.\n";
		throw 0;
		return;
	}
	else
		std::cout << "game data file opened successfully.\n";
	//END OPEN FILE

	std::string line;
	char ch;
	char ch2;

	while (!file.eof())
	{
		file >> line;
		if (line.compare("item") == 0)
		{
			Item::counter++;
			items.resize(Item::counter);
			file.seekg(1L, file.cur);
			file.get(ch);
			items.at(ch - '0').id = ch - '0';
			file >> line;
			items.at(ch - '0').name = line;
		}
		else if (line.compare("location") == 0)
		{
			file.seekg(1L, file.cur);

			file.get(ch2);
			locations.at(ch2 - '0').setID(ch2 - '0');

			file >> line;
			locations.at(ch2 - '0').setName(line);

			if (file.peek() == 34);
			{
				line.clear();
				file.seekg(2L, file.cur);
				std::getline(file, line);
				locations.at(ch2 - '0').setDesc(line);
			}
		}
		else if (line.compare("exit") == 0)
		{
			line.clear();
			file.seekg(1L, file.cur);
			file.get(ch);

			while (file.peek() != '\n' && !file.eof())
			{
				file.seekg(1L, file.cur);
				file.get(ch2);

				if (ch != ch2)
					locations.at(ch - '0').addLoc(locations.at(ch2 - '0'));
			}
		}
		else if (line.compare("items") == 0)
		{
			line.clear();
			file.seekg(1L, file.cur);
			file.get(ch);

			while (file.peek() != '\n' && !file.eof())
			{
				file.seekg(1L, file.cur);
				file.get(ch2);

				locations.at(ch - '0').addItem(items.at(ch2 - '0'));
			}
		}
	}
	file.close();
}

void printItems(std::vector<Location> locations, std::vector<Item> items, Player player)
{
	std::cout << "Items: ";
	for (int i = 0; i < locations.at(player.getPos()).items.size(); i++)
	{
		std::cout << locations.at(player.getPos()).items.at(i).name;

		if (i == locations.at(player.getPos()).items.size() - 1)
			std::cout << ".";
		else
			std::cout << ", ";
	}
}

void printLocs(std::vector<Location> locations, Player player)
{
	std::cout << "Exits: ";
	for (int i = 0; i < locations.at(player.getPos()).nextLocations.size(); i++)
	{
		std::cout << locations.at(player.getPos()).nextLocations.at(i)->getName();

		if (i == locations.at(player.getPos()).nextLocations.size() - 1)
			std::cout << ".";
		else
			std::cout << ", ";
	}
}