/********************************************************************
 *
 * File :        discard.c
 * Description : Discard enhancement bits.
 * Created :     24/Jul/96 12:54
 * Written by :  Maxim A. Lagerev
 *
 *******************************************************************/

#include <stdlib.h>
#include <stdio.h>


FILE *in, *out;

int getWord(int wordLen);
void putWord(unsigned char word, int wordLen);


static void
error(const char *errMsg)
{
    fputs(errMsg, stderr);
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int n;
    int ne;
    int word;

    if (argc < 5)
        error("Usage: discard N Ne <infile> <outfile>\n");

    n = atoi(argv[1]);
    ne = atoi(argv[2]);

    if (!(2 <= n && n <= 8 && 1 <= ne && ne < n))
        error("Incorrect parameters\n");

    if ((in = fopen(argv[3], "rb")) == NULL)
        error("Can't open input file\n");

    if ((out = fopen(argv[4], "wb")) == NULL)
        error("Can't open output file\n");

    while ((word = getWord(n)) != EOF)
        putWord((unsigned char)(word >> ne), n - ne);

    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}


int
getWord(int wordLen)
{
    int fieldLen = 8;
    static int firstFl = 1;
    static unsigned char currFieldi = 0;
    static int currPosi = 0;
    int oldPos = currPosi;
    unsigned char retw;
    int ch;

    if (firstFl) {
        int ch = fgetc(in);

        if (ch == EOF)
            return EOF;

        currFieldi = ch;
        firstFl = 0;
    }

    currPosi += wordLen;

    if (currPosi < fieldLen)
        return (unsigned char)((currFieldi >> oldPos) & ~(0xffff << wordLen));

    retw = currFieldi >> oldPos;
    currPosi -= fieldLen;
    ch = fgetc(in);

    if (ch == EOF) {
        firstFl = 1;
        return (currPosi == 0) ? retw : EOF;
    }

    currFieldi = ch;

    if (currPosi == 0)
        return retw;

    return (unsigned char)(((currFieldi << (fieldLen - oldPos)) | retw) &
           ~(0xffff << wordLen));
}


void
putWord(unsigned char word, int wordLen)
{
    int fieldLen = 8;
    static unsigned char currFieldo = 0;
    static int currPoso = 0;
    int oldPos = currPoso;

    if (wordLen == 0) {
        if (currPoso != 0)
            fputc(currFieldo, out);

        fflush(out);
        return;
    }

    currPoso += wordLen;
    currFieldo |= word << oldPos;

    if (currPoso < fieldLen)
        return;

    currPoso -= fieldLen;
    fputc(currFieldo, out);
    currFieldo = 0;

    if (currPoso == 0)
        return;

    currFieldo = word >> (fieldLen - oldPos);
}

