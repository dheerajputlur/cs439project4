/*
 * kthread.h
 *
 *  Created on: Apr 23, 2015
 *      Author: mwkurian
 */
#ifndef KERNEL_INCLUDE_KTHREAD_H_
#define KERNEL_INCLUDE_KTHREAD_H_

#include "global_defs.h"
#include <stdint.h>
#include "vm.h"
typedef struct thread * thread_t;

typedef void (*kthread_callback_handler)();

typedef enum THREAD_STATE
{
	THREAD_NEW, THREAD_READY, THREAD_RUNNING, THREAD_BLOCKED, THREAD_DYING
} THREAD_STATE;

typedef struct kthread_handle {
    uint32_t parent_pid;
    int tid;
    // int niceness;
    // int state;
    kthread_callback_handler cb_handler;
    //added by the group
    	//cpu state
        THREAD_STATE current_state;
        //unbanked register
		uint32_t R0;
		uint32_t R1;
		uint32_t R2;
		uint32_t R3;
		uint32_t R4;
		uint32_t R5;
		uint32_t R6;
		uint32_t R7;

		//banked registers
		uint32_t R8;
		uint32_t R9;
		uint32_t R10;
		uint32_t R11;
		uint32_t R12;
		uint32_t R13; //corresponds to the SP; do we need both?
		uint32_t R14;
		uint32_t R15; //corresponds to the PC; do we need both?

		uint32_t SPSR;
		uint32_t PC;

		//same virtual address as process?
		struct vas* stored_vas;

} kthread_handle;

kthread_handle* kthread_create(thread_t *thread, void *(*func)(void*), void *args); 

uint32_t kthread_save_state(uint32_t tid);
// uint32_t kthread_load_state(AS_KTHREAD(active_task));

#endif /* KERNEL_INCLUDE_KTHREAD_H_ */