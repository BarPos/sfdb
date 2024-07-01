#include "database.h"

namespace sfdb {
	std::string Database::getString(const std::string& key)
	{
		if (dataInt.count(key)) {
			throw std::runtime_error("'" + key + "' is an integer!");
			return "";
		}
		if (!dataString.count(key)) {
			throw std::runtime_error("'" + key + "' does not exist!");
			return "";
		}
		return dataString.at(key);
	}
	int Database::getInt(const std::string& key)
	{
		if (dataString.count(key)) {
			throw std::runtime_error("'" + key + "' is a string!");
			return 0;
		}
		if (!dataInt.count(key)) {
			throw std::runtime_error("'" + key + "' does not exist!");
		}
		return dataInt.at(key);
	}
	void Database::edit(const std::string& key, std::string data)
	{
		if (dataInt.count(key)) {
			throw std::runtime_error("'" + key + "' is an integer!");
			return;
		}
		dataString[key] = data;
		empty = false;
	}
	void Database::edit(const std::string& key, int data)
	{
		if (dataString.count(key)) {
			throw std::runtime_error("'" + key + "' is a string!");
			return;
		}
		dataInt[key] = data;
		empty = false;
	}
	void Database::remove(const std::string& key)
	{
		dataInt.erase(key);
		dataString.erase(key);
	}
	DataType Database::getType(const std::string& key)
	{
		if (dataInt.count(key)) {
			return DataType::INT;
		}
		if (dataString.count(key)) {
			return DataType::STR;
		}
		return DataType::NUL;
	}
	void Database::loadFile()
	{
		if (!std::filesystem::exists(path)) {
			fmt::println("'{}' does not exist. Using an empty database.", path);
			return;
		}
		if (std::filesystem::is_directory(path)) {
			throw std::runtime_error(fmt::format("'{}' is a folder!", path));
			return;
		}
		readDatabase();
	}
	void Database::writeDatabase()
	{
		std::ofstream db;
		db.open(path, std::ios::out);
		if (!db.is_open()) {
			throw std::runtime_error(fmt::format("'{}' could not be opened!", path));
			return;
		}

		for (auto i : dataInt) {
			db << fmt::format("{};{};{}\n", encode(i.first), "i", i.second);
		}
		for (auto s : dataString) {
			db << fmt::format("{};{};{}\n", encode(s.first), "s", encode(s.second));
		}
		db.close();
	}
	void Database::readDatabase()
	{
		std::ifstream db;
		db.open(path, std::ios::in);
		if (!db.is_open()) {
			throw std::runtime_error(fmt::format("'{}' could not be opened!", path));
			return;
		}

		std::string line;
		std::vector<std::string> lines;
		while (std::getline(db, line)) {
			lines.push_back(line);
		}

		for (auto e : lines) {
			auto ee = split(e, ";");
			if (ee[1] == "i") {
				dataInt[decode(ee[0])] = std::atoi(ee[2].c_str());
			}
			else if (ee[1] == "s") {
				dataString[decode(ee[0])] = decode(ee[2]);
			}
		}

		empty = false;

		db.close();
	}
}