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

#define RTPARAMPREFIX "-fobjc-runtime=gnustep-"

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

/***************************************************************************************************************************************************************************//**
 *
 */
static const char *XLAT[] = { //@f:0
    "$'\\200'", "$'\\201'", "$'\\202'", "$'\\203'", "$'\\204'", "$'\\205'", "$'\\206'", "$'\\207'", "$'\\210'", "$'\\211'", "$'\\212'", "$'\\213'", "$'\\214'", "$'\\215'", "$'\\216'", "$'\\217'", // 80 - 8f
    "$'\\220'", "$'\\221'", "$'\\222'", "$'\\223'", "$'\\224'", "$'\\225'", "$'\\226'", "$'\\227'", "$'\\230'", "$'\\231'", "$'\\232'", "$'\\233'", "$'\\234'", "$'\\235'", "$'\\236'", "$'\\237'", // 90 - 9f
    "$'\\240'", "$'\\241'", "$'\\242'", "$'\\243'", "$'\\244'", "$'\\245'", "$'\\246'", "$'\\247'", "$'\\250'", "$'\\251'", "$'\\252'", "$'\\253'", "$'\\254'", "$'\\255'", "$'\\256'", "$'\\257'", // a0 - af
    "$'\\260'", "$'\\261'", "$'\\262'", "$'\\263'", "$'\\264'", "$'\\265'", "$'\\266'", "$'\\267'", "$'\\270'", "$'\\271'", "$'\\272'", "$'\\273'", "$'\\274'", "$'\\275'", "$'\\276'", "$'\\277'", // b0 - bf
    "$'\\300'", "$'\\301'", "$'\\302'", "$'\\303'", "$'\\304'", "$'\\305'", "$'\\306'", "$'\\307'", "$'\\310'", "$'\\311'", "$'\\312'", "$'\\313'", "$'\\314'", "$'\\315'", "$'\\316'", "$'\\317'", // c0 - cf
    "$'\\320'", "$'\\321'", "$'\\322'", "$'\\323'", "$'\\324'", "$'\\325'", "$'\\326'", "$'\\327'", "$'\\330'", "$'\\331'", "$'\\332'", "$'\\333'", "$'\\334'", "$'\\335'", "$'\\336'", "$'\\337'", // d0 - df
    "$'\\340'", "$'\\341'", "$'\\342'", "$'\\343'", "$'\\344'", "$'\\345'", "$'\\346'", "$'\\347'", "$'\\350'", "$'\\351'", "$'\\352'", "$'\\353'", "$'\\354'", "$'\\355'", "$'\\356'", "$'\\357'", // e0 - ef
    "$'\\360'", "$'\\361'", "$'\\362'", "$'\\363'", "$'\\364'", "$'\\365'", "$'\\366'", "$'\\367'", "$'\\370'", "$'\\371'", "$'\\372'", "$'\\373'", "$'\\374'", "$'\\375'", "$'\\376'", "$'\\377'", // f0 - ff

    0,          "$'\\001'", "$'\\002'", "$'\\003'", "$'\\004'", "$'\\005'", "$'\\006'", "$'\\007'", "$'\\010'", "$'\\011'", "$'\\012'", "$'\\013'", "$'\\014'", "$'\\015'", "$'\\016'", "$'\\017'", // 00 - 0f
    "$'\\020'", "$'\\021'", "$'\\022'", "$'\\023'", "$'\\024'", "$'\\025'", "$'\\026'", "$'\\027'", "$'\\030'", "$'\\031'", "$'\\032'", "\\E",      "$'\\034'", "$'\\035'", "$'\\036'", "$'\\037'", // 10 - 1f
    "\\ ",      "\\!",      "\\\"",     "\\#",      "\\$",      0,          "\\&",      "\\'",      "\\(",      "\\)",      "\\*",      0,          "\\,",      0,          0,          0,          // 20 - 2f
    0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          "\\;",      "\\<",      0,          "\\>",      "\\?",      // 30 - 3f
    0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          // 40 - 4f
    0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          "\\[",      "\\\\",     "\\]",      "\\^",      0,          // 50 - 5f
    "\\`",      0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          // 60 - 6f
    0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          0,          "\\{",      "\\|",      "\\}",      "\\~",      "$'\\177'", // 70 - 7f
}; // @f:1

/***************************************************************************************************************************************************************************//**
 *
 */
typedef struct __argset__ {
    // the original arguments
    int        argc;
    const char **argv;
    // the filtered arguments
    size_t     fargc;
    const char **fargv;
    // the Objective-C runtime version parameter prefix
    const char *rtParamPfx;
    size_t     rtParamPfxLen;
    // the output file
    FILE       *fout;
}                 ArgSet;

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @param arg
 * @return
 */
int isArgumentNew(ArgSet *argset, const char *arg) {
    for(int i = 0; i < argset->fargc; ++i) {
        if(strcmp(arg, argset->fargv[i]) == 0) return 0;
    }
    return 1;
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @param a
 * @param idx
 * @return
 */
int isHigherRuntimeFlag(const ArgSet *argset, const char *a, int idx) {
    return (strcmp(argset->fargv[idx], a) < 0);
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @param a
 * @return
 */
int isNotRuntimeFlag(const ArgSet *argset, const char *a) {
    return (strncmp(argset->rtParamPfx, a, argset->rtParamPfxLen) != 0);
}

/***************************************************************************************************************************************************************************//**
 *
 * @param str
 * @param fout
 */
void putStr(const char *str, FILE *fout) {
    int j  = 0;
    int ch = (int)str[j++];

    while(ch) {
        const char *xlt = XLAT[ch + 128];
        if(xlt) fputs(xlt, fout);
        else {
            putc(ch, fout);
        }
        ch = (int)str[j++];
    }
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @param a
 * @return
 */
int insertArgument(ArgSet *argset, const char *a) {
    argset->fargv[argset->fargc] = a;
    return (int)(argset->fargc++);
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @param a
 * @param idx
 * @return
 */
int replaceArgument(ArgSet *argset, const char *a, int idx) {
    argset->fargv[idx] = a;
    return idx;
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @return
 */
int findRTFlag(ArgSet *argset) {
    for(int i = 0; i < argset->fargc; ++i) {
        if(!isNotRuntimeFlag(argset, argset->fargv[i])) return i;
    }
    return -1;
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @param a
 * @return
 */
int handleRTFlag(ArgSet *argset, const char *a) {
    int idx = findRTFlag(argset);
    return ((idx < 0) ? insertArgument(argset, a) : (isHigherRuntimeFlag(argset, a, idx) ? replaceArgument(argset, a, idx) : -1));
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @param a
 */
void filterArgument(ArgSet *argset, const char *a) {
    if(isArgumentNew(argset, a)) {
        (isNotRuntimeFlag(argset, a) ? insertArgument : handleRTFlag)(argset, a);
    }
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 */
void filterArguments(ArgSet *argset) {
    for(int i = 1; i < argset->argc;) filterArgument(argset, argset->argv[i++]);
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @param i
 */
void printArgument(ArgSet *argset, int i) {
    putc(' ', argset->fout);
    putStr(argset->fargv[i], argset->fout);
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 */
void printFilteredArguments(ArgSet *argset) {
    if(argset->fargc) {
        int        j    = 0;
        const char *str = argset->fargv[j++];
        putStr(str, argset->fout);
        while(j < argset->fargc) printArgument(argset, j++);
    }

    putc('\n', argset->fout);
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @return
 */
int processArguments(ArgSet *argset) {
    if(argset->fargv) {
        filterArguments(argset);
        printFilteredArguments(argset);
        free(argset->fargv);
        return 0;
    }

    puts("Not enough memory.");
    return 1;
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argset
 * @return
 */
int processArgument(ArgSet *argset) {
    if(argset->argc == 2) {
        putStr(argset->argv[1], argset->fout);
    }
    putc('\n', argset->fout);
    return 0;
}

/***************************************************************************************************************************************************************************//**
 *
 * @param fout
 * @param argc
 * @param argv
 * @return
 */
int cleanGNUstepConfigCommandLine(FILE *fout, int argc, const char *argv[]) {
    ArgSet argset = { /*@f:0*/
        .argc = argc,
        .argv = argv,
        .fargc = 0,
        .fargv = malloc(argc * sizeof(char *)),
        .rtParamPfx = RTPARAMPREFIX,
        .rtParamPfxLen = strlen(RTPARAMPREFIX),
        .fout = fout
    /*@f:1*/};

    return ((argset.argc <= 2) ? processArgument : processArguments)(&argset);
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argc
 * @param argv
 * @return
 */
int mainTest(int argc, const char *argv[]) {
    const char *ar[] = { "", "Galen😇", "Rhodes\nwas here." };
    return cleanGNUstepConfigCommandLine(stdout, (sizeof(ar) / sizeof(ar[0])), ar);
}

/***************************************************************************************************************************************************************************//**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char *argv[]) {
    return cleanGNUstepConfigCommandLine(stdout, argc, argv);
}
