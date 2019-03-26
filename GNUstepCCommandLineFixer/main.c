/******************************************************************************************************************************//**
 *     PROJECT: GNUstepCCommandLineFixer
 *    FILENAME: main.c
 *         IDE: AppCode
 *      AUTHOR: Galen Rhodes
 *        DATE: 12/14/2018
 * DESCRIPTION:
 *
 * Copyright © 2018 Project Galen. All rights reserved.
 *
 * "It can hardly be a coincidence that no language on Earth has ever produced the expression 'As pretty as an airport.' Airports
 * are ugly. Some are very ugly. Some attain a degree of ugliness that can only be the result of special effort."
 * - Douglas Adams from "The Long Dark Tea-Time of the Soul"
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *//******************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <regex.h>

#ifndef _DOTEST_
    #define _DOTEST_ 0
#endif

typedef long long lngint;

/*
00 E ''         1A E $'\032'    34 - 4          4E - N          68 - h
01 E $'\001'    1B E $'\E'      35 - 5          4F - O          69 - i
02 E $'\002'    1C E $'\034'    36 - 6          50 - P          6A - j
03 E $'\003'    1D E $'\035'    37 - 7          51 - Q          6B - k
04 E $'\004'    1E E $'\036'    38 - 8          52 - R          6C - l
05 E $'\005'    1F E $'\037'    39 - 9          53 - S          6D - m
06 E $'\006'    20 E \          3A - :          54 - T          6E - n
07 E $'\a'      21 E \!         3B E \;         55 - U          6F - o
08 E $'\b'      22 E \"         3C E \<         56 - V          70 - p
09 E $'\t'      23 E \#         3D - =          57 - W          71 - q
0A E $'\n'      24 E \$         3E E \>         58 - X          72 - r
0B E $'\v'      25 - %          3F E \?         59 - Y          73 - s
0C E $'\f'      26 E \&         40 - @          5A - Z          74 - t
0D E $'\r'      27 E \'         41 - A          5B E \[         75 - u
0E E $'\016'    28 E \(         42 - B          5C E \\         76 - v
0F E $'\017'    29 E \)         43 - C          5D E \]         77 - w
10 E $'\020'    2A E \*         44 - D          5E E \^         78 - x
11 E $'\021'    2B - +          45 - E          5F - _          79 - y
12 E $'\022'    2C E \,         46 - F          60 E \`         7A - z
13 E $'\023'    2D - -          47 - G          61 - a          7B E \{
14 E $'\024'    2E - .          48 - H          62 - b          7C E \|
15 E $'\025'    2F - /          49 - I          63 - c          7D E \}
16 E $'\026'    30 - 0          4A - J          64 - d          7E E \~
17 E $'\027'    31 - 1          4B - K          65 - e          7F E $'\177'
18 E $'\030'    32 - 2          4C - L          66 - f
19 E $'\031'    33 - 3          4D - M          67 - g
 */

/****************************************************************************************************************************************************//**
 * This translation table is used to escape certain characters for a command-line.  This table is setup for BASH and other similar SH command
 * processors.  It may work with CSH but I haven't tested it.  For Windows CMD.EXE processor someone else will have to come up with a translation table
 * because I don't have the need nor the time (IE motivation) to learn enough about CMD.EXE to create one.
 *
 * A value of zero (0) in the table below means the character does NOT need to be escaped and can be used as-is.  Otherwise the string
 * given is put in the character's place.
 *
 * This table may seem upside down with the values 0 (0x00) thru 127 (0x7F) at the end and the values 128 (0x80) thru 255 (0xFF) at the beginning but
 * this is because the characters are all signed which means half the values (those over 0x7F) are actually negative even when cast as an
 * <code>int</code> value and I don't like casting signed values to unsigned.  So we simply add 128 (0x80) to the characters value to arrive at an
 * index for the array of 0 <= idx <= 255.
 */
static const char *XLAT[] = { //@f:0
    "$'\\200'", "$'\\201'", "$'\\202'", "$'\\203'", "$'\\204'", "$'\\205'", "$'\\206'", "$'\\207'", // 80 - 87
    "$'\\210'", "$'\\211'", "$'\\212'", "$'\\213'", "$'\\214'", "$'\\215'", "$'\\216'", "$'\\217'", // 88 - 8f
    "$'\\220'", "$'\\221'", "$'\\222'", "$'\\223'", "$'\\224'", "$'\\225'", "$'\\226'", "$'\\227'", // 90 - 87
    "$'\\230'", "$'\\231'", "$'\\232'", "$'\\233'", "$'\\234'", "$'\\235'", "$'\\236'", "$'\\237'", // 98 - 9f
    "$'\\240'", "$'\\241'", "$'\\242'", "$'\\243'", "$'\\244'", "$'\\245'", "$'\\246'", "$'\\247'", // a0 - 87
    "$'\\250'", "$'\\251'", "$'\\252'", "$'\\253'", "$'\\254'", "$'\\255'", "$'\\256'", "$'\\257'", // a8 - af
    "$'\\260'", "$'\\261'", "$'\\262'", "$'\\263'", "$'\\264'", "$'\\265'", "$'\\266'", "$'\\267'", // b0 - 87
    "$'\\270'", "$'\\271'", "$'\\272'", "$'\\273'", "$'\\274'", "$'\\275'", "$'\\276'", "$'\\277'", // b8 - bf
    "$'\\300'", "$'\\301'", "$'\\302'", "$'\\303'", "$'\\304'", "$'\\305'", "$'\\306'", "$'\\307'", // c0 - 87
    "$'\\310'", "$'\\311'", "$'\\312'", "$'\\313'", "$'\\314'", "$'\\315'", "$'\\316'", "$'\\317'", // c8 - cf
    "$'\\320'", "$'\\321'", "$'\\322'", "$'\\323'", "$'\\324'", "$'\\325'", "$'\\326'", "$'\\327'", // d0 - 87
    "$'\\330'", "$'\\331'", "$'\\332'", "$'\\333'", "$'\\334'", "$'\\335'", "$'\\336'", "$'\\337'", // d8 - df
    "$'\\340'", "$'\\341'", "$'\\342'", "$'\\343'", "$'\\344'", "$'\\345'", "$'\\346'", "$'\\347'", // e0 - 87
    "$'\\350'", "$'\\351'", "$'\\352'", "$'\\353'", "$'\\354'", "$'\\355'", "$'\\356'", "$'\\357'", // e8 - ef
    "$'\\360'", "$'\\361'", "$'\\362'", "$'\\363'", "$'\\364'", "$'\\365'", "$'\\366'", "$'\\367'", // f0 - 87
    "$'\\370'", "$'\\371'", "$'\\372'", "$'\\373'", "$'\\374'", "$'\\375'", "$'\\376'", "$'\\377'", // f8 - ff

    0,          "$'\\001'", "$'\\002'", "$'\\003'", "$'\\004'", "$'\\005'", "$'\\006'", "$'\\007'", // 00 - 87
    "$'\\010'", "$'\\011'", "$'\\012'", "$'\\013'", "$'\\014'", "$'\\015'", "$'\\016'", "$'\\017'", // 08 - 0f
    "$'\\020'", "$'\\021'", "$'\\022'", "$'\\023'", "$'\\024'", "$'\\025'", "$'\\026'", "$'\\027'", // 10 - 87
    "$'\\030'", "$'\\031'", "$'\\032'", "\\E",      "$'\\034'", "$'\\035'", "$'\\036'", "$'\\037'", // 18 - 1f
    "\\ ",      "\\!",      "\\\"",     "\\#",      "\\$",      0,          "\\&",      "\\'",      // 20 - 87
    "\\(",      "\\)",      "\\*",      0,          "\\,",      0,          0,          0,          // 28 - 2f
    0,          0,          0,          0,          0,          0,          0,          0,          // 30 - 87
    0,          0,          0,          "\\;",      "\\<",      0,          "\\>",      "\\?",      // 38 - 3f
    0,          0,          0,          0,          0,          0,          0,          0,          // 40 - 87
    0,          0,          0,          0,          0,          0,          0,          0,          // 48 - 4f
    0,          0,          0,          0,          0,          0,          0,          0,          // 50 - 87
    0,          0,          0,          "\\[",      "\\\\",     "\\]",      "\\^",      0,          // 58 - 5f
    "\\`",      0,          0,          0,          0,          0,          0,          0,          // 60 - 87
    0,          0,          0,          0,          0,          0,          0,          0,          // 68 - 6f
    0,          0,          0,          0,          0,          0,          0,          0,          // 70 - 87
    0,          0,          0,          "\\{",      "\\|",      "\\}",      "\\~",      "$'\\177'"  // 78 - 7f
}; // @f:1

/****************************************************************************************************************************************************//**
 * Error message for running out of memory.
 */
char *const NOT_ENOUGH_MEMORY = "Not Enough Memory";

/****************************************************************************************************************************************************//**
 * The regex pattern used to identify the GNUstep Objective-C runtime flag argument and isolate the runtime library version information from it.
 */
char *const RTPARAMREGEX = "^-fobjc-runtime=gnustep-([0-9]{1,10})\\.([0-9]{1,10})(\\.([0-9]{1,10}))?$";

/****************************************************************************************************************************************************//**
 * The GNUstep Objective-C runtime flag argument's library version information will be in the regex's capturing groups identified by the capturing
 * group indexes below.
 */
const size_t MAJOR_GROUP_IDX = 1;
const size_t MINOR_GROUP_IDX = 2;
const size_t HOTFX_GROUP_IDX = 4;

/****************************************************************************************************************************************************//**
 * The total number of capturing groups in the GNUstep Objective-C runtime flag argument regex.
 */
const size_t RTPARAMREGEXGROUPS = 5;

/****************************************************************************************************************************************************//**
 * The initial size of the output buffer.
 */
const size_t INITIAL_WORK_BUFFER_SIZE = 8192; // 8KB starting size.

/****************************************************************************************************************************************************//**
 * Used for converting the version to integer values.
 */
char TEMP[12];

/****************************************************************************************************************************************************//**
 * A structure to hold the current work.
 */
typedef struct __context__ {
    // the original arguments
    int        argc;
    const char **argv;
    // the filtered arguments
    size_t     fargc;
    const char **fargv;
    // The String Buffer.
    char       *buffer;
    size_t     buflen;
    size_t     charlen;
    // the output file
    FILE       *fout;
    // Information on the Objective-C runtime flags.
    regex_t    objcrtflagregex;
    regmatch_t objcrtmatches[RTPARAMREGEXGROUPS];
    int        objcrtmajor;
    int        objcrtminor;
    int        objcrthotfx;
    // The index of the previous Objective-C runtime flag or 0.
    size_t     objcrtindex;
}    Context;

#ifdef __APPLE__

/****************************************************************************************************************************************************//**
 * Implements the the <a href="https://www.gnu.org/software/libc/manual/html_node/Error-Messages.html#index-error">error(int, int, const char *, ...)</a>
 * function found in GCC.
 *
 * @param status The status. If this is non-zero then <code>exit(int)</code> is called with this value at the end of the function.
 * @param errnum The error number.
 * @param format The format for the user defined error message.
 * @see <a href="https://www.gnu.org/software/libc/manual/html_node/Error-Messages.html#index-error">error(int, int, const char *, ...)</a>
 * @see <a href="https://www.gnu.org/software/libc/manual/html_node/Normal-Termination.html#index-exit">exit(int)</a>
 */
void error(int status, int errnum, const char *format, ...) {
    va_list list;
    if(errnum) {
        va_start(list, format);
        fprintf(stderr, "%s: ", getprogname());
        vfprintf(stderr, format, list);
        fprintf(stderr, ": %s\n", strerror(errnum));
        va_end(list);
    }
    if(status) exit(status);
}

#endif

/****************************************************************************************************************************************************//**
 * Creates a copy of the command-line arguments.
 *
 * @param ctx the work context.
 * @param argc the number of command-line arguments.
 * @param argv the command-line arguments.
 */
void copyCommandLineToContext(Context *ctx, int argc, const char *argv[]) {
    ctx->argc  = argc;
    ctx->argv  = malloc(argc * sizeof(char *));
    ctx->fargv = malloc(argc * sizeof(char *));

    if(ctx->argv && ctx->fargv) {
        ctx->fargc = 1;
        ctx->fargv[0] = ctx->argv[0] = argv[0];

        for(int i = 1; i < argc; ++i) {
            ctx->argv[i] = argv[i];
        }
    }
    else {
        error(ENOMEM, ENOMEM, "%s", NOT_ENOUGH_MEMORY);
    }
}

/****************************************************************************************************************************************************//**
 * Checks to make sure the work buffer is big enough to hold an additional number of characters.  If not then the work buffer is expanded by doubling
 * it's size until it is big enough.
 *
 * @param ctx the work context.
 * @param count the number of additional character that need to be added to the work buffer.
 */
void checkBufferSpace(Context *ctx, size_t count) {
    while((ctx->charlen + count) > ctx->buflen) {
        size_t nbsz = (ctx->buflen * 2);
        char   *nb  = realloc(ctx->buffer, nbsz);

        if(!nb) {
            error(ENOMEM, ENOMEM, "%s", NOT_ENOUGH_MEMORY);
        }
        else {
            ctx->buflen = nbsz;
            ctx->buffer = nb;
        }
    }
}

/****************************************************************************************************************************************************//**
 * Add a single character to the work buffer.
 *
 * @param ctx the work context.
 * @param ch the character.
 */
void addCharacterToBuffer(Context *ctx, char ch) {
    checkBufferSpace(ctx, 1);
    ctx->buffer[ctx->charlen++] = ch;
    ctx->buffer[ctx->charlen]   = 0;
}

/****************************************************************************************************************************************************//**
 * Add the given command-line argument to the work buffer.  Characters will be escaped if needed - see <code>XLAT[]</code> above.
 *
 * @param ctx the work context.
 * @param a the command-line argument.
 */
void addArgumentToBuffer(Context *ctx, const char *a) {
    int j  = 0;
    int ch = (int)a[j++];

    while(ch) {
        const char *xlt = XLAT[ch + 128]; // We add 128 to offset negative values.

        if(xlt) {
            size_t xltlen = strlen(xlt);
            checkBufferSpace(ctx, xltlen);
            strcpy((ctx->buffer + ctx->charlen), xlt);
            ctx->charlen += xltlen;
        }
        else {
            addCharacterToBuffer(ctx, (char)ch);
        }

        ch = (int)a[j++];
    }
}

/****************************************************************************************************************************************************//**
 * Checks to see if the given argument has been given already or not.
 *
 * @param ctx the work context.
 * @param arg the argument.
 * @return Zero if the argument has already been given. Non-zero if it's a new, never before seen argument.
 */
int isArgumentNew(Context *ctx, const char *arg) {
    for(int i = 0; i < ctx->fargc; ++i) {
        if(strcmp(arg, ctx->fargv[i]) == 0) return 0;
    }
    return 1;
}

/****************************************************************************************************************************************************//**
 * Returns a non-zero value if the given Objective-C runtime flag argument has a higher version number than the previous one.
 *
 * @param ctx the work context.
 * @param major the new RT flag's major version number.
 * @param minor the new RT flag's minor version number.
 * @param hotfx the new RT flag's hotfx version number.
 * @return non-zero if the new runtime flag has a higher version number.
 */
int isHigherRuntime(const Context *ctx, int major, int minor, int hotfx) {
    if(ctx->objcrtindex) {
        int cc = (major - ctx->objcrtmajor);

        if(cc > 0) return 1;
        if(cc < 0) return 0;

        cc = (minor - ctx->objcrtminor);

        if(cc > 0) return 1;
        if(cc < 0) return 0;

        cc = (hotfx - ctx->objcrthotfx);

        return ((cc > 0) ? 1 : 0);
    }

    return 1;
}

/****************************************************************************************************************************************************//**
 * Inserts an argument into the output argument list.
 *
 * @param ctx the work context.
 * @param a the argument string.
 * @return the index in the output argument list where it was inserted.
 */
size_t insertArgument(Context *ctx, const char *a) {
    ctx->fargv[ctx->fargc] = a;
    return (ctx->fargc++);
}

/****************************************************************************************************************************************************//**
 * Handle a regex error.
 *
 * @param ctx the work context.
 * @param regexResp the response from one of the regex functions.
 */
void handleRegexError(Context *ctx, int regexResp) {// Error Happened.
    size_t neededLen = regerror(regexResp, &(ctx->objcrtflagregex), NULL, 0);
    char   *errorMsg = malloc(neededLen);
    regerror(regexResp, &(ctx->objcrtflagregex), errorMsg, neededLen);
    error(1, EINVAL, "%s", errorMsg);
}

/****************************************************************************************************************************************************//**
 * Returns the larger of two regoff_t values.
 *
 * @param a value one.
 * @param b value two.
 * @return the larger of values one and two.
 */
regoff_t max(regoff_t a, regoff_t b) {
    return ((a < b) ? b : a);
}

/****************************************************************************************************************************************************//**
 * Returns the integer value of the version component specified by the regex capturing group.
 *
 * @param arg the command-line argument.
 * @param subex the regex capturing group.
 * @return the integer value or zero (0) if missing or invalid.
 */
int getRegexArg(const char *arg, const regmatch_t *subex) {
    if(subex && (subex->rm_so >= 0) && (subex->rm_eo > subex->rm_so)) {
        size_t l = (size_t)max(((subex->rm_eo - subex->rm_so) + 1), 11);
        strncpy(TEMP, (arg + (size_t)(subex->rm_so)), l);
        TEMP[l] = 0;
        return atoi(TEMP);
    }

    return 0;
}

/****************************************************************************************************************************************************//**
 * Process the GNUstep Objective-C runtime library argument.
 *
 * @param ctx the work context.
 * @param arg the command-line argument.
 * @param major the major version number.
 * @param minor the minor version number.
 * @param hotfx the hotfix version number.
 */
void processRTFlag(Context *ctx, const char *arg, int major, int minor, int hotfx) {
#if _DOTEST_ != 0
    fprintf(stderr, "Version: %d.%d.%d\n", major, minor, hotfx);
#endif
    if(isHigherRuntime(ctx, major, minor, hotfx)) {
        if(ctx->objcrtindex) {
            const char **p = &(ctx->fargv[ctx->objcrtindex]);
            memmove(p, (p + 1), ((--ctx->fargc) - ctx->objcrtindex));
        }

        ctx->objcrtindex = insertArgument(ctx, arg);
        ctx->objcrtmajor = major;
        ctx->objcrtminor = minor;
        ctx->objcrthotfx = hotfx;
    }
}

/****************************************************************************************************************************************************//**
 * Process the command-line argument.
 *
 * @param ctx the work context.
 * @param arg the command-line argument.
 */
void processArgument(Context *ctx, const char *arg) {
    if(isArgumentNew(ctx, arg)) {
        int regexResp = regexec(&(ctx->objcrtflagregex), arg, RTPARAMREGEXGROUPS, ctx->objcrtmatches, 0);

        if(regexResp == REG_NOMATCH) {
            insertArgument(ctx, arg);
        }
        else if(regexResp == 0) {
            processRTFlag(ctx,
                          arg,
                          getRegexArg(arg, &(ctx->objcrtmatches[MAJOR_GROUP_IDX])),
                          getRegexArg(arg, &(ctx->objcrtmatches[MINOR_GROUP_IDX])),
                          getRegexArg(arg, &(ctx->objcrtmatches[HOTFX_GROUP_IDX])));
        }
        else {
            handleRegexError(ctx, regexResp);
        }
    }
}

/****************************************************************************************************************************************************//**
 * Print the output buffer.
 *
 * @param ctx the work context.
 */
void printBuffer(Context *ctx) {
    addCharacterToBuffer(ctx, '\n');
    fputs(ctx->buffer, ctx->fout);
}

/****************************************************************************************************************************************************//**
 * Build the output buffer from the output command-line argument list.
 *
 * @param ctx the work context.
 */
void buildOutput(Context *ctx) {
    if(ctx->fargc > 1) {
        int j = 1;
        addArgumentToBuffer(ctx, ctx->fargv[j++]);
        while(j < ctx->fargc) {
            addCharacterToBuffer(ctx, ' ');
            addArgumentToBuffer(ctx, ctx->fargv[j++]);
        }
    }
    else {
        ctx->buffer[0] = '\n';
        ctx->buffer[1] = 0;
    }
}

/****************************************************************************************************************************************************//**
 * Parse the command-line arguments.
 *
 * @param ctx the work context.
 */
void parseCommandLine(Context *ctx) {
    if(ctx->argc == 2) {
        insertArgument(ctx, ctx->argv[1]);
    }
    else if(ctx->argc > 2) {
        int regexResp = regcomp(&ctx->objcrtflagregex, RTPARAMREGEX, REG_EXTENDED);

        if(regexResp) {
            handleRegexError(ctx, regexResp);
        }
        else {
            for(int i = 1; i < ctx->argc; ++i) {
                processArgument(ctx, ctx->argv[i]);
            }

            regfree(&ctx->objcrtflagregex);
        }
    }
}

/****************************************************************************************************************************************************//**
 * Run the program.
 *
 * @param argc Number of command-line arguments.
 * @param argv the command-line arguments.
 * @return the exit code.
 */
int main(int argc, const char *argv[]) {
    if(argc < 1) error(1, EINVAL, "%s", "Invalid command-line.");

    Context *ctx = calloc(1, sizeof(Context));
    ctx->buflen = INITIAL_WORK_BUFFER_SIZE;
    ctx->buffer = calloc(1, (INITIAL_WORK_BUFFER_SIZE * sizeof(char)));
    ctx->fout   = stdout;

    if(!ctx->buffer) {
        error(ENOMEM, ENOMEM, "%s", NOT_ENOUGH_MEMORY);
    }
    else {
#if _DOTEST_ != 0
        const char *testArgv[] = {
                "",
                "-fobjc-runtime=gnustep-1.2.3",
                "Galen😇",
                "Rhodes\nwas here.",
                "-fobjc-runtime=gnustep-1.3.2-Beta",
                "-fobjc-runtime=gnustep-1.3.0",
                "-fobjc-runtime=gnustep-2.0",
                "I wanna be a \"cowboy\"",
                "-fobjc-runtime=gnustep-1.2.0"
        };
        copyCommandLineToContext(ctx, (sizeof(testArgv) / sizeof(testArgv[0])), testArgv);
#else
        copyCommandLineToContext(ctx, argc, argv);
#endif

        parseCommandLine(ctx);
        buildOutput(ctx);
        printBuffer(ctx);

        free(ctx->argv);
        free(ctx->fargv);
        free(ctx->buffer);
    }

    return 0;
}
