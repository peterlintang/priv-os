
/*
 *
 * 实现环形缓冲区
 * 在num数组中，当write index + 1 == read index时，
 * 数组满， 此时数组有一个空元素，所以真正的大小式num-1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "ring_buf.h"


#define T Ringbuf_T

struct T {
	unsigned char *buf;
	unsigned int buf_max;
	unsigned int read_index;
	unsigned int write_index;
};



static int ring_buf_print(T r, int len, char *s)
{
#if TEST_RING
	fprintf(stdout, "buf: %p, max: %d, read: %d, write: %d, %s, len: %d\n",
			r->buf, r->buf_max, r->read_index, r->write_index,
			s, len);
#endif
	return 0;
}

/*
 * 以ptr为存储空间初始化一个ringbuf，ptr长度为len
 * 成功返回一个ringbuf的指针,失败null
 * ptr: ringbuf的空间，
 * len: ptr的长度
 */
T Ringbuf_init(unsigned char *ptr, unsigned int len)
{
	assert(ptr && len > 0);

	T r = NULL;

	r = (T)ptr;
	r->buf_max = len - sizeof(*r);
	r->read_index = 0;
	r->write_index = 0;
	r->buf = ptr + sizeof(*r);

	return r;
}

T Ringbuf_create(unsigned int num)
{
	assert(num > 0);

	T r = NULL;

	r = (T)calloc(1, sizeof(*r) + num);
	if (r)
	{
		r->buf_max = num;
		r->read_index = 0;
		r->write_index = 0;
		r->buf = ((unsigned char *)r) + sizeof(*r);
	}

	return r;
}

int Ringbuf_destroy(T *pr)
{
	assert(pr && *pr);

	free(*pr);
	*pr = NULL;
	return 0;
}


int Ringbuf_is_empty(T r)
{
	assert(r);

	if (r->read_index == r->write_index)
		return 1;
	return 0;
}

int Ringbuf_is_full(T r)
{
	assert(r);

	if (((r->write_index + 1) % r->buf_max) == r->read_index)
		return 1;
	return 0;
}

int Ringbuf_left_space(T r)
{
	assert(r);

	if (r->write_index >= r->read_index)
		return (r->buf_max - (r->write_index - r->read_index) - 1);
	else
		return r->read_index - r->write_index - 1;
}

int Ringbuf_available_data(T r)
{
	assert(r);

	if (r->write_index >= r->read_index)
	{
		return r->write_index - r->read_index;
	}
	else
	{
		return r->buf_max - (r->read_index - r->write_index);
	}
}

int Ringbuf_put(T r, unsigned char *data, unsigned int len)
{
	int ret = 0;
	int left = 0;

	assert(r != NULL);
	assert(data != NULL);
	assert(len > 0);

	if (Ringbuf_is_full(r))
	{
		return -1;
	}

	ring_buf_print(r, len, "put b");

	left = Ringbuf_left_space(r);
	len = (left >= len ? len : left);
	ret = len;

	if (r->write_index + len > r->buf_max)
	{
		memcpy(r->buf + r->write_index, data, r->buf_max - r->write_index);
		len = len - (r->buf_max - r->write_index);

		memcpy(r->buf, data + (r->buf_max - r->write_index), len);
		r->write_index = len;
	}
	else
	{
		memcpy(r->buf + r->write_index, data, len);
		r->write_index = (r->write_index + len) % r->buf_max;
	}

	ring_buf_print(r, len, "put a");

	return ret;
}

int Ringbuf_get(T r, unsigned char *data, unsigned int len)
{
	int ret = 0;
	int available = 0;

	assert(r != NULL);
	assert(data != NULL);
	assert(len > 0);

	if (Ringbuf_is_empty(r))
	{
		return -1;
	}

	ring_buf_print(r, len, "get b");

	available = Ringbuf_available_data(r);
	len = available >= len ? len : available;
	ret = len;

	if (r->read_index + len > r->buf_max)
	{
		memcpy(data, r->buf + r->read_index, r->buf_max - r->read_index);
		len = len - (r->buf_max - r->read_index);

		memcpy(data + r->buf_max - r->read_index, r->buf, len);
		r->read_index = len;
	}
	else
	{
		memcpy(data, r->buf + r->read_index, len);
		r->read_index = (r->read_index + len) % r->buf_max;
	}

	ring_buf_print(r, len, "get a");

	return ret;
}

