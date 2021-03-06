#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>

#define align4(x) (((((x)-1)>>2)<<2)+4)
#define BLOCK_SIZE sizeof(struct s_block)


typedef struct s_block *t_block;
struct s_block {
	size_t size;
	struct s_block *next;
	struct s_block *prev;
	int free;
	void *ptr;
	char data[1];
};

void *base=NULL;



 /** 
  *Esta funcion busca el siguiente bloque libre 
 */
t_block find_block(t_block *last , size_t size){
	t_block b=base;
	while (b && !(b->free && b->size >= size)) {
		*last = b;
		b = b->next;
	}
	return (b);
}

 /** 
  *Esta funcion extiende el tamño del monton actual 
 */
t_block extend_heap(t_block last , size_t s){
	int sb;
	t_block b;
	b = sbrk (0);
	sb = (int)sbrk(BLOCK_SIZE + s);
	if (sb < 0)
	{
		return (NULL);
	}b->size = s;
	b->next = NULL;
	b->prev = last;
	if (last)
	last ->next = b;
	b->free = 0;
	return (b);


}

/** 
  *Esta funcion corta el bloque para que el bloque de datos quede del tamaño deseado
 */

void split_block(t_block b, size_t s){
	t_block new;
	new = (t_block)(b->data + s);
	new->size = b->size - s - BLOCK_SIZE;
	new->next = b->next;
	new->prev = b;
	new->free = 1;
	new->ptr = new->data;
	b->size = s;
	b->next = new;
	if (new->next)
	new->next ->prev = new;
}

/** 
  *Esta funcion une un bloque con su siguiente 
 */

t_block fusion(t_block b){
	if (b->next && b->next ->free){
		b->size += BLOCK_SIZE + b->next ->size;
		b->next = b->next ->next;
		if (b->next)
		b->next ->prev = b;
	}
	return (b);
}



t_block get_block(void *p)
{
	char *tmp;
	tmp = p;
	return (p = tmp -= BLOCK_SIZE);
}


int valid_addr(void *p)
{
	 
	if (base)
	{
		if ( p>base && p<sbrk (0))
		{
			return (p == (get_block(p))->ptr);
		}
	}
	return (0);
}

/** 
  *Esta funcion alloca la cantidad de memoria necesaria para el arreglo que recibe como parametro
 */
void *calloc(size_t number , size_t size){
	size_t *new;
	size_t s4,i;
	new = malloc(number * size);
	if (new) {
		s4 = align4(number * size) << 2;
		for (i=0; i<s4 ; i++)
		new[i] = 0;
	}
	return (new);
}
/** 
  *Esta funcion alloca la cantidad de memoria que recibe como parametro
 */

void *malloc(size_t size){
	t_block b,last;
	size_t s;
	s = align4(size);
	if (base) {
		last = base;
		b = find_block(&last ,s);

		if (b) {
			/* can we split */
			if ((b->size - s) >= (BLOCK_SIZE + 4))
			{
				split_block(b,s);
			}
			b->free=1;  //0 o 1??
		} else {
			b = extend_heap(last ,s);
			if (!b)
			{
				return(NULL);
			}

		}
	} else {
		b = extend_heap(NULL ,s);
		if (!b)
		return(NULL);
		base = b;
	}
	return(b->data);
}




/** 
  *Esta funcion libera la memoria allocada
 */
void free(void *p)
{
	t_block b;
	if (valid_addr(p))
	{
		
		b = get_block(p);
		b->free = 1;
		/* fusion with previous if possible */
		if(b->prev && b->prev ->free)
		b = fusion(b->prev);
		/* then fusion with next */
		if (b->next)
		fusion(b);
		else
		{
			/* free the end of the heap */
			if (b->prev)
			b->prev ->next = NULL;
			else
			/* NO HAY MAS BLOQUES  !*/
			{
				base = NULL;
			}
			brk(b);
		}
	}
}

 /** 
  *Esta funcion copia los datos desde un bloque a otro
 */
void copy_block(t_block src, t_block dst)
{
	int *sdata ,*ddata;
	size_t i;
	sdata = src->ptr;
	ddata = dst->ptr;
	for (i=0; i*4<src->size && i*4<dst->size; i++)
	ddata[i] = sdata[i];
}