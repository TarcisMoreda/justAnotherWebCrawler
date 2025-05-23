#if !defined(JAWC_H)
#define JAWC_H

#include <lexbor/html/html.h>

char jawc_init(void);
void jawc_destroy(void);
char jawc_get_html(const char *const url);
char jawc_parse_html(void (*parse)(lxb_html_document_t *));
lxb_dom_collection_t *jawc_get_all_by_attr(lxb_html_document_t *doc, const char *attr, const char *val);

#endif // JAWC_H
