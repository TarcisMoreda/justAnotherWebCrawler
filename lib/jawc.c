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
char jawc_parse_html(void (*parse)(lxb_html_document_t *))
{
    lxb_status_t status = lxb_html_document_parse(doc, (lxb_char_t *)curl_res.data, curl_res.size);
    if (status)
        return status;
    parse(doc);
    return status;
}

lxb_dom_collection_t *jawc_get_all_by_attr(lxb_html_document_t *doc, const char *attr, const char *val)
{
    lxb_dom_element_t *body = lxb_dom_interface_element(doc->body);
    lxb_dom_collection_t *collection = lxb_dom_collection_make(&doc->dom_document, 128);
    if (!collection)
        return NULL;

    lxb_status_t status = lxb_dom_elements_by_attr_begin(body, collection,
                                                         (const lxb_char_t *)attr, strlen(attr),
                                                         (const lxb_char_t *)val, strlen(val),
                                                         1);

    if (status)
        return NULL;

    return collection;
}