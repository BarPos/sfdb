#pragma once

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <fmt/format.h>

namespace sfdb {
	enum DataType {
		INT,
		STR,
		NUL
	};

	class Database {
	public:
		Database(const std::string& path) 
		: path(path) {
			loadFile();
		}
		//~Database();

		std::string getString(const std::string& key);
		int getInt(const std::string& key);

		void edit(const std::string& key, std::string data);
		void edit(const std::string& key, int data);
		void remove(const std::string& key);

		DataType getType(const std::string& key);

		bool isEmpty() { return empty; }

		void save() {
			writeDatabase();
		}
	private:
		void loadFile();
		std::string path;
		bool empty = true;


		std::string replace_all(
			const std::string& str,   // where to work
			const std::string& find,  // substitute 'find'
			const std::string& replace //      by 'replace'
		) {
			using namespace std;
			string result;
			size_t find_len = find.size();
			size_t pos, from = 0;
			while (string::npos != (pos = str.find(find, from))) {
				result.append(str, from, pos - from);
				result.append(replace);
				from = pos + find_len;
			}
			result.append(str, from, string::npos);
			return result;
			/*
				This code might be an improvement to James Kanze's
				because it uses std::string methods instead of
				general algorithms [as 'std::search()'].
			*/
		}
		std::string encode(const std::string& data) {
			std::string buffer;
			buffer.reserve(data.size());
			for (size_t pos = 0; pos != data.size(); ++pos) {
				switch (data[pos]) {
				case ':': buffer.append("&e:");      break;
				case '\n': buffer.append("&endl:");      break;
				case ';': buffer.append("&end:");      break;
				case '&':  buffer.append("&amp:");       break;
				default:   buffer.append(&data[pos], 1); break;
				}
			}
			return buffer;
		}
		std::string decode(const std::string& data) {
			std::string buffer = data;
			buffer = replace_all(buffer, "&amp:", "&");
			buffer = replace_all(buffer, "&end:", ";");
			buffer = replace_all(buffer, "&endl:", "\n");
			buffer = replace_all(buffer, "&e:", ":");
			return buffer;
		}
		std::vector<std::string> split(std::string str, std::string splitBy)
		{
			std::vector<std::string> tokens;
			/* Store the original string in the array, so we can loop the rest
			 * of the algorithm. */
			tokens.push_back(str);

			// Store the split index in a 'size_t' (unsigned integer) type.
			size_t splitAt;
			// Store the size of what we're splicing out.
			size_t splitLen = splitBy.size();
			// Create a string for temporarily storing the fragment we're processing.
			std::string frag;
			// Loop infinitely - break is internal.
			while (true)
			{
				/* Store the last string in the vector, which is the only logical
				 * candidate for processing. */
				frag = tokens.back();
				/* The index where the split is. */
				splitAt = frag.find(splitBy);
				// If we didn't find a new split point...
				if (splitAt == std::string::npos)
				{
					// Break the loop and (implicitly) return.
					break;
				}
				/* Put everything from the left side of the split where the string
				 * being processed used to be. */
				tokens.back() = frag.substr(0, splitAt);
				/* Push everything from the right side of the split to the next empty
				 * index in the vector. */
				tokens.push_back(frag.substr(splitAt + splitLen, frag.size() - (splitAt + splitLen)));
			}
			return tokens;
		}

		void writeDatabase();
		void readDatabase();

		std::map<std::string, std::string> dataString;
		std::map<std::string, int> dataInt;
	};
}