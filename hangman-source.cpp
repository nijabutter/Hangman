//#include "pch.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
using namespace std;

/*
 _____________________________________________________
|                                                     |
|                     A _ _ L _                       |
|                   ____________                      |
|                   |/          |                     |
|                   |           O                     |
|                   |          /I\                    |
|                   |          / \                    |
|                   |____________                     |
|_____________________________________________________|
|                   Remaining Letters:                |
| _ B C D E F G H I J K _ M N O P Q R S T U V W X Y Z |
|_____________________________________________________|


*/
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   #define CLEAR "cls"
#elif __APPLE__ || __linux__ || __unix__
	#define CLEAR "clear"
#endif
class Word {
public:
	string chosenWord;
	int guesses;
	string remainingLetters;
	string progress;
	string theme;
	vector<string> availableWords;
	string previousWord;
};

string fruit[] = { "ORANGE", "APPLE", "PEAR", "LEMON", "STRAWBERRY", "AVACADO", "BANANA", "MANGO", "APRICOT", "PEACH", "PINEAPPLE", "GRAPE", "CRABNBERRY", "BLACKBERRY", "BLUEBERRY", "CHERRY", "GRAPEFRUIT", "GUAVA", "PLUM"};
string animals[] = {"COW", "PIG", "CHICKEN", "CAT", "LION", "ELEPHANT", "DOG", "BEAVER", "GIRAFFE", "BADGER", "BAT", "RAT", "PIGEON", "PARROT", "WORM", "HORSE", "MONKEY", "TIGER", "DUCK", "GOOSE", "BEAR"};
string flowers[] = {"ROSE", "SUNFLOWER", "DAFODIL", "TULIP", "LILY", "BUTTERCUP", "DAISY", "DANDELION", "BLUEBELL"};
string cars[] = {"HONDA", "FORD", "AUDI", "JAGUAR", "BMW", "MERCEDES", "NISA", "ROVER", "FIAT", "FERRARI", "LAMBORGHINI", "JEEP", "TOYOTA"};

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
bool gameOver;
bool win;
Word word;

string chooseWord()
{

	//int numberOfWords = sizeof(words) / sizeof(words[0]);
	bool validWord = false;
	int numberOfWords = word.availableWords.size();
	string chosenWord;
	do {
		srand(time(NULL));
		int randomNumber = rand() % numberOfWords;
		chosenWord = word.availableWords[randomNumber];
		if (chosenWord[0] != '#' && chosenWord != word.previousWord && chosenWord.length() < 15) validWord = true;
	} while (validWord == false);
	word.previousWord = chosenWord;
	return chosenWord;
}

void WordsNotFound() {
	cout << "Failed to find words.txt." << endl;
	cout << "Format for words.txt:" << endl << endl; 
	cout << "# THEME" << endl << "Word1" << endl << "Word2" << endl << "Word3" << endl; 
	cout << "Would you like to use a pre-made list? Y/N: ";
	if (tolower(cin.get()) == 'y') {
		
		bool valid = false;
		do {
			system(CLEAR);
			cout << "Please choose a list by number:" << endl;
			cout << "1. Fruit" << endl << "2. Car brands" << endl << "3. Animals" << endl << "4. Flowers" << endl;
			switch (cin.get())
			{
				case '1':
					for (int i=0; i<sizeof(fruit)/sizeof(fruit[0]);i++)
						word.availableWords.push_back(fruit[i]);
					word.theme = "FRUIT";
					valid = true;
					break;
				case '2':
					for (int i=0; i< sizeof(cars)/sizeof(cars[0]);i++)
						word.availableWords.push_back(cars[i]);
					word.theme = "CAR BRANDS";
					valid = true;
					break;
				case '3':
					for (int i=0; i<sizeof(animals)/sizeof(animals[0]);i++)
						word.availableWords.push_back(animals[i]);
					word.theme = "ANIMALS";
					valid = true;
					break;
				case '4':
					for (int i=0; i < sizeof(flowers)/sizeof(flowers[0]);i++)
						word.availableWords.push_back(flowers[i]);
					word.theme = "FLOWERS";
					valid = true;
					break;
			}
		} while(!valid);
	} else {
		exit(0);
	}
}

void Setup()
{
	char data;
	string fileName = "words.txt";
	ifstream input_stream(fileName);
	if (!input_stream) {
		WordsNotFound();
	}
	else {
		string line;
		while (getline(input_stream, line))
		{
			if (line[0] == '#') {
				word.theme = line;
			}
			else { 
				if (line.length() > 14) {
					cout << "There is a word in your list that is "<< endl << "longer than 14 characters. Would you like to continue? It will be ignored. Y/N" << endl;
					if (tolower(cin.get()) != 'y') exit(0);
				}
				else {
					word.availableWords.push_back(line); 
				}
			}
		}
		if (word.availableWords.size() < 2) {
			cout << "You must have at least 2 words in your list." << endl;
			exit(0);
		}
	}
}

char askForGuess()
{
	bool validInput = false;
	char guess;
	cout << "     Guess a letter:        |" << endl;
	cout << "____________________________|" << endl;
	do {
		cin >> guess;
		guess = toupper(guess);
		//loops through the alphabet (26 letters)
		for (int i = 0; i < 26; i++)
		{
			if (guess == alphabet[i])
			{
				validInput = true;
				break;
			}
		}
	} while (validInput == false);
	return guess;
}

void checkGuess(char guess)
{
	bool correctGuess = false;
	bool hasBeenGuessed = false;
	int correctLetter;
	for (int i = 0; i < 26; i++)
	{
		if (word.remainingLetters[i] == '_' && alphabet[i] == guess)
		{
			hasBeenGuessed = true;
			correctLetter = i;
			break;
		}
	}

	if (hasBeenGuessed == false)
	{

		for (int i = 0; i < 26; i++)
		{
			if (word.remainingLetters[i] ==  guess)
			{
				word.remainingLetters[i] = '_';
			}
		}
	//	word.remainingLetters[correctLetter] = '_';//removes it from remaining letters
		//Checks if the guess is correct and shows that letter
		for (int i = 0; i < word.progress.length(); i++)
		{
			if (word.chosenWord[i] == guess) {
				word.progress[i] = guess;
				correctGuess = true;
			}
		}
		if (correctGuess == false)
			word.guesses++;
	}



}

void displayWordProgress()
{
	for (int i = 0; i < word.chosenWord.length(); i++)
		cout << word.progress[i] << ' ';
	//cout << endl;
}

void displayMan()
{
	switch (word.guesses)
	{
	case 0:
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		break;
	case 1:
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		cout << "____________                |" << endl;
		break;
	case 2:
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|___________                |" << endl;
		break;
	case 3:
		cout << "____________                |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|____________               |" << endl;
		break;
	case 4:
		cout << "____________                |" << endl;
		cout << "|/                          |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|____________               |" << endl;
		break;
	case 5:
		cout << "____________                |" << endl;
		cout << "|/         |                |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|____________               |" << endl;
		break;
	case 6:
		cout << "____________                |" << endl;
		cout << "|/         |                |" << endl;
		cout << "|          O                |" << endl;
		cout << "|                           |" << endl;
		cout << "|                           |" << endl;
		cout << "|____________               |" << endl;
		break;
	case 7:
		cout << "____________                |" << endl;
		cout << "|/         |                |" << endl;
		cout << "|          O                |" << endl;
		cout << "|          I                |" << endl;
		cout << "|                           |" << endl;
		cout << "|____________               |" << endl;
		break;
	case 8:
		cout << "____________                |" << endl;
		cout << "|/         |                |" << endl;
		cout << "|          O                |" << endl;
		cout << "|         /I                |" << endl;
		cout << "|                           |" << endl;
		cout << "|____________               |" << endl;
		break;
	case 9:
		cout << "____________                |" << endl;
		cout << "|/         |                |" << endl;
		cout << "|          O                |" << endl;
		cout << "|         /I\\               |" << endl;
		cout << "|                           |" << endl;
		cout << "|____________               |" << endl;
		break;
	case 10:
		cout << "____________                |" << endl;
		cout << "|/         |                |" << endl;
		cout << "|          O                |" << endl;
		cout << "|         /I\\               |" << endl;
		cout << "|         /                 |" << endl;
		cout << "|____________               |" << endl;
		break;
	case 11:
		cout << "____________                |" << endl;
		cout << "|/         |                |" << endl;
		cout << "|          O                |" << endl;
		cout << "|         /I\\               |" << endl;
		cout << "|         / \\               |" << endl;
		cout << "|____________               |" << endl;
		break;
	}
}

void checkGameEnd()
{
	bool missingLetter = false;
	for (int i = 0; i < word.progress.length(); i++)
	{
		if (word.progress[i] == '_')
		{
			missingLetter = true;
			break;
		}
	}
	if (missingLetter == false)
	{
		win = true;
		gameOver = true;
	}
	if (word.guesses >= 11)
	{
		gameOver = true;
	}
}

void newGame()
{
	win = false;
	gameOver = false;
	word.chosenWord = chooseWord();
	word.guesses = 0;
	word.remainingLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	word.progress = "";
	for (int i = 0; i < word.chosenWord.length(); i++)
	{
		word.progress += "_";
	}
	do {
		system(CLEAR);

		cout << "____________________________" << endl;
		cout << word.theme;
		for (int i = 0; i < 28 - word.theme.length(); i++)
			cout << " ";
		cout << "|" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		displayWordProgress();
		for (int i = 0; i < (28 - (word.chosenWord.length() * 2)); i++)
			cout << " ";
		cout << "|" << endl;
		cout << "                            |" << endl;
		cout << "                            |" << endl;
		displayMan();
		cout << "                            |" << endl;
		cout << "____________________________|" << endl;
		cout << "     Remaining letters:     |" << endl;
		cout << " " << word.remainingLetters << " |" <<  endl;
		char guess = askForGuess();

		checkGuess(guess);
		checkGameEnd();
	} while (gameOver == false);
	system(CLEAR);

	cout << "____________________________" << endl;
	cout << word.theme;
	for (int i = 0; i < 28 - word.theme.length(); i++)
		cout << " ";
	cout << "|" << endl;
	cout << "                            |" << endl;
	cout << "                            |" << endl;
	displayWordProgress();
	for (int i = 0; i < (28 - (word.chosenWord.length() * 2)); i++)
		cout << " ";
	cout << "|" << endl;
	cout << "                            |" << endl;
	cout << "                            |" << endl;
	displayMan();
	cout << "                            |" << endl;
	cout << "____________________________|" << endl;
	cout << "     Remaining letters:     |" << endl;
	cout << " " << word.remainingLetters << " |" <<  endl;
	cout << "____________________________|" << endl;
	if (win == false)
	{
		cout << endl <<  "G A M E    O V E R" << endl;
		cout << "The word was: " << word.chosenWord << endl;
	}
	else {
		cout << endl << "YOU WIN!" << endl;
	}
	char choice;
	do {
		cout << "Would you like to play again? Y/N: ";
		cin >> choice;
		choice = toupper(choice);
		if (choice == 'Y')
		{
			newGame();
		}
		else
		{
			exit(0);
		}
		system(CLEAR);
	} while (choice != 'Y' && choice != 'N');
}



int main()
{
	Setup();
	newGame();

	

}
