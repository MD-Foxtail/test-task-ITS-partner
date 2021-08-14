#include <iostream>
#include <curl/curl.h>
#include <string>
#include <cstdlib>
#include "parS.h"

const std::string monthName[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const int UTC = +3;

struct time
{
  int day, month, year, hour, min, sec;
};

void timeElToStr(std::string &newTime, int elem)
{
  if (elem < 10)
    newTime += "0";
  newTime += std::to_string(elem);
}

void timeToStr(std::string &newTime, struct time &_time)
{
  timeElToStr(newTime, _time.month);
  timeElToStr(newTime, _time.day);
  timeElToStr(newTime, _time.hour);
  timeElToStr(newTime, _time.min);
  newTime += std::to_string(_time.year) + ".";
  timeElToStr(newTime, _time.sec);
}

bool setTime(struct time &_time)
{
  std::string newTime = "date ";
  timeToStr(newTime, _time);
  system(newTime.c_str());
  return true;
}

bool getTime(std::string &str, struct time &_time)
{
  std::string word = "Date:";
  int i = findWord(str, word);
  _time.day = currS(str, i);
  currS(str, word, i);
  for (int j = 0; j < 12; j++)
    if (word == monthName[j])
      {
        _time.month = j + 1;
        break;
      }
  _time.year = currS(str, i);
  _time.hour = nextS(str, i) + UTC;
  _time.min = nextS(str, i);
  _time.sec = nextS(str, i);
  return true;
}

static size_t getResponseToString(void* contents, size_t size, size_t nmemb, void* userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

bool httpCreator(std::string &strResponse, std::string http)
{
  CURL* curl;
  curl = curl_easy_init();
  if (!curl)
  {
    std::cout << "init failed\n";
    return false;
  }
  curl_easy_setopt(curl, CURLOPT_HEADER, 1);
  curl_easy_setopt(curl, CURLOPT_URL, http.c_str());
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

bool start (std::string &strResponse, std::string http)
{
  while (true)
    if (!httpCreator(strResponse, http))
    {
      char restartHttp;
      std::cout << "create a new http request?(y - yes, n - no)\n";
      std::cin >> restartHttp;
      if (restartHttp != 'y')
        return false;
    }
    else return true;
  return true;
}

int main()
{
  std::string strResponse;
  start (strResponse, "http://google.com");
  struct time _time;
  getTime(strResponse, _time);
  setTime(_time);
  strResponse = "";
  start (strResponse, "https://example.com");
  std::cout << strResponse << "\n";
}
