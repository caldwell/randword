/* $Header$
-------=====================<<<< COPYRIGHT >>>>========================-------
         Copyright (c) 1997-2002 Indigita Corp,  All Rights Reserved.
 See full text of copyright notice and limitations of use in file COPYRIGHT.h
-------================================================================-------
*/
#include "COPYRIGHT.h"

#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <time.h>

void Usage(char *me)
{
        fprintf(stderr,"usage: [dict-file] \n",me);
        fprintf(stderr,"The default filename is read from the environment variable \"words\"\nif no filename is specified.\n");
	exit(1);
}

void main(int c,char **v)
{
    int r;
    unsigned long offset;
    char *filename=0;
	FILE *f;

	struct stat s;

	/* parse command line */
	while (--c)	/* we must skip first argument (0) as that is OUR program name, so we pre-decrement c */
		if (v[c][0] == '-')
            switch(v[c][1])
            {
                default:
                    fprintf(stderr,"'%c' is an invalid option\n",v[c][1]);
                    Usage(v[0]);
                    break;
			}
		else /* no dash must mean its a filename */
			if (!filename)
				filename = v[c];
			else {
                fprintf(stderr,"Only one file may be specified; can't open \"%s\" AND \"%s\".\n",filename,v[c]);
				Usage(v[0]);
			}

	if (!filename) {
        filename = getenv("WORDS");
        if (!filename) {
            fprintf(stderr,"You must specify a file or set the environment variable \"WORDS\".\n");
            Usage(v[0]);
        }
	}

	r = open(filename, O_RDONLY
#ifdef MSDOS
             |O_BINARY
#endif
			 );
	if(r==-1){
		fprintf(stderr,"%s: can't open %s\n",v[0],filename);
		exit(1);
	}
	if(fstat(r,&s) == -1) {
		fprintf(stderr,"%s: why did stat fail?\n",v[0]);
		exit(1);
	}
    close(r);

    f = fopen(filename,"rb");
	if(f==0) {
        fprintf(stderr,"%s: can't open %s\n",v[0],filename);
		exit(1);
	}

    srand(time(&offset));
    offset = rand() % s.st_size;

    fseek(f,offset,SEEK_SET);

    while ((c = fgetc(f)) != '\n') /* skip to the end of current word */
        if (feof(f)) {/* if we hit eof, then use the first line of the file. */
                fseek(f,0,SEEK_SET);
                break;
        }

    while ((c = fgetc(f)) != '\n' && !feof(f)) /* print out new word */
        putchar(c);
    //        putchar('\n');

	fclose(f);
	exit (0);
}

/*
 * $Log$
 */
