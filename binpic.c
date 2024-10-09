/*    _   ,_,   _
 *   / `'=) (='` \      binpic - main file.
 *  /.-.-.\ /.-.-.\     Oct 9 2024
 *         "      `
 */
#include <err.h>
#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct BinPic
{
    char    *animation;
    bool    title;
};

static void print_usage (void);

int main (int argc, char **argv)
{
    struct BinPic bp = {0};
    int32_t op;

    while ((op = getopt(argc, argv, ":Tha:")) != -1) {
        switch (op) {
            case 'h': print_usage();
            case 'T': bp.title = true; break;
            case 'a': bp.animation = optarg; break;
            case '?': errx(EXIT_FAILURE, "unknonwn '-%c' option...", optopt); break;
            case ':': errx(EXIT_FAILURE, "option '-%c' needs an argument...", optopt); break;
        }
    }

    return 0;
}

static void print_usage (void)
{
    fprintf(stderr, "binpic - binary picture (%s - %s)\n\n", __DATE__, __TIME__);
    fputs("Usage: binpic <filename> -[Th] -[a] <*>\n", stderr);
    fputs("Arguments:\n", stderr);
    fputs("  -T             Write file's name in the middle of the screen\n", stderr);
    fputs("  -a             Animate output with <*> animation\n", stderr);
    fputs("  -h             Display this message\n", stderr);
    exit(EXIT_SUCCESS);
}
