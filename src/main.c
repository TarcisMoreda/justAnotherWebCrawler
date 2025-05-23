#include <jawc.h>
#include <stdio.h>
#include <curl/curl.h>
#include "cc.h"
#include "base.h"

void print_text_nodes(lxb_dom_node_t *node)
{
  while (node != NULL)
  {
    if (node->type == LXB_DOM_NODE_TYPE_TEXT)
    {
      lxb_char_t *text;
      size_t len;
      text = lxb_dom_node_text_content(node, &len);

      if (text != NULL)
        printf("%.*s\n", (int)len, (char *)text);
    }
    print_text_nodes(lxb_dom_node_first_child(node));
    node = lxb_dom_node_next(node);
  }
}

void print_text(lxb_html_document_t *doc)
{
  lxb_dom_node_t *node = lxb_dom_interface_node(doc->body);
  print_text_nodes(node);
}

void get_all_tags(lxb_html_document_t *doc)
{
  lxb_dom_element_t *element = NULL;
  lxb_dom_element_t *body = lxb_dom_interface_element(doc->body);
  lxb_dom_collection_t *collection = lxb_dom_collection_make(&doc->dom_document, 128);
  if (!collection)
  {
    fprintf(stderr, "Failed to create Collection object.\n");
    return;
  }

  lxb_status_t status = lxb_dom_elements_by_attr_begin(body, collection,
                                                       (const lxb_char_t *)"href", 4,
                                                       (const lxb_char_t *)"http", 4,
                                                       1);

  if (status)
  {
    fprintf(stderr, "Failed to get elements with 'href' attribute.\n");
    return;
  }

  for (size_t i = 0; i < lxb_dom_collection_length(collection); i++)
  {
    element = lxb_dom_collection_element(collection, i);
    serialize_node(lxb_dom_interface_node(element));
  }

  lxb_dom_collection_destroy(collection, 1);
}

int main()
{
  if (jawc_init())
  {
    fprintf(stderr, "Failed to initialize jawc.\n");
    return 1;
  }

  printf("jawc init successful.\n");

  char res = jawc_get_html("https://example.com");
  if (res)
  {
    fprintf(stderr, "cURL failed: %s\n", curl_easy_strerror(res));
    return 1;
  }

  printf("jawc got url.\n");

  jawc_parse_html(get_all_tags);

  jawc_destroy();
  return 0;
}