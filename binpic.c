#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    FILE *file = fopen("inst", "rb");

    fseek(file, 0L, SEEK_END);
    size_t bytes = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char *contents = (char*) calloc(bytes, sizeof(char));
    fread(contents, 1, bytes, file);
    fclose(file);

    for (size_t i = 0; i < bytes; i++) {
        printf("%02x ", (unsigned char) contents[i]);
        if (((i + 1) % 16) == 0) putchar(10);
    }

    printf("%s\n", contents);

    return 0;
}
