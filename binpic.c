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
    struct  { long lines, cols; } termsz;
    char    *contents;
    char    *filename;
    char    *anfilename;
    size_t  filength;
    bool    putitle;
};

static void print_usage (void);
static void read_bin_file (struct BinPic *const);

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

    bp->contents = (char*) calloc(bp->filength + 1, sizeof(char));
    if (!bp->contents) err(EXIT_FAILURE, "cannot allocate memory :(");

    const size_t read = fread(bp->contents, 1, bp->filength, bfile);
    if (read != bp->filength)
        warnx("cannot read whole file: %ld/%ld (B) were read", read, bp->filength);
    fclose(bfile);
}
