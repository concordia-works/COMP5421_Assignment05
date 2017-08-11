#include <string>
#include <vector>
#include <map>
#include <set>
#include <array>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iomanip>

#include <functional> // for std::not1
#include <cctype> // for std::ptr_fun

using std::string;
using std::vector;
using std::map;
using std::set;
using std::cout;
using std::endl;

class MyCompare {
public:
	MyCompare() {};
	virtual ~MyCompare() {};

	bool operator()(const string& str1, const string& str2) const {
		string temp1 = str1;
		string temp2 = str2;
		std::transform(temp1.begin(), temp1.end(), temp1.begin(), tolower);
		std::transform(temp2.begin(), temp2.end(), temp2.begin(), tolower);
		return (temp1 < temp2);
	}
};

bool is_palindrome(const string& phrase) {
	string temp;

	// Copy all alphabet chars to temp
	std::remove_copy_if(phrase.begin(), phrase.end(), std::insert_iterator<string>(temp, temp.begin()), std::not1(std::ptr_fun((int(*)(int))std::isalpha)));

	// Transform all chars to UPPERCASE
	std::transform(temp.begin(), temp.end(), temp.begin(), toupper);

	return std::equal(temp.begin(), temp.end(), temp.rbegin());
}

void test_is_palindrome() {
	string str_i = string("Was it a car or a cat I saw?");
	string str_u = string("Was it a car or a cat U saw?");
	cout << "the phrase \"" + str_i + "\" is " + (is_palindrome(str_i) ? "" : "not ") + "a palindrome\n";
	cout << "the phrase \"" + str_u + "\" is " + (is_palindrome(str_u) ? "" : "not ") + "a palindrome\n";
}

template <class Iterator>
std::pair<Iterator, bool> second_max(Iterator start, Iterator finish) {
	// If the container is empty
	if (start == finish)
		return std::make_pair(finish, false);

	Iterator maxIt = start, secondMaxIt = start;
	for (auto it = start; it != finish; it++) {
		if (*it > *maxIt) {
			secondMaxIt = maxIt;
			maxIt = it;
		}
		else if ((*it < *maxIt) && (*it > *secondMaxIt))
			secondMaxIt = it;
	}

	// If the container contains all equal elements
	if (maxIt == start && secondMaxIt == start)
		return std::make_pair(start, false);

	return std::make_pair(secondMaxIt, true);
}

void test_second_max() {
	vector<int> int_vec = { 1,1,5,5,7,7 };
	// IF NOT COMPILE, TRY BELOW
	//std::array<int, 6> v0_init = { 1, 1, 5, 5, 7, 7 };
	//vector<int> int_vec(v0_init.begin(), v0_init.end());

	// return type from auto below: std::pair<std::vector<int>::iterator, bool>
	auto retval = second_max(int_vec.begin(), int_vec.end());
	if (retval.second) {
		cout << "The second largest element in int_vec is " << *retval.first << endl;
	}
	else {
		if (retval.first == int_vec.end())
			cout << "List empty, no elements\n";
		else
			cout << "Container's elements are all equal to " << *retval.first << endl;
	}

}

string& remove_leading_trailing_non_alpha(string& word) {
	if (word.empty())
		return word;

	auto it = word.begin();
	if (!std::isalpha(*it))
		word.erase(it);
	it = word.end() - 1;
	if (!std::isalpha(*it))
		word.erase(it);

	return word;
}

void print_word_frequency_1(const string& filename) {
	// open file
	std::ifstream ifs = std::ifstream(filename);
	if (!ifs)
		throw std::invalid_argument(std::string("could not open file ") + filename);

	// map strings (words) to ints (frequency count)
	map<string, int> word_frequency_map;
	// std::map<std::string, int, MyCompare> word_frequency_map;

	// process lines in ifs
	string line;
	while (getline(ifs, line)) {
		// turn line into a string stream
		std::istringstream iss(line);

		// process strings in iss
		string word;
		while (iss >> word) { // read a word
			//++word_frequency_map[word]; // process a word
			++word_frequency_map[remove_leading_trailing_non_alpha(word)]; // process a word
		}
	}

	for (const auto& word : word_frequency_map) { // print the frequency list
		cout << std::setw(10) << word.first << " " << word.second << endl;
	}
}

void print_word_frequency_2(const string& filename) {
	// open file
	std::ifstream ifs = std::ifstream(filename);
	if (!ifs)
		throw std::invalid_argument(std::string("could not open file ") + filename);

	// map strings (words) to ints (frequency count)
	std::map<std::string, int, MyCompare> word_frequency_map;

	// process lines in ifs
	string line;
	while (getline(ifs, line)) {
		// turn line into a string stream
		std::istringstream iss(line);

		// process strings in iss
		string word;
		while (iss >> word) { // read a word
			++word_frequency_map[remove_leading_trailing_non_alpha(word)]; // process a word
		}
	}

	for (const auto& word : word_frequency_map) { // print the frequency list
		cout << std::setw(10) << word.first << " " << word.second << endl;
	}
}

void print_word_index(const string& filename) {
	// open file
	std::ifstream ifs = std::ifstream(filename);
	if (!ifs)
		throw std::invalid_argument(std::string("could not open file ") + filename);

	// map strings (words) to set of lines appear
	map<string, set<int>, MyCompare> word_lines_map;

	// process lines in ifs
	int lineNo = 1;
	string line;
	while (getline(ifs, line)) {
		// turn line into a string stream
		std::istringstream iss(line);

		// process strings in iss
		string word;
		while (iss >> word) { // read a word
			word_lines_map[remove_leading_trailing_non_alpha(word)].insert(lineNo);
		}
		lineNo++;
	}

	for (const auto& word : word_lines_map) { // print the map
		cout << std::setw(10) << word.first << " ";
		for (int lineNo : word.second)
			cout << lineNo << " ";
		cout << endl;
	}
}

int main() {
	test_is_palindrome();
	cout << "----------------------------------------" << endl;

	test_second_max();
	cout << "----------------------------------------" << endl;
	
	std::string filename = "input.txt";

	print_word_frequency_1(filename);
	cout << "----------------------------------------" << endl;

	print_word_frequency_2(filename);
	cout << "----------------------------------------" << endl;

	print_word_index(filename);
	
	getchar();
	return 0;
}
