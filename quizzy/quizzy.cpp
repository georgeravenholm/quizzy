// QUIZZY!
// georgeravenholm
#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <algorithm> // shuffle
#include <random>

#include <sstream> // for end message

#define BORDER '*'

struct Answer
{
	Answer() = default;
	Answer(std::string txt, bool correct) { this->txt = txt; this->correct = correct; }
	std::string txt = "";
	char letter = '?'; // this is set in answering time
	bool correct = false;
};

struct Question
{
	Question(std::string txt) { this->txt = txt; }
	void Shuffle()
	{
		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(answers.begin(), answers.end(), g);
	}
	std::string txt;
	std::vector<Answer> answers;
};

void TrimString(std::string & str)
{
	// thanks stack overflow
	size_t endpos = str.find_last_not_of(" \t");
	size_t startpos = str.find_first_not_of(" \t");
	if (std::string::npos != endpos)
	{
		str = str.substr(0, endpos + 1);
		str = str.substr(startpos);
	}
	else
	{
		str.erase(std::remove(std::begin(str), std::end(str), ' '), std::end(str));
	}
}

int main(int argc, char* argv[])
{
	// read file from command arguments
#ifdef _DEBUG
	std::ifstream file("sample.txt");
#else
	if (argc < 2) // no aruments dude
	{
		std::cerr << "usage: quizzy <filename>" << std::endl;
		return 0xff;
	}
	std::string filename = argv[1]; // problem before that it was dereferencing a null pointer because invalid
									// open your eyes!
	std::ifstream file( filename );
#endif

	if (!file) {std::cerr << "file error"; return 1;}

	// parse file
	std::string str;
	std::vector<Question> questions;

	// check first line
	std::getline(file, str);
	TrimString(str);
	if (str != "quiz:") { std::cerr << "invalid quiz file"; return 1; }

	while (std::getline(file, str))
	{
		TrimString(str);
		
		if (str == "question:")
		{
			// new question
			std::getline(file, str); // read next line, question text
			Question current = Question(str);// gm_construct new question object
			
			while (std::getline(file, str) ) // scan for all answers
			{
				TrimString(str);
				if (str == "correct:" || str == "wrong:")
				{
					bool correct = str == "correct:"; // see if this is a correct or wrong answer
					// found an answer
					std::getline(file, str); // read next line, answer text
					current.answers.push_back( Answer(str , correct ) );	// gm_construct new answer object, generate bool if we are
																					// doing correct: or wrong:
				}
				else if (str == "quizend:") goto heck;
				else if (str == "null:") continue;
				else if (str[0] == '#') continue; // allow for comments
				else if (str[0] == '\n') continue; // skip newlines

				else if (str == "questionend:") { std::getline(file, str); break; } // get outa here! (also move to next line)

			}

			// now we have generated the question fully, put it into vector
			current.Shuffle(); // shuffle answers
			questions.push_back(current);

		}
		else if (str == "quizend:") goto heck;
		else if (str == "null:") continue;
		else if (str[0] == '#') continue; // allow for comments
		else if (str[0] == '\n' || str == "") continue; // skip newlines
		else if (!file.good()) { std::cerr << "missing end of file!" << std::endl; return 2; }

		//std::cout << str << std::endl;
	}
	
heck: 

	// Shuffle questions
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(questions.begin(), questions.end(), g);

	////////////////////////////// ANSWERING /////////////////////////////
	//std::cout << "Question file loaded!" << std::endl;
	
	const char alphabet[27] = "abcdefghijklmnopqrstuvwxyz"; // I dont think anybody will put more than 26 answers

	int score = 0; 

	for (unsigned int i = 0; i < questions.size(); i++)
	{
		std::cout << "Question #" << i << std::endl << questions[i].txt << std::endl << std::endl;
		// print answers
		for (unsigned int j = 0; j < questions[i].answers.size(); j++)
		{
			std::cout << alphabet[j] << ") " << questions[i].answers[j].txt << std::endl; // print answer
			questions[i].answers[j].letter = alphabet[j]; // assign the answer a letter so we can find it again
		}

		// get users answer
		for (;;)
		{
			char ans;
			std::cout << "answer? ";
			std::cin >> ans;

			// check if its an answer
			for (auto k : questions[i].answers) // use this for loop because dontneedto track iterator
			{
				if (k.letter == ans) // found an answer!
				{
					std::cout << std::endl; // newline
					if (k.correct)
					{
						std::cout << "Correct, my guy!" << std::endl;
						score+=2;
					}
					else
					{
						std::cout << "Wrong!" << std::endl;
						score--;
					}
					goto nextquestion; // double break out
				}
			}
			
			// here is reached if incorrect answer
			std::cout << "come on dude..." << std::endl;
		}
		nextquestion:

		std::cout << "---" << std::endl;

	}

	// End of quiz, now we have score
	std::stringstream message;
	message << "End of quiz! You scored " << score << " point" << (score == 1 ? "." : "s.");

	int strlen = message.str().length() + 2; // +2 for border
	std::cout << std::string(strlen, BORDER) << "\n" << BORDER << message.str() << BORDER <<"\n" << std::string(strlen, BORDER) << std::endl;

#ifdef _DEBUG
	system("pause");
#endif
    return 0;
}

