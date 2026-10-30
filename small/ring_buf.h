
/*
 *
 * 实现环形缓冲区
 * 在num数组中，当write index + 1 == read index时，
 * 数组满， 此时数组有一个空元素，所以真正的大小式num-1
 *
 */

#ifndef RING_BUF_H
#define RING_BUF_H


#define T Ringbuf_T
typedef struct T *T;


/*
 * 以ptr为存储空间初始化一个ringbuf，ptr长度为len
 * 成功返回一个ringbuf的指针,失败null
 * 调用此接口，不用调用destroy释放ringbuf内容,由提供ptr的作者释放
 * ptr: ringbuf的空间，
 * len: ptr的长度
 */
T Ringbuf_init(unsigned char *ptr, unsigned int len);

T Ringbuf_create(unsigned int num);

int Ringbuf_destroy(T *pr);

int Ringbuf_empty(T r);

int Ringbuf_full(T r);

int Ringbuf_left_space(T r);

int Ringbuf_available_data(T r);

int Ringbuf_put(T r, unsigned char *data, unsigned int len);

int Ringbuf_get(T r, unsigned char *data, unsigned int len);

#undef T

#endif
