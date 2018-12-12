#include "seq_num_internal.h"

#define UINT16_SIZE (1 << 16)
#define UINT16_MIDSIZE (1 << 15)

//==================================================================
//                             PUBLIC
//==================================================================
uxrSeqNum uxr_seq_num_add(uxrSeqNum seq_num, uxrSeqNum increment)
{
    return (uxrSeqNum)((seq_num + increment) % UINT16_SIZE);
}

uxrSeqNum uxr_seq_num_sub(uxrSeqNum seq_num, uxrSeqNum decrement)
{
    return (uxrSeqNum)((decrement > seq_num)
        ? (seq_num + (UINT16_SIZE - decrement)) % UINT16_SIZE
        : seq_num - decrement);
}

int uxr_seq_num_cmp(uxrSeqNum seq_num_1, uxrSeqNum seq_num_2)
{
    int result;
    if(seq_num_1 == seq_num_2)
    {
        result = 0;
    }
    else if((seq_num_1 < seq_num_2 && (seq_num_2 - seq_num_1) < UINT16_MIDSIZE) ||
            (seq_num_1 > seq_num_2 && (seq_num_1 - seq_num_2) > UINT16_MIDSIZE))
    {
        result = -1;
    }
    else
    {
        result = 1;
    }
    return result;
}
