/*    _   ,_,   _
 *   / `'=) (='` \      binpic - main file.
 *  /.-.-.\ /.-.-.\     Oct 9 2024
 *         "      `
 */
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ioctl.h>

uint32_t Colors[256] = {
  0x340000, 0x560000, 0x640000, 0x750000, 0x870000, 0x9b0000, 0xb00000, 0xc60000,
  0xdd0000, 0xf50000, 0xff0f0f, 0xff2828, 0xff4343, 0xff5e5e, 0xff7979, 0xfe9595,
  0x4c1600, 0x561900, 0x641e00, 0x752300, 0x872800, 0x9b2e00, 0xb03400, 0xc63b00,
  0xdd4200, 0xf54900, 0xff570f, 0xff6928, 0xff7b43, 0xff8e5e, 0xffa179, 0xfeb595,
  0x4c3900, 0x564000, 0x644b00, 0x755700, 0x876500, 0x9b7400, 0xb08400, 0xc69400,
  0xdda600, 0xf5b800, 0xffc30f, 0xffc928, 0xffd043, 0xffd65e, 0xffdd79, 0xfee495,
  0x4c4c00, 0x565600, 0x646400, 0x757500, 0x878700, 0x9b9b00, 0xb0b000, 0xc6c600,
  0xdddd00, 0xf5f500, 0xffff0f, 0xffff28, 0xffff43, 0xffff5e, 0xffff79, 0xfffe95,
  0x324c00, 0x395600, 0x426400, 0x4e7500, 0x5a8700, 0x679b00, 0x75b000, 0x84c600,
  0x93dd00, 0xa3f500, 0xafff0f, 0xb7ff28, 0xc0ff43, 0xc9ff5e, 0xd2ff79, 0xdbfe95,
  0x1f4c00, 0x235600, 0x296400, 0x307500, 0x388700, 0x409b00, 0x49b000, 0x52c600,
  0x5cdd00, 0x66f500, 0x73ff0f, 0x82ff28, 0x91ff43, 0xa1ff5e, 0xb1ff79, 0xc1fe95,
  0x004c00, 0x005600, 0x006400, 0x007500, 0x008700, 0x009b00, 0x00b000, 0x00c600,
  0x00dd00, 0x00f500, 0x0fff0f, 0x28ff28, 0x43ff43, 0x5eff5e, 0x79ff79, 0x95fe95,
  0x004c19, 0x00561c, 0x006421, 0x007527, 0x00872d, 0x009b33, 0x00b03a, 0x00c642,
  0x00dd49, 0x00f551, 0x0fff5f, 0x28ff70, 0x43ff81, 0x5eff93, 0x79ffa6, 0x95feb8,
  0x004c4c, 0x005656, 0x006464, 0x007575, 0x008787, 0x009b9b, 0x00b0b0, 0x00c6c6,
  0x00dddd, 0x00f5f5, 0x0ffffe, 0x28fffe, 0x43fffe, 0x5efffe, 0x79ffff, 0x95fffe,
  0x00394c, 0x004056, 0x004b64, 0x005775, 0x006587, 0x00749b, 0x0084b0, 0x0094c6,
  0x00a6dd, 0x00b8f5, 0x0fc3ff, 0x28c9ff, 0x43d0ff, 0x5ed6ff, 0x79ddff, 0x95e4fe,
  0x00264c, 0x002b56, 0x003264, 0x003a75, 0x004387, 0x004d9b, 0x0058b0, 0x0063c6,
  0x006edd, 0x007af5, 0x0f87ff, 0x2893ff, 0x43a1ff, 0x5eaeff, 0x79bcff, 0x95cafe,
  0x00134c, 0x001556, 0x001964, 0x001d75, 0x002187, 0x00269b, 0x002cb0, 0x0031c6,
  0x0037dd, 0x003df5, 0x0f4bff, 0x285eff, 0x4372ff, 0x5e86ff, 0x799aff, 0x95b0fe,
  0x19004c, 0x1c0056, 0x210064, 0x270075, 0x2d0087, 0x33009b, 0x3a00b0, 0x4200c6,
  0x4900dd, 0x5100f5, 0x5f0fff, 0x7028ff, 0x8143ff, 0x935eff, 0xa679ff, 0xb895fe,
  0x33004c, 0x390056, 0x420064, 0x4e0075, 0x5a0087, 0x67009b, 0x7500b0, 0x8400c6,
  0x9300dd, 0xa300f5, 0xaf0fff, 0xb728ff, 0xc043ff, 0xc95eff, 0xd279ff, 0xdb95fe,
  0x4c004c, 0x560056, 0x640064, 0x750075, 0x870087, 0x9b009b, 0xb000b0, 0xc600c6,
  0xdd00dd, 0xf500f5, 0xfe0fff, 0xfe28ff, 0xfe43ff, 0xfe5eff, 0xfe79ff, 0xfe95fe,
  0x4c0032, 0x560039, 0x640042, 0x75004e, 0x87005a, 0x9b0067, 0xb00075, 0xc60084,
  0xdd0093, 0xf500a3, 0xff0faf, 0xff28b7, 0xff43c0, 0xff5ec9, 0xff79d2, 0xff79f5,
};

struct BinPic
{
    uint32_t screenwidth;
    size_t   filength;
    char     *contents;
};

static void print_usage (void);
static void read_bin_file (struct BinPic *const, const char *const);

static void get_screen_width (uint32_t *const);
static void display_colored_bin (struct BinPic *const, const char *const);

int main (int argc, char **argv)
{
    if (argc == 1) print_usage();

    struct BinPic bp = {0};

    read_bin_file(&bp, argv[1]);
    get_screen_width(&bp.screenwidth);

    display_colored_bin(&bp, argv[1]);
    return 0;
}

static void print_usage (void)
{
    fprintf(stderr, "binpic - binary picture (%s - %s)\n\n", __DATE__, __TIME__);
    fputs("Usage: binpic <filename>\n", stderr);
    exit(EXIT_SUCCESS);
}

static void read_bin_file (struct BinPic *const bp, const char *const filename)
{
    FILE *bfile = fopen(filename, "rb");
    if (!bfile) err(EXIT_FAILURE, "'%s' file is not valid", filename);

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

static void get_screen_width (uint32_t *const width)
{
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    *width = sz.ws_col;
}

static void display_colored_bin (struct BinPic *const bp, const char *const name)
{
    printf("\x1b[2J");
    uint8_t r = 0, g = 0, b = 0;

    for (size_t k = 0; k < bp->filength; k++) {
        uint8_t index = (uint8_t) bp->contents[k];
        const uint32_t color = (index) ? Colors[(uint8_t) bp->contents[k]] : Colors[(index + r + b + g) % 255];

        r = (color >> 16) & 0xff;
        g = (color >> 8) & 0xff;
        b = (color) & 0xff;

        printf("\x1b[48;2;%d;%d;%dm ", r, g, b);
    }
    printf("\x1b[0m\x1b[2m\n");

    const size_t msglen = strlen(name) + strlen("This is how '' looks like; c'est magnifique...!") + 1;
    char *message = (char*) calloc(msglen, sizeof(char));

    snprintf(message, msglen, "This is how '%s' looks like; c'est magnifique...!", name);

    printf("%*s< \x1b[5m%s\x1b[25m >\n", (uint32_t) (bp->screenwidth - msglen - 4), " ", message);
    printf("%*s", bp->screenwidth - 15, "     \\   ^__^\n");
    printf("%*s", bp->screenwidth - 7,  "      \\  (oo)\\_______\n");
    printf("%*s", bp->screenwidth - 3, "         (__)\\       )\\/\\\n");
    printf("%*s", bp->screenwidth - 6, "             ||-----w||\n");
    printf("%*s", bp->screenwidth - 6, "             ||      ||\n");

    free(bp->contents);
    free(message);
    printf("\x1b[0m");
}
