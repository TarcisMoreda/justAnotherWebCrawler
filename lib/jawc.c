#include "jawc.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <lexbor/html/html.h>

/**
 * @brief Struct for the HTTP header
 */
typedef struct
{
    char *data;
    size_t size;
} ResponseBuffer;

static CURL *curl = NULL;
static ResponseBuffer curl_res = {0};

/**
 * @brief https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 *
 * @param data      Chunk of data to be saved
 * @param size      Size is always 1
 * @param nmemb     Size of the data to be saved
 * @param clientp   HTTP header
 * @return size_t   Size of the data
 */
size_t write_callback(char *data, size_t size, size_t nmemb, void *clientp)
{
    size_t real_size = size * nmemb;
    ResponseBuffer *buf = (ResponseBuffer *)clientp;

    char *ptr = realloc(buf->data, buf->size + real_size + 1);
    if (!ptr)
        return 0;

    buf->data = ptr;
    memcpy(&(buf->data[buf->size]), data, real_size);
    buf->size += real_size;
    buf->data[buf->size] = 0;

    return real_size;
}

/**
 * @brief Initializes cURL for jawc use.
 *
 * @return char 1 on sucess 0 on fail.
 */
char jawc_init(void)
{
    curl = curl_easy_init();
    if (!curl)
        return 0;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_res);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    return 1;
}

/**
 * @brief Frees all the memory, even tho the OS could take care of it...
 *
 */
void jawc_destroy(void)
{
    if (curl_res.data)
        free(curl_res.data);
    if (curl)
        curl_easy_cleanup(curl);
}

// void print_text_nodes(lxb_dom_node_t *node)
// {
//   while (node != NULL)
//   {
//     if (node->type == LXB_DOM_NODE_TYPE_TEXT)
//     {
//       lxb_char_t *text = lxb_dom_node_text_content(node, NULL);
//       if (text != NULL)
//       {
//         printf("%s\n", (char *)text); // Print line by line
//       }
//     }
//     // Recursively process child nodes
//     print_text_nodes(lxb_dom_node_first_child(node));
//     node = lxb_dom_node_next(node);
//   }
// }

// curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
//   CURLcode res = curl_easy_perform(curl);
//   if (res != CURLE_OK)
//   {
//     fprintf(stderr, "CURL failed: %s\n", curl_easy_strerror(res));
//     free(buf.data);
//     curl_easy_cleanup(curl);
//     return 1;
//   }
//   // Parse HTML with Lexbor
//   lxb_html_document_t *doc = lxb_html_document_create();
//   lxb_status_t status = lxb_html_document_parse(doc, (lxb_char_t *)buf.data, buf.size);
//   if (status != LXB_STATUS_OK)
//   {
//     fprintf(stderr, "Lexbor parsing failed\n");
//     free(buf.data);
//     lxb_html_document_destroy(doc);
//     curl_easy_cleanup(curl);
//     return 1;
//   }
//   // Print text content line by line
//   print_text_nodes(lxb_dom_interface_node(doc->body));
//   // Cleanup
//   free(buf.data);
//   lxb_html_document_destroy(doc);
//   curl_easy_cleanup(curl);
//   return 0;