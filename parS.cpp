#include <iostream>
#include <string>
#include <climits>
#include "parS.h"

int findWord(std::string &str, std::string &word, int start)
{
  if (start >= str.size())
  {
    std::cout << "findWord() received invalid input data\n";
    return -1;
  }
  if (word == "")
    return -1;
  int i = start, j = 0, ret = 0;
  while (i < str.size())
  {
    if (str[i] == word[j])
    {
      if (j == 0)
        ret = i;
      j++;
      if (j >= word.size())
        return ret;
    }
    else j = 0;
    i++;
  }
  return -1;
}

int findS(std::string &str, int start, comparisonType t)
{
  if (start >= str.size())
  {
    std::cout << "findS() received invalid input data\n";
    return -1;
  }
  int i = start;
  if (t == WORD)
    while (i < str.size() && !((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')))
      i++;
  else
    while (i < str.size() && (str[i] < '0' || str[i] > '9'))
      i++;
  if (i >= str.size())
  {
    return -1;
  }
  return i;
}

int endS(std::string &str, int start, comparisonType t)
{
  if (start >= str.size())
  {
    std::cout << "endS() received invalid input data\n";
    return -1;
  }
  int i = start;
  if (t == WORD)
    while (i < str.size() && ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')))
      i++;
  else
    while (i < str.size() && str[i] >= '0' && str[i] <= '9')
      i++;
  if (i >= str.size())
  {
    return -1;
  }
  return i;
}

int getS(std::string &str, int start)
{
  if (start >= str.size())
  {
    std::cout << "getS() received invalid input data\n";
    return -1;
  }
  int ret = 0;
  int i = start;
  while (i < str.size() && str[i] >= '0' && str[i] <= '9')
  {
    int temp = INT_MAX - (str[i] - '0');
    if (temp / 10 < ret || (temp / 10 == ret && temp % 10 > 0))
    {
      std::cout << "The number in the string exceeds the maximum int value\n";
      return -1;
    }
    else ret = ret * 10 + (str[i] - '0');
    i++;
  }
  return ret;
}

bool getS(std::string &str, std::string &word ,int start)
{
  if (start >= str.size())
  {
    std::cout << "getWord() received invalid input data\n";
    return false;
  }
  word = "";
  int i = start;
  while (i < str.size() && ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')))
  {
    word = word + str[i];
    i++;
  }
  return true;
}

int nextNum(std::string &str, int &i)
{
  i = endS(str, i, NUM);
  i = findS(str, i, NUM);
  return getS(str, i);
}

int currNum(std::string &str, int &i)
{
  i = findS(str, i, NUM);
  return getS(str, i);
}

int nextWord(std::string &str, std::string &word, int &i)
{
  i = endS(str, i, WORD);
  i = findS(str, i, WORD);
  return getS(str, word, i);
}

int currWord(std::string &str, std::string &word, int &i)
{
  i = findS(str, i, WORD);
  return getS(str, word, i);
}
