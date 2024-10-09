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
#include <string.h>
#include <stdbool.h>
#include <sys/ioctl.h>

struct BinPic
{
    struct  { long x, y; } sz;
    char    *contents;
    char    *filename;
    char    *anfilename;
    size_t  filength;
    bool    putitle;
};

static void print_usage (void);
static void read_bin_file (struct BinPic *const);

static void get_terminal_dimensions (long *const, long *const);
static void display_colored_bin (struct BinPic *const);

int main (int argc, char **argv)
{
    if (argc == 1) print_usage();

    struct BinPic bp = {0};
    int32_t op;

    while ((op = getopt(argc, argv, ":Tha:b:")) != -1) {
        switch (op) {
            case 'T': bp.putitle    = true; break;
            case 'a': bp.anfilename = optarg; break;
            case 'b': bp.filename   = optarg; break;
            case 'h': print_usage(); break;
            case '?': errx(EXIT_FAILURE, "unknonwn '-%c' option...", optopt); break;
            case ':': errx(EXIT_FAILURE, "option '-%c' needs an argument...", optopt); break;
        }
    }

    if (!bp.filename) print_usage();

    read_bin_file(&bp);
    get_terminal_dimensions(&bp.sz.y, &bp.sz.x);

    display_colored_bin(&bp);
    return 0;
}

static void print_usage (void)
{
    fprintf(stderr, "binpic - binary picture (%s - %s)\n\n", __DATE__, __TIME__);
    fputs("Usage: binpic -b <filename> -[Th] -[a] <*>\n", stderr);
    fputs("Arguments:\n", stderr);
    fputs("  -T             Write file's name in the middle of the screen\n", stderr);
    fputs("  -a             Animate output with <*> anfilename\n", stderr);
    fputs("  -h             Display this message\n", stderr);
    exit(EXIT_SUCCESS);
}

static void read_bin_file (struct BinPic *const bp)
{
    FILE *bfile = fopen(bp->filename, "rb");
    if (!bfile) err(EXIT_FAILURE, "'%s' file is not valid", bp->filename);

    fseek(bfile, 0L, SEEK_END);
    bp->filength = ftell(bfile);
    fseek(bfile, 0L, SEEK_SET);

    /* Add two extra bytes in order to avoid conditionals
     * in `display_colored_bin` function. */
    bp->contents = (char*) calloc(bp->filength + 3, sizeof(char));
    if (!bp->contents) err(EXIT_FAILURE, "cannot allocate memory :(");

    const size_t read = fread(bp->contents, 1, bp->filength, bfile);
    if (read != bp->filength)
        warnx("cannot read whole file: %ld/%ld (B) were read", read, bp->filength);
    fclose(bfile);
}

static void get_terminal_dimensions (long *const lines, long *const cols)
{
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    *lines = sz.ws_row;
    *cols = sz.ws_col;
}

static void display_colored_bin (struct BinPic *const bp)
{
    printf("\x1b[2J");
    unsigned char rgb[3] = {0};

    for (size_t k = 0; k < bp->filength; k++) {
        if (!(k % bp->sz.x)) putchar(10);
        memcpy(rgb, bp->contents + k, 3);

        if (!rgb[0] && !rgb[1] && !rgb[2]) continue;
        printf("\x1b[48;2;%d;%d;%dm \x1b[0m", rgb[0], rgb[1], rgb[2]);
        //printf("%d;%d;%d\n", rgb[0], rgb[1], rgb[2]);
    }
}
