/* $Id: ex03.mc,v 2.1 2005/06/14 22:16:47 jls Exp $ */

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


void subr (int64_t I0[], int64_t Out0[], int64_t Out1[], int num, int64_t *t_first, int64_t *t_second, int mapnum) {
    OBM_BANK_A (AL, int64_t, MAX_OBM_SIZE)
    OBM_BANK_B (BL, int64_t, MAX_OBM_SIZE)

    int64_t ABR[2048];
    int64_t t0, t1;
    int i, idx;

    Stream_64 SA,SB,SC;




#pragma src parallel sections
{
#pragma src section
{
    streamed_dma_cpu_64 (&SA, PORT_TO_STREAM, I0, num*sizeof(int64_t));
}
# pragma src section
{
    int i;
    int64_t i64;

    for (i=0;i<num;i++)  {
       get_stream_64 (&SA, &i64);
       ABR[i] = i64;
    }
}
}
    read_timer (&t0);

    // this is an update-in-place loop where the indices are sequential
    for (i=0; i<num; i++) {
	ABR[i] = ABR[i] + 42;
	}

    read_timer (&t1);

    *t_first = t1 - t0;

#pragma src parallel sections
{
#pragma src section
{
    streamed_dma_cpu_64 (&SB, STREAM_TO_PORT, Out0, num*sizeof(int64_t));
}
# pragma src section
{
    int i;
    int64_t i64;

    for (i=0;i<num;i++)  {
       i64 = ABR[i];
       put_stream_64 (&SB, i64, 1);
    }
}
}


    read_timer (&t0);

    // this is an update-in-place loop where the indices are not sequential
    // #pragma loop noloop_dep
    for (i=0; i<num; i++) {
	idx = i ^ 0xf;  // invert the low four bits of the index
	ABR[idx] = ABR[idx] + 42;
	}

    read_timer (&t1);

    *t_second = t1 - t0;

#pragma src parallel sections
{
#pragma src section
{
    streamed_dma_cpu_64 (&SC, STREAM_TO_PORT, Out1, num*sizeof(int64_t));
}
# pragma src section
{
    int i;
    int64_t i64;

    for (i=0;i<num;i++)  {
       i64 = ABR[i];
       put_stream_64 (&SC, i64, 1);
    }
}
}

}
