#include <iostream>
#include <curl/curl.h>
#include <string>

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
  std::cout << strResponse << "\n";
}
