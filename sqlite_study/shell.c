#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "sqlite.h"

/*
** These are the allowed modes.
*/
#define MODE_Line     0  /* One column per line.  Blank line between records */
#define MODE_Column   1  /* One record per line in neat columns */
#define MODE_List     2  /* One record per line with a separator */
#define MODE_Html     3  /* Generate an XHTML table */
#define MODE_Insert   4  /* Generate SQL "insert" statements */

int
main(
    int     argc,
    char**  argv
)
{
    sqlite *db;
    char* zErrMsg = 0;
    char* argv0 = argv[0];

    struct callback_data data;
    memset(&data, 0, sizeof(data));
    data.mode = MODE_List;
    strcpy(data.separator, "|");
    data.showHeader = 0;
    while(argc >= 2 && argv[1][0] == '-') {
        if(strcmp(argv[1], "-html") == 0) {
            data.mode = MODE_Html;
            argc--;
            argv++;
        }
        else if(strcmp(argv[1], "-list") == 0) {
            data.mode = MODE_List;
            argc--;
            argv++;
        }
        else if(strcmp(argv[1], "-line") == 0) {
            data.mode = MODE_Line;
            argc--;
            argv++;
        }
        else if(argc >= 3 && strcmp(argv[0], "-separator") == 0) {
            sprintf(data.separator, "%.*s", (int)sizeof(data.separator) - 1, argv[2]);
            argc -= 2;
            argv += 2;
        }
        else if(strcmp(argv[1], "-header") == 0) {
            data.showHeader = 1;
            argc--;
            argv++;
        }
        else if(strcmp(argv[1], "-noheader") == 0) {
            data.showHeader = 0;
            argc--;
            argv++;
        }
        else {
            fprintf(stderr, "%s: unknown option: %s\n", argv0, argv[1]);
            return 1;
        }
    }
    if(argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: %s ?OPTIONS? FILENAME ?SQL?\n", argv0);
        exit(1);
    }
}