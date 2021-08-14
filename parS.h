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
int nextS(std::string &str, int &i);
int currS(std::string &str, int &i);
bool nextS(std::string &str, std::string &word, int &i);
bool currS(std::string &str, std::string &word, int &i);
#endif
