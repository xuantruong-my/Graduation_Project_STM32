#ifndef AFIO_H
#define AFIO_H

#include <stdint.h>

#define __VO		volatile

typedef struct {
	__VO uint32_t EVCR;
	__VO uint32_t MAPR;
	__VO uint32_t EXTICR1;
	__VO uint32_t EXTICR2;
	__VO uint32_t EXTICR3;
	__VO uint32_t EXTICR4;
	__VO uint32_t MAPR2;
} AFIO_TypeDef;

typedef enum {
	Port_A ,
	Port_B ,
	Port_C ,
} Port_Interrupt;

#define AFIO_ADD_BASE		0x40010000UL
#define AFIO				((AFIO_TypeDef*)(AFIO_ADD_BASE))

void SetLineInter(Port_Interrupt port, uint16_t Pin);

#endif
