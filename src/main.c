#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "lexbor/html/html.h"

// Callback to store HTTP response in a buffer
typedef struct
{
  char *data;
  size_t size;
} ResponseBuffer;

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
  ResponseBuffer *buf = (ResponseBuffer *)userdata;
  size_t new_size = buf->size + size * nmemb;
  buf->data = realloc(buf->data, new_size + 1);
  memcpy(buf->data + buf->size, ptr, size * nmemb);
  buf->data[new_size] = '\0';
  buf->size = new_size;
  return size * nmemb;
}

// Recursively print text nodes (line by line)
void print_text_nodes(lxb_dom_node_t *node)
{
  while (node != NULL)
  {
    if (node->type == LXB_DOM_NODE_TYPE_TEXT)
    {
      lxb_char_t *text = lxb_dom_node_text_content(node, NULL);
      if (text != NULL)
      {
        printf("%s\n", (char *)text); // Print line by line
      }
    }
    // Recursively process child nodes
    print_text_nodes(lxb_dom_node_first_child(node));
    node = lxb_dom_node_next(node);
  }
}

int main()
{
  CURL *curl = curl_easy_init();
  if (!curl)
  {
    fprintf(stderr, "Failed to initialize CURL\n");
    return 1;
  }

  // Fetch URL
  ResponseBuffer buf = {0};
  curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK)
  {
    fprintf(stderr, "CURL failed: %s\n", curl_easy_strerror(res));
    free(buf.data);
    curl_easy_cleanup(curl);
    return 1;
  }

  // Parse HTML with Lexbor
  lxb_html_document_t *doc = lxb_html_document_create();
  lxb_status_t status = lxb_html_document_parse(doc, (lxb_char_t *)buf.data, buf.size);
  if (status != LXB_STATUS_OK)
  {
    fprintf(stderr, "Lexbor parsing failed\n");
    free(buf.data);
    lxb_html_document_destroy(doc);
    curl_easy_cleanup(curl);
    return 1;
  }

  // Print text content line by line
  print_text_nodes(lxb_dom_interface_node(doc->body));

  // Cleanup
  free(buf.data);
  lxb_html_document_destroy(doc);
  curl_easy_cleanup(curl);
  return 0;
}