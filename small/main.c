
#include <stdio.h>
#include <stdlib.h>	// for exit

#include "proc.h"

#include "ring_buf.h"

static unsigned char producer_stack1[8 * 1024];
static unsigned char producer_stack2[8 * 1024];
static unsigned char consumer_stack[8 * 1024];
static unsigned char hello_stack[2 * 1024];

static Ringbuf_T ring_buf;

char *hello(void *priv)
{
	return "hello exit";
}

char *producer(void *priv)
{
	for (unsigned char i = 0; ; i++)
	{
		printf("%s running, i: %u\n", __func__, i);
		Ringbuf_put(ring_buf, &i, 1);
		//sleep(1);
		yield();
	}
	return "producer exit";
}

char *consumer(void *priv)
{
	unsigned char data = 0;
	for (unsigned char i = 0; ; i++)
	{
		while (Ringbuf_get(ring_buf, &data, 1) > 0)
		{
			printf("%s running, data: %u\n", __func__, data);
		}
		//sleep(1);
		yield();
	}
	return "consumer exit";
}

int main(int argc, char *argv[])
{
	init_os();

	_spawn("producer", producer, NULL, producer_stack1, 8 * 1024, 0);
	_spawn("producer", producer, NULL, producer_stack2, 8 * 1024, 0);
	_spawn("consumer", consumer, NULL, consumer_stack, 8 * 1024, 0);
	_spawn("hello", hello, NULL, hello_stack, 2 * 1024, 0);

	printf("call first process, proc_t: %d, ucontext_t: %d\n", (int)sizeof(struct proc_t), (int)sizeof(ucontext_t));
	sleep(3);

	ring_buf = Ringbuf_create(1024);

	start_os();

	exit(0);
}
