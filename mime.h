//mime.h
#ifndef __MIME_H__
#define __MIME_H__
typedef struct {
    const char *extension;
    const char *mime_type;
} mime_map;
static char *default_mime_type = "text/plain";
static mime_map meme_types [] = {
    {".css", "text/css"},
    {".gif", "image/gif"},
    {".htm", "text/html"},
    {".html", "text/html"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpeg"},
    {".ico", "image/x-icon"},
    {".js", "application/javascript"},
    {".pdf", "application/pdf"},
    {".mp4", "video/mp4"},
    {".png", "image/png"},
    {".svg", "image/svg+xml"},
    {".xml", "text/xml"},
    {NULL, NULL},
};

char* get_mime_type(char *filename);

#endif
