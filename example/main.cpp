#include <iostream>
#include <sfdb.h>

int main(int argc, char** argv) {
	try {
		sfdb::Database db("db");

		if (db.isEmpty()) {
			db.edit("Time", 2137);
			db.edit("Hello", "World!");
		}

		int id = db.getInt("Time");
		std::string sd = db.getString("Hello");
		
		fmt::println("Time: '{}', Hello: '{}'", id, sd);

		db.save();
	}
	catch (std::runtime_error e) {
		fmt::println("Exception: {}", e.what());
	}

	return 0;
}