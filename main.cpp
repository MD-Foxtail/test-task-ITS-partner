#include <iostream>
#include <curl/curl.h>
#include <string>
#include <limits.h>

enum comparisonType
{
  NUM,
  WORD
};

struct time
{
  int day, month, year, hour, min, sec;
};

int findWord(std::string &str, std::string &word, int start = 0)
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

int findS(std::string &str, int start = 0, comparisonType t = NUM)
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

int endS(std::string &str, int start = 0, comparisonType t = NUM)
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
  if (i >= str.size() || i == start)
  {
    return -1;
  }
  return i;
}

int getS(std::string &str, int start = 0)
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

bool getS(std::string &str, std::string &word ,int start = 0)
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

bool getTime(std::string &str, struct time &_time)
{
  const std::string monthName[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  std::string word = "date:";
  int i = findWord(str, word);
  if (i == -1)
  {
    std::cout << "The string does not contain a date\n";
    return false;
  }
  i = findS(str, i, NUM);
  _time.day = getS(str, i);
  i = findS(str, i, WORD);
  getS(str, word, i);
  for (int j = 0; j < 12; j++)
    if (word == monthName[j])
      {
        _time.month = j;
        break;
      }
  i = findS(str, i, NUM);
  _time.year = getS(str, i);
  i = endS(str, i, NUM);
  i = findS(str, i, NUM);
  _time.hour = getS(str, i);
  i = endS(str, i, NUM);
  i = findS(str, i, NUM);
  _time.min = getS(str, i);
  i = endS(str, i, NUM);
  i = findS(str, i, NUM);
  _time.sec = getS(str, i);
  std::cout << _time.day << " " << _time.month + 1 << " " << _time.year << " " << _time.hour << ":" << _time.min << ":" << _time.sec;
  return true;
}

static size_t getResponseToString(void* contents, size_t size, size_t nmemb, void* userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

bool httpCreator(std::string &strResponse)
{
  CURL* curl;
  curl = curl_easy_init();
  if (!curl)
  {
    std::cout << "init failed\n";
    return false;
  }
  curl_easy_setopt(curl, CURLOPT_HEADER, 1);
  curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.ru/");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponseToString);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponse);
  CURLcode result = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  if (result != CURLE_OK)
  {
    std::cout << "download problem: " << curl_easy_strerror(result) << "\n";
    return false;
  }
  return true;
}

int main()
{
  std::string strResponse;
  while (true)
    if (!httpCreator(strResponse))
    {
      char restartHttp;
      std::cout << "create a new http request?(y - yes, n - no)\n";
      std::cin >> restartHttp;
      if (restartHttp != 'y')
        return 0;
    }
    else break;
  struct time _time;
  getTime(strResponse, _time);
}
