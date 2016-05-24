#ifndef NETWORKCODINGHEADER_H_
#define NETWORKCODINGHEADER_H_

#include <stddef.h>

#define OUTER_HEADER_SIZE                                              (sizeof(OuterHeader))
#define GET_OUTER_SIZE(pkt)                                             (*(unsigned short int*)  ((unsigned char*)pkt + offsetof(OuterHeader, size)))
#define GET_OUTER_BLK_SEQ(pkt)                                     (*(unsigned short int*)  ((unsigned char*)pkt + offsetof(OuterHeader, blk_seq)))
#define GET_OUTER_BLK_SIZE(pkt)                                     (*(unsigned char*)			((unsigned char*)pkt + offsetof(OuterHeader, blk_size)))
#define GET_OUTER_FLAGS(pkt)                                         (*(unsigned char*)			((unsigned char*)pkt + offsetof(OuterHeader, flags)))
#define GET_INNER_SIZE(pkt)                                              (*(unsigned short int*)   ((unsigned char*)pkt + sizeof(OuterHeader) + offsetof(InnerHeader, size)))
#define GET_INNER_LAST_INDICATOR(pkt)                        (*(unsigned char*)          ((unsigned char*)pkt + sizeof(OuterHeader) + offsetof(InnerHeader, last_indicator)))
#define GET_INNER_CODE(pkt)                                           ((unsigned char*)pkt + sizeof(OuterHeader) + offsetof(InnerHeader, codes))
#define GET_INNER_PAYLOAD(pkt, max_block_size)         ((unsigned char*)pkt + sizeof(OuterHeader) + sizeof(InnerHeader) + (max_block_size-1))

#define TOTAL_HEADER_SIZE(max_block_size)                  (sizeof(OuterHeader) + sizeof(InnerHeader) + (max_block_size-1))
#define MAX_PAYLOAD_SIZE(max_block_size)                  (1500/*ETHERNET MTU*/ - 20/*IP*/ - 8/*UDP*/ - TOTAL_HEADER_SIZE(max_block_size))

struct OuterHeader
{
    unsigned short int size;          /*2*/
    unsigned short int blk_seq;    /*2*/
    unsigned char blk_size;          /*1*/
    unsigned char flags;               /*1*/
    enum : unsigned char
    {
        FLAGS_ORIGINAL = 0x1,
        FLAGS_END_OF_BLK = 0x2
    };
}__attribute__((packed));

#define PRINT_OUTERHEADER(pkt)\
do\
{\
    printf("OUTERHDR [Size = %6hu][Block Seq = %6hu][Block Size = %3hhu][Flags = %hhx]\n", \
    ((OuterHeader*)pkt)->size, ((OuterHeader*)pkt)->blk_seq, ((OuterHeader*)pkt)->blk_size, ((OuterHeader*)pkt)->flags);\
}while(0)

struct InnerHeader
{
    unsigned short int size;          /*2*/
	unsigned char last_indicator;	/*1*/
    unsigned char codes;             /*blk_size*/
}__attribute__((packed));

#define PRINT_INNERHEADER(max_block_size, pkt)\
do\
{\
    printf("INNERHDR [Size = %6hu][Last Indicator = %2hhu][Code =", ((InnerHeader*)(pkt+sizeof(OuterHeader)))->size, ((InnerHeader*)(pkt+sizeof(OuterHeader)))->last_indicator);\
    for(unsigned char _tmp = 0 ; _tmp < max_block_size ; _tmp++)\
    {\
        printf(" %hhu", (&((InnerHeader*)(pkt+sizeof(OuterHeader)))->codes)[_tmp]);\
    }\
    printf("]\n");\
}while(0)


#define PRINT_CODE_BLOCK(max_block_size, buf)\
do\
{\
    printf("Code===\n");\
    for(unsigned char _tmp2 = 0 ; _tmp2 < max_block_size ; _tmp2++)\
    {\
        for(unsigned char _tmp3 = 0 ; _tmp3 < max_block_size ; _tmp3++)\
        {\
        printf("%4hhu", GET_INNER_CODE(buf[_tmp2].buffer)[_tmp3]);\
        }\
        printf("\n");\
    }\
}while(0)

struct Ack
{
    unsigned short int blk_seq;    /*2*/
}__attribute__((packed));

#endif
