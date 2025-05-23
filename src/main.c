#include <jawc.h>
#include <stdio.h>
#include <curl/curl.h>
#include "cc.h"
#include "base.h"

#define MAX_URLS 100
static uint8_t urls_visited = 0;
static map(char *, u_int8_t) visited;

void web_crawl(lxb_html_document_t *doc)
{
  lxb_dom_collection_t *collection = jawc_get_all_by_attr(doc, "href", "http");
  lxb_dom_element_t *element = NULL;
  u_int8_t *url_visited = NULL;

  urls_visited += 1;
  printf("Visited: %d\n", urls_visited);

  for (size_t i = 0; i < lxb_dom_collection_length(collection); i++)
  {
    if (urls_visited == MAX_URLS)
      return;

    size_t value_len;
    element = lxb_dom_collection_element(collection, i);
    const lxb_char_t *attr = lxb_dom_element_get_attribute(element, (lxb_char_t *)"href", 4, &value_len);
    url_visited = get(&visited, (char *)attr);
    if (url_visited)
    {
      *url_visited += 1;
      continue;
    }

    if (!insert(&visited, (char *)attr, 1))
    {
      fprintf(stderr, "Ran out of memory.\n");
      return;
    }

    // serialize_node(lxb_dom_interface_node(element));

    printf("%s\n", (char *)attr);
    const char res = jawc_get_html((char *)attr);
    if (res)
    {
      fprintf(stderr, "cURL failed: %s\n", curl_easy_strerror(res));
      return;
    }
    jawc_parse_html(web_crawl);
  }

  lxb_dom_collection_destroy(collection, 1);
}

int main()
{
  init(&visited);
  if (!visited)
  {
    fprintf(stderr, "Failed to initialize visited url map.\n");
    return 1;
  }

  if (jawc_init())
  {
    fprintf(stderr, "Failed to initialize jawc.\n");
    return 1;
  }

  printf("jawc init successful.\n");

  const char res = jawc_get_html("https://www.google.com/");
  if (res)
  {
    fprintf(stderr, "cURL failed: %s\n", curl_easy_strerror(res));
    return 1;
  }

  printf("jawc got url.\n");

  jawc_parse_html(web_crawl);

  cleanup(&visited);
  jawc_destroy();
  return 0;
}