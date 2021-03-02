/*
 * Code used to solve a Vigenere cipher exercise
*/
#include <iostream>
#include <algorithm>
#include <map>
#include <cmath>
//#include <cctype>


class Utility {
public:
	static const std::string removeSpaces(const std::string& s)
	/* returns a copy of s with spaces removed */
	{
		std::string trimmed(s);
		trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
		return trimmed;
	};

	static std::string changeCase(const std::string& s, bool toUpper)
	/* returns a copy of s with with letter case according to toUpper
	 * toUpper: true for upper case, false for lower case
	 */
	{
		std::string switched(s);
		std::transform(switched.begin(), switched.end(), switched.begin(), [toUpper](unsigned char c){ return toUpper ? std::toupper(c) : std::tolower(c); });
		return switched;
	};
};

class Vigenere {
// Vigenere cipher class
private:
	std::string plainText;					// plain text message
	std::string key;						// encryption key

	std::string message;					// trimmed and case-converted plain text message
	std::string repeatedKey;				// encryption key repeated to message length
	std::string cipher;						// cipher text (encrypted message)
	std::map<char, std::string> tabula;		// decryption keys (tabula)

	bool upperCase;							// whether to convert message and key to upper case and perform all encryption/decryption operations in upper case
	char baseChar;							// base character

// internal operations, no need to expose
	const char shiftCaesar(const char& c, const int& n)
	/*
	 * returns a Caesar shift of n for c
	 */
	{
		return (char) (this->baseChar + ((c - this->baseChar + n) % 26));
	};

	const std::string createTabulaRow(const char& c)
	/*
	 * returns a string of all 26 Caesar shifts for c
	 */
	{
		std::string row;
		for(unsigned int i = 0; i < 26; ++i)
		{
			row.append(1, shiftCaesar(c, i));
		}
		return row;
	};

	const std::map<char, std::string> createTabula(const std::string& key)
	/*
	 * returns a map of the Vigenere decryption keys
	 */
	{
		std::map<char, std::string> tabula;

		for(char c : key)
		{
			tabula[c] = createTabulaRow(this->upperCase ? std::toupper(c) : std::tolower(c));
		}

		return tabula;
	};

	const std::string repeatKey(const std::string& key, const int& mlength)
	/*
	 * returns a string of key repeated up to mlength characters
	 */
	{
		std::string keyrepeat;
		for(unsigned int i = 1; i <= std::floor(mlength / key.length()); ++i)
		{
			keyrepeat.append(key);
		}

		if(mlength % key.length() != 0)
			keyrepeat.append(key, 0, mlength % key.length());

		return keyrepeat;
	};


public:
	Vigenere(const std::string& plainText, const std::string& key, bool upperCase) {
	/*
	 * creates a Vigenere class object
	 * @param plaintText the plain text message
	 * @param key the encryption key
	 * @param upperCase whether to convert message and key to upper case and perform all encryption/decryption operations in upper case
	 * */
		this->upperCase = upperCase;
		this->baseChar = this->upperCase ? 'A' : 'a';
		this->plainText = Utility::changeCase(plainText, this->upperCase);
		this->key = Utility::changeCase(key, this->upperCase);

		// remove spaces from the original message and change case
		this->message = Utility::removeSpaces(Utility::changeCase(this->plainText, this->upperCase));
		// repeat key character sequence for the length of the message
		this->repeatedKey = this->repeatKey(this->key, this->message.length());
	};

	const std::string& getPlainText() {
	/*
	 * returns the plain text message
	 */
		return this->plainText;
	};

	void setPlainText(const std::string& plainText) {
	/*
	 * sets the plain text value and update message accordingly
	 * @param plaintText value to set plain text to
	 */
		this->plainText = Utility::changeCase(plainText, this->upperCase);
		this->message = Utility::removeSpaces(Utility::changeCase(this->plainText, this->upperCase));
	}

	const std::string& getRepeatedKey() {
	/* get repeated key
	 */
		return this->repeatedKey;
	}

	const std::string& getCipher() {
	/*
	 * get cipher
	 */
		return this->cipher;
	}

	const std::string& getKey() {
		return this->key;
	};

	void setKey(const std::string& key) {
		this->key = Utility::changeCase(key, this->upperCase);
		this->repeatedKey = this->repeatKey(this->key, this->key.length());
		this->tabula = this->createTabula(this->key);
	};

	const std::string& getMessage() {
		return this->message;
	}

	const std::map<char, std::string>& getDecryptionKeys() {
		return this->tabula;
	}

	void createDecyptionKeys() {
		this->tabula = this->createTabula(this->key);
	}

	void encodeVigenere()
	/*
	 * creates cipher
	 */
	{
		std::string cipher;
		// encode message characters and add them to cipher
		unsigned int count = 0;
		for(char c : this->message)
		{
			cipher.append(
				1,
				this->tabula.find(this->key.at(count % this->key.length()))->second.at(c - this->baseChar)
			);
			++count;
		}

		this->cipher = cipher;
	};

	const std::string decodeVigenere()
	/*
	 * returns a string of the decoded cipher
	 * */
	{
		std::string plain;

		// look at the corresponding tabula row for the position of cipher's character
		unsigned int count = 0;
		for(char c : cipher)
		{
			std::size_t found = this->tabula.find(key.at(count % key.length()))->second.find(c);
			plain.append(1, this->baseChar + found);
			++count;
		}
		return plain;
	};
};


int main() {
	// plain text message and key
	std::string message("CRYPTO IS SHORT FOR CRYPTOGRAPHY");
	std::string key("ABCD");

	Vigenere vigenere(message, key, true);
	// create decryption keys
	vigenere.createDecyptionKeys();
	// create cipher
	vigenere.encodeVigenere();

	// show plain text message, trimmed message, key repeat and cipher text
	std::cout
		<< "Plain text\t\t" << vigenere.getPlainText() << std::endl
		<< "Without spaces\t\t"<< vigenere.getMessage() << std::endl
		<< "Key\t\t\t" << vigenere.getKey() << std::endl
		<< "Repeated key\t\t"<< vigenere.getRepeatedKey() << std::endl
		<< "Cipher\t\t\t"<< vigenere.getCipher() << std::endl;

	// show decipher
	std::cout << "Decipher\t\t" << vigenere.decodeVigenere() << "\n" << std::endl;

	// show tabula
	std::cout << "Tabula" << std::endl;
	for(auto s : vigenere.getDecryptionKeys())
	{
		std::cout << s.first << "|" << s.second << std::endl;
	}

	return 0;
}
