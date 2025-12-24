#include <stl/manstr.h>
#include <string.h>

void strclean(char* restrict compressed, const char* restrict scattered)
{
    if(!compressed) return ;
    if(!scattered) {
        compressed[0] = '\0';
        return;
    }

    size_t j = 0;
    for(size_t i = 0; scattered[i] != '\0'; ++i) {
        if(scattered[i] != '\t') {
            compressed[j++] = scattered[i];
        }
    }

    compressed[j++] = '\0';
}
