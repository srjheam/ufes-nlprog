#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "console.h"

void console_clear() { system("@cls||clear"); }

void console_msg(char *msg) {
    printf(ANSI_COLOR_YELLOW);

    if (msg != NULL && strlen(msg) > 0)
        printf("%s\n", msg);

    printf(ANSI_COLOR_RESET "Pressione ENTER para continuar.");
    scanf("%*c");
}
