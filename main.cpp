#include <iostream>
#include <curl/curl.h>
#include <string>
#include "parS.h"

struct time
{
  int day, month, year, hour, min, sec;
};

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
  _time.day = currNum(str, i);
  i = findS(str, i, WORD);
  getS(str, word, i);
  for (int j = 0; j < 12; j++)
    if (word == monthName[j])
      {
        _time.month = j;
        break;
      }
  _time.year = currNum(str, i);
  _time.hour = nextNum(str, i);
  _time.min = nextNum(str, i);
  _time.sec = nextNum(str, i);
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
