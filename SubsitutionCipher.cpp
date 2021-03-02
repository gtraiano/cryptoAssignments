/*
 * simple substitution cipher
 */
#include <iostream>
#include <map>
#include <string>
#include <algorithm>

int main()
{
	// create key phrase and remove spaces from it
    std::string keyphrase = "FINAL YEAR STUDENT";
    keyphrase.erase(std::remove_if(keyphrase.begin(), keyphrase.end(), ::isspace), keyphrase.end());

    // create a map of occurrences for the letters of the alphabet
    std::map<char, int> occurrences;

    for(char character = 'A'; character <= 'Z'; ++character)
    {
    	occurrences[character] = 0;
    }

    // count occurrences of letters in the key phrase
    for(char character : keyphrase)
    {
        occurrences[character] += 1;
    }

    // copy key phrase to create key
    std::string key(keyphrase);

    // remove all except the first occurrence of letters in the key
    for(std::pair<const char, int>& entry : occurrences)
    {
    	if(entry.second > 1)
    	{
    		std::string::size_type found = key.find_first_of(entry.first); // first occurrence
    		while ((found = key.find_first_of(entry.first, found + 1)) != std::string::npos)
    		{
    		    key.replace(found, 1, "*"); // instead of removing letter, replace it with an asterisk ('*')
    		}
    	}
    }
	// erase asterisks from key
	key.erase(std::remove(key.begin(), key.end(), '*'), key.end());

    // append remaining letters of the alphabet to key
    for(std::pair<const char, int>& entry : occurrences)
    {
    	if(entry.second == 0)
    	{
    		key.append(1, entry.first);
    	}
    }

    // show key phrase and key for comparison
    std::cout << keyphrase << " --> " << key << std::endl;

    // encryption table
    std::map<char, char> enctable;

    for(unsigned int c = 0; c < 26; ++c)
    {
    	enctable[(char) ('a' + c)] = key[c];
    }

    // encode a message and show its cipher
    std::string message("kedleston road");

    std::cout << message << " --> ";
    for(char c : message)
    {
    	std::cout << (!::isspace(c) ? enctable.find(c)->second : ' '); // skip space characters
    }

    // show encryption table
    std::cout << std::endl << "\nencryption table " << std::endl;
    for(std::pair<const char, char>& entry : enctable)
    {
    	std::cout << entry.first << " -> " << entry.second <<std::endl;
    }

}
