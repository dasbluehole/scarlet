#ifndef __URL_H__
#define __URL_H__

char from_hex(char ch);

char to_hex(char code);

char *url_encode(char *str);

char *url_decode(char *str);

#endif
