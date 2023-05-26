#ifndef QUEUE_QUEUE_H_
#define QUEUE_QUEUE_H_
#include ".\app_cfg.h"
#if USE_SERVICE_QUEUE == ENABLED
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
/* example:

    ENQUEUE(&my_queue,data1);//根据变量的类型，自动计算对象的大小
    ENQUEUE(&my_queue,data2);
    ENQUEUE(&my_queue,data3);

    // 一下三种方式都可以正确存储数组
    ENQUEUE(&my_queue,data4,2);//可以不指名数据类型
    ENQUEUE(&my_queue,data4,uint16_t,2);//也可以指名数据类型
    ENQUEUE(&my_queue,data4,uint8_t,sizeof(data4));//或者用其他类型

    //一下两种方式都可以正确存储结构体类型
    ENQUEUE(&my_queue,data5);//根据结构体的类型，自动计算对象的大小
    ENQUEUE(&my_queue,&data5,uint8_t,sizeof(data5));//也可以以数组方式存储


    // 读出全部数据
    DEQUEUE(&my_queue,data,GET_QUEUE_COUNT(&my_queue));
  //DEQUEUE(&my_queue,data,uint8_t,GET_QUEUE_COUNT(&my_queue))

*/

#define __PLOOC_CLASS_USE_STRICT_TEMPLATE__

#if     defined(__BYTE_QUEUE_CLASS_IMPLEMENT__)
    #define __PLOOC_CLASS_IMPLEMENT__
#elif   defined(__BYTE_QUEUE_CLASS_INHERIT__)
    #define __PLOOC_CLASS_INHERIT__
#endif

#include "plooc_class.h"


#ifndef __protect_queue__
    #include "perf_counter.h"
    #define __protect_queue__  __IRQ_SAFE
#endif

#define __DEQUEUE_0( __QUEUE, __ADDR)                                \
    dequeue_bytes((__QUEUE), (__ADDR),(sizeof(typeof(*(__ADDR)))))

#define __DEQUEUE_1( __QUEUE, __ADDR, __ITEM_COUNT)                        \
    dequeue_bytes((__QUEUE), (__ADDR), __ITEM_COUNT*(sizeof(typeof((__ADDR[0])))))

#define __DEQUEUE_2( __QUEUE, __ADDR, __TYPE,__ITEM_COUNT)                 \
    dequeue_bytes((__QUEUE), (__ADDR), (__ITEM_COUNT * sizeof(__TYPE)))


#define __ENQUEUE_0( __QUEUE, __VALUE)                                     \
    ({typeof((__VALUE)) SAFE_NAME(value) = __VALUE;                     \
        enqueue_bytes((__QUEUE), &(SAFE_NAME(value)), (sizeof(__VALUE)));})

#define __ENQUEUE_1( __QUEUE, __ADDR, __ITEM_COUNT)                         \
    enqueue_bytes((__QUEUE), (__ADDR), __ITEM_COUNT*(sizeof(typeof((__ADDR[0])))))

#define __ENQUEUE_2( __QUEUE, __ADDR, __TYPE, __ITEM_COUNT)                 \
    enqueue_bytes((__QUEUE), (__ADDR), (__ITEM_COUNT * sizeof(__TYPE)))


#define __PEEK_QUEUE_0( __QUEUE, __ADDR)                               \
    peek_bytes_queue((__QUEUE), (__ADDR), (sizeof(typeof(*(__ADDR)))))

#define __PEEK_QUEUE_1( __QUEUE, __ADDR, __ITEM_COUNT)                       \
    peek_bytes_queue((__QUEUE), (__ADDR), __ITEM_COUNT*(sizeof(typeof((__ADDR[0])))))

#define __PEEK_QUEUE_2( __QUEUE, __ADDR, __TYPE, __ITEM_COUNT)               \
    peek_bytes_queue((__QUEUE), (__ADDR), (__ITEM_COUNT * sizeof(__TYPE)))


/*!
 * \brief Initialize the queue object.
 *
 * \param[in] __queue pointer to the queue object.
 * \param[in] __buffer address of ring buffer var
 * \param[in] __size size of the ring buffer in bytes.
 *
 * \return the address of queue item
 *
 * \details Here is an example:
    E.g.
    \code
        static uint8_t s_hwQueueBuffer[100];
        static byte_queue_t my_queue;
        queue_init(&my_queue,s_hwQueueBuffer,sizeof(s_hwQueueBuffer));
    \endcode
 */

#define queue_init(__queue,__buffer,__size)                 \
    queue_init_byte(__queue, __buffer, __size )

#define QUEUE_INIT(__QUEUE, __BUFFER, __SIZE )               \
    queue_init_byte(__QUEUE, __BUFFER, __SIZE )

/*!
 * \brief Get data from the ring buffer.
 *
 * \param[in] __queue pointer to the queue object.
 * \param[in] __addr address to the data buffer
 * \param[in] ... Optional parameters,You can add data types and data quantities
 *
 * \return Return the data size we read from the ring buffer.
 *
 * \details Here is an example:
    E.g. The size is automatically calculated based on the variable type
    \code
        uint8_t  data1;
        uint16_t data2;
        uint32_t data3;
        dequeue(&my_queue,&data1);
        dequeue(&my_queue,&data2);
        dequeue(&my_queue,&data3);
    \endcode
    E.g. Read out all data of ring buffer
    \code
         uint8_t  data[100];
         dequeue(&my_queue,data,get_queue_count(&my_queue));
         dequeue(&my_queue,data,uint8_t,get_queue_count(&my_queue));
    \endcode
 */
#define dequeue(__queue,__addr,...)                       \
    __PLOOC_EVAL(__DEQUEUE_,##__VA_ARGS__)        \
    (__queue,(__addr),##__VA_ARGS__)

#define DEQUEUE(__QUEUE, __ADDR,...)                                            \
    __PLOOC_EVAL(__DEQUEUE_,##__VA_ARGS__)                              \
    (__QUEUE,(__ADDR),##__VA_ARGS__)
/*!
 * \brief Put a block of data into the ring buffer. If the capacity of ring buffer is insufficient, it will discard out-of-range data.
 *
 * \param[in] __queue pointer to the queue object.
 * \param[in] __addr address to the data buffer
 * \param[in] ... Optional parameters,You can add data types and data quantities
 *
 * \return Return the data size we put into the ring buffer.
 *
 * \details Here is an example:
    E.g. The size is automatically calculated based on the variable type
    \code
        uint8_t  data1 = 0XAA;
        uint16_t data2 = 0X55AA;
        uint32_t data3 = 0X55AAAA55;
        enqueue(&my_queue,data1);
        enqueue(&my_queue,data2);
        enqueue(&my_queue,data3);
    \endcode
    E.g. Put a block of data into the ring buffer
    \code
         uint32_t  data[100];
         enqueue(&my_queue,data,100);
         enqueue(&my_queue,data,uint32_t,100);
    \endcode
 */

#define enqueue(__queue, __addr,...)                    \
    __PLOOC_EVAL(__ENQUEUE_,##__VA_ARGS__)      \
    (__queue,(__addr),##__VA_ARGS__)

#define ENQUEUE(__QUEUE, __ADDR,...)                    \
    __PLOOC_EVAL(__ENQUEUE_,##__VA_ARGS__)      \
    (__QUEUE,(__ADDR),##__VA_ARGS__)


/*!
 * \brief Peek data from the ring buffer.
 *
 * \param[in] __queue pointer to the queue object.
 * \param[in] __addr address to the data buffer
 * \param[in] ... Optional parameters,You can add data types and data quantities
 *
 * \return Return the data size we peek from the ring buffer.
 *
 * \details Here is an example:
    E.g. The size is automatically calculated based on the variable type
    \code
        uint8_t  data1;
        uint16_t data2;
        uint32_t data3;
        peek_queue(&my_queue,&data1);
        peek_queue(&my_queue,&data2);
        peek_queue(&my_queue,&data3);
    \endcode
 */

#define peek_queue(__queue, __addr,...)                       \
    __PLOOC_EVAL(__PEEK_QUEUE_,##__VA_ARGS__)          \
    (__queue,(__addr),##__VA_ARGS__)

#define PEEK_QUEUE(__QUEUE, __ADDR,...)                         \
    __PLOOC_EVAL(__PEEK_QUEUE_,##__VA_ARGS__)            \
    (__QUEUE,(__ADDR),##__VA_ARGS__)



declare_class(byte_queue_t)
def_class(byte_queue_t,
          private_member(
              uint8_t *pchBuffer;
              uint16_t hwSize;
              uint16_t hwHead;
              uint16_t hwTail;
              uint16_t hwLength;
              uint16_t hwPeek;
              uint16_t hwPeekLength;
          )
         )
end_def_class(byte_queue_t)

extern
byte_queue_t * queue_init_byte(byte_queue_t *ptObj, void *pBuffer, uint16_t hwItemSize);

extern
bool reset_queue(byte_queue_t *ptObj);

extern
bool enqueue_byte(byte_queue_t *ptQueue, uint8_t chByte);

extern
int16_t enqueue_bytes(byte_queue_t *ptObj, void *pDate, uint16_t hwLength);

extern
bool dequeue_byte(byte_queue_t *ptQueue, uint8_t *pchByte);

extern
int16_t dequeue_bytes(byte_queue_t *ptObj, void *pDate, uint16_t hwLength);

extern
bool is_queue_empty(byte_queue_t *ptQueue);

extern
bool peek_byte_queue(byte_queue_t *ptQueue, uint8_t *pchByte);

extern
int16_t peek_bytes_queue(byte_queue_t *ptObj, void *pDate, uint16_t hwLength);

extern
bool reset_peek(byte_queue_t *ptQueue);

extern
bool get_all_peeked(byte_queue_t *ptQueue);

extern
uint16_t get_peek_status(byte_queue_t *ptQueue);

extern
bool restore_peek_status(byte_queue_t *ptQueue, uint16_t hwCount);

extern
int16_t get_queue_count(byte_queue_t *ptObj);

extern
int16_t get_queue_available_count(byte_queue_t *ptObj);

#undef __BYTE_QUEUE_CLASS_INHERIT__
#undef __BYTE_QUEUE_CLASS_IMPLEMENT__

#endif
#endif /* QUEUE_QUEUE_H_ */
