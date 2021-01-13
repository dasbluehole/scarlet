//mime.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mime.h"

char* get_mime_type(char *filename)
{
    char *dot = strrchr(filename, '.');
    if(dot){ // strrchar Locate last occurrence of character in string
        mime_map *map = meme_types;
        while(map->extension){
            if(strcmp(map->extension, dot) == 0){
                return map->mime_type;
            }
            map++;
        }
    }
    return default_mime_type;
}
