#include <jawc.h>
#include <stdio.h>
#include <curl/curl.h>

void print_text_nodes(lxb_dom_node_t *_, lxb_dom_node_t *node)
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
    print_text_nodes(NULL, lxb_dom_node_first_child(node));
    node = lxb_dom_node_next(node);
  }
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

  jawc_parse_html(print_text_nodes);

  jawc_destroy();
  return 0;
}