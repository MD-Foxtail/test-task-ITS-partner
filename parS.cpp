#include <iostream>
#include <string>
#include <climits>
#include "parS.h"
#include "logger.h"

LOG_CONNECT;

int findWord(std::string &str, std::string &word, int start)
{
  if (start >= str.size() || word == "")
  {
    LOG_ERROR("findWord(): received invalid input data");
    return -1;
  }
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
  LOG_ERROR("findWord(): The text does not contain the word '" + word + "'");
  return -1;
}

int findS(std::string &str, int start, comparisonType t)
{
  if (start >= str.size())
  {
    LOG_ERROR("findS(): received invalid input data");
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
    LOG_ERROR("findS(): The text does not contain '" + str + "'")
    return -1;
  }
  return i;
}

int endS(std::string &str, int start, comparisonType t)
{
  if (start >= str.size())
  {
    LOG_ERROR("endS(): received invalid input data");
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
    if (t == WORD)
      LOG_ERROR("endS(): The word completes the text. It is impossible to find the next character after the end of the word");
    if (t == NUM) LOG_ERROR("endS(): The number completes the text. It is impossible to find the next character after the end of the number");
    return -1;
  }
  return i;
}

int getS(std::string &str, int start)
{
  if (start >= str.size())
  {
    LOG_ERROR("getS(): received invalid input data");
    return -1;
  }
  int ret = 0;
  int i = start;
  while (i < str.size() && str[i] >= '0' && str[i] <= '9')
  {
    int temp = INT_MAX - (str[i] - '0');
    if (temp / 10 < ret || (temp / 10 == ret && temp % 10 > 0))
    {
      LOG_ERROR("getS(): The number in the text exceeds the maximum int value");
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
    LOG_ERROR("getS() received invalid input data\n");
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

int nextS(std::string &str, int &i)
{
  i = endS(str, i, NUM);
  i = findS(str, i, NUM);
  return getS(str, i);
}

int currS(std::string &str, int &i)
{
  i = findS(str, i, NUM);
  return getS(str, i);
}

bool nextS(std::string &str, std::string &word, int &i)
{
  i = endS(str, i, WORD);
  i = findS(str, i, WORD);
  return getS(str, word, i);
}

bool currS(std::string &str, std::string &word, int &i)
{
  i = findS(str, i, WORD);
  return getS(str, word, i);
}
