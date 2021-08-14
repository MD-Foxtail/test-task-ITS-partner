#ifndef PARS
#define PARS
enum comparisonType
{
  NUM,
  WORD
};
int findWord(std::string &str, std::string &word, int start = 0);
int findS(std::string &str, int start = 0, comparisonType t = NUM);
int endS(std::string &str, int start = 0, comparisonType t = NUM);
int getS(std::string &str, int start = 0);
bool getS(std::string &str, std::string &word ,int start = 0);
int nextNum(std::string &str, int &i);
int currNum(std::string &str, int &i);
int nextWord(std::string &str, std::string &word, int &i);
int currWord(std::string &str, std::string &word, int &i);
#endif
