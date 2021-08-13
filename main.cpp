#include <iostream>
#include <curl/curl.h>
int main()
{
  CURL* curl;
  CURLcode responce;
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_HEADER, 1);
  curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.ru/");
  responce = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
}
