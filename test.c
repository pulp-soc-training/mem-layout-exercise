/* 
 * Copyright (C) 2017 ETH Zurich, University of Bologna and GreenWaves Technologies
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 * Authors: Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)
 */

#include "pmsis.h"


#define NB_PRIV_DATA 16

/* Statically alloc an array in the new section */
PI_L2_PRIV2_DATA int my_static_l2_priv2_data[NB_PRIV_DATA] = {0};

static int test_entry()
{
    /* Dynamically alloc an array in the new section */
    int * my_dynamic_l2_priv2_data = pi_l2_priv2_malloc(NB_PRIV_DATA * sizeof(int));

    /* Is it in L2? */
    int * pointer_to_bigger_shared_l2_addr = (int *)0x1c090000;

    *(int *)pointer_to_bigger_shared_l2_addr = 0x12345678;
    for(int i=0; i<NB_PRIV_DATA; i++)
    {
      my_static_l2_priv2_data[i]  = i;
      my_dynamic_l2_priv2_data[i] = NB_PRIV_DATA - i;
    }

    printf("Shared L2 access -> %x, %p\n", *(int *)pointer_to_bigger_shared_l2_addr, pointer_to_bigger_shared_l2_addr);

    for(int i=0; i<NB_PRIV_DATA; i++)
    {
      printf("[%d]: PRIV2 static allocation: addr %p value %x, PRIV2 dynamic allocation: addr %p value %x\n", i, (my_static_l2_priv2_data + i), my_static_l2_priv2_data[i], (my_dynamic_l2_priv2_data + i), my_dynamic_l2_priv2_data[i]);
    }

    return 0;
}

static void test_kickoff(void *arg)
{
    int ret = test_entry();
    pmsis_exit(ret);
}

int main()
{
    return pmsis_kickoff((void *)test_kickoff);
}
