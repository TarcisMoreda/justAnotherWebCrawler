#include "jawc.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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
static lxb_html_document_t *doc = NULL;

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
 * @brief Initializes cURL for jawc use
 *
 * @return char 0 on sucess 1 on fail
 */
char jawc_init(void)
{
    doc = lxb_html_document_create();
    curl = curl_easy_init();
    if (!curl)
        return 1;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_res);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    return 0;
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
    if (doc)
        lxb_html_document_destroy(doc);
}

/**
 * @brief Uses curl to get the html contents of a web page
 *
 * @param url       URL to the web page
 * @return char     Returns a CURLcode response
 */
char jawc_get_html(const char *const url)
{
    curl_easy_setopt(curl, CURLOPT_URL, url);
    CURLcode res = curl_easy_perform(curl);
    return res;
}

/**
 * @brief Parses the html using lexbor and run the parse function
 *
 * @param parse A function that recieves two lexbor nodes, the first is the html head and the second is the body
 * @return char Returns a lxb_status_t response
 */
char jawc_parse_html(void (*parse)(lxb_dom_node_t *, lxb_dom_node_t *))
{
    lxb_status_t status = lxb_html_document_parse(doc, (lxb_char_t *)curl_res.data, curl_res.size);
    if (status)
        return status;
    parse(lxb_dom_interface_node(doc->head), lxb_dom_interface_node(doc->body));
    return status;
}