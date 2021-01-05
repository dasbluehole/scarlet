# !/bin/bash
rm -f scarlet
gcc -g scarlet.c request.c util.c server_utils.c url.c -o scarlet -lpthread
