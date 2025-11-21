#include "../include/Utils.h"

void copyString(const char* src, char* dest, int maxLen) {
    if (!src || !dest || maxLen <= 0) return;

    int i = 0;
    while (src[i] && i < maxLen - 1) {
        dest[i] = src[i];
        ++i;
    }
    dest[i] = '\0';
}
