static char const cvsid[] = "$Id: main.c,v 2.1 2005/06/14 22:16:47 jls Exp $";

/*
 * Copyright 2005 SRC Computers, Inc.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, Inc.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, Inc.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <libmap.h>
#include <stdlib.h>


void subr (int64_t I0[], int64_t Out0[], int64_t Out1[], int num, int64_t *t_first, int64_t *t_second, int mapnum);


int main (int argc, char *argv[]) {
    FILE *res_map, *res_cpu;
    int i, num;
    int64_t *A, *B, *C, *ResB, *ResC;
    int64_t tm0, tm1;
    int mapnum = 0;

    if ((res_map = fopen ("res_map", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_map'\n");
        exit (1);
        }

    if ((res_cpu = fopen ("res_cpu", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_cpu'\n");
        exit (1);
        }

    if (argc < 2) {
	fprintf (stderr, "need number of elements as arg\n");
	exit (1);
	}

    if (sscanf (argv[1], "%d", &num) < 1) {
	fprintf (stderr, "need number of elements as arg\n");
	exit (1);
	}

    if ((num < 16) || (num > 2048)) {
        fprintf (stderr, "number of elements must be in the range 16 through 2048\n");
	exit (1);
	}

    if ((num % 16) != 0) {
        fprintf (stderr, "number of elements must be a multiple of 16\n");
	exit (1);
	}

    A = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
    B = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
    C = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
    ResB = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
    ResC = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));

    srandom (99);

    for (i=0; i<num; i++) {
        A[i] = random ();
	}

    for (i=0; i<num; i++) {
        ResB[i] = A[i] + 42;
	ResC[i] = A[i] + 84;
	}

    map_allocate (1);

    subr (A, B, C, num, &tm0, &tm1, mapnum);

    printf ("%lld clocks , %lld clocks\n", tm0, tm1);

    for (i=0; i<num; i++) {
        fprintf (res_map, "%lld %lld\n", B[i], C[i]);
        fprintf (res_cpu, "%lld %lld\n", ResB[i], ResC[i]);
	}

    map_free (1);

    exit(0);
    }
