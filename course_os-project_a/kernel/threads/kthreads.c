/*
 * kthreads.c
 *
 *  Created on: Apr 23, 2015
 *      Author: mwkurian
 */

#include "kthread.h"
#include <global_defs.h>

#include "klibc.h"
#include "mem_alloc.h"
#include "loader.h"
#include "vm.h"
#include "elf.h"
#include "drivers/timer.h"
#include "scheduler.h"


static uint32_t GLOBAL_TID;

int init_all_processes()
{
	// pcb_table = kmalloc(MAX_PROCESSES * 4);
	// os_memset(pcb_table, 0, sizeof(int) * MAX_PROCESSES);
	GLOBAL_TID = 0;
	return 0;
}

//kthread_callback_handler cb_handler - do we need this in the argument 

kthread_handle* kthread_create(void* thread, void* func, void* args)
{
	vm_new_vas(); 
	// make sure you are in the right virtual address at the right 

	kthread_handle * kthread = kmalloc(sizeof(kthread_handle));
	os_printf("kthread is malloced"); 
	sched_task* current_task = get_active_task(); 
	vas* virtual_address_space = current_task->task->stored_vas; 
	kthread->stored_vas = virtual_address_space; //get current process's stored vas//get currently running process, and get it from scheduler - active task and get current process() --> write the function 
	os_printf("kthread stored virtual address space is created"); 
	kthread->tid = GLOBAL_TID; 
	kthread->R15 = func; 
	kthread->R0 = args; 
	//R0 - R4 is for arguments - but only need R0
	//set up new threads registers
	//load thread state amd set the program counter to func - setting kthreads r15 member 
	//r0 = args 
	//r15 = func
	//r13 = stack pointer  
	//r14 - return value, when you pop off the stack where do you return to 
	return kthread;
}

//make pcb changes to thread
int32_t* get_address_of_PCB(uint32_t PID)
{
	if (PID <= 0)
	{
		os_printf("Invalid PID of: %d, exiting.", PID);
		return 0;
	}

	int i;
	uint32_t* current_address = pcb_table;

	for (i = 0; i < MAX_PROCESSES; ++i)
	{
		if ((*current_address) != 0)
		{
			pcb* pcb_p = (pcb*) *current_address;
			if (pcb_p->PID == PID)
			{
				return current_address;
			}
		}
		current_address++;
	}

	os_printf("Did not find PID: %d\n", PID);
	return 0;
}

//make pcb changes to thread
uint32_t kthread_save_state(uint32_t tid)
{
	uint32_t* thread_to_save = get_address_of_thread(tid);
	kthread_handle* kthread_t = get_PCB(tid);

	if (((uint32_t) process_to_save) == -1 || pcb_p == 0)
	{
		os_printf("Invalid tid in load_process_state");
		return 0;
	}

	asm("MOV %0, r0":"=r"(kthread_t->R0)::);
	asm("MOV %0, r1":"=r"(kthread_t->R1)::);
	asm("MOV %0, r2":"=r"(kthread_t->R2)::);
	asm("MOV %0, r3":"=r"(kthread_t->R3)::);
	asm("MOV %0, r4":"=r"(kthread_t->R4)::);
	asm("MOV %0, r5":"=r"(kthread_t->R5)::);
	asm("MOV %0, r6":"=r"(kthread_t->R6)::);
	asm("MOV %0, r7":"=r"(kthread_t->R7)::);
	asm("MOV %0, r8":"=r"(kthread_t->R8)::);
	asm("MOV %0, r9":"=r"(kthread_t->R9)::);
	asm("MOV %0, r10":"=r"(kthread_t->R10)::);
	asm("MOV %0, r11":"=r"(kthread_t->R11)::);
	asm("MOV %0, r12":"=r"(kthread_t->R12)::);
	asm("MOV %0, r13":"=r"(kthread_t->R13)::);
	asm("MOV %0, r14":"=r"(kthread_t->R14)::);
	asm("MOV %0, r15":"=r"(kthread_t->R15)::);

	return 1;
}
uint32_t kthread_load_state(AS_KTHREAD(active_task))
{

}

/*pcb* process_create(uint32_t* file_p)
{

	uint32_t* free_space_in_pcb_table = next_free_slot_in_pcb_table();

	//This used to be == 0, which doesn't seem correct
	if (free_space_in_pcb_table != 0)
	{
		pcb* pcb_pointer = (pcb*) kmalloc(sizeof(pcb));

		//Create the process VAS here so that we can use it when allocating process memory
		pcb_pointer->stored_vas = vm_new_vas();

		//Load the file. This function returns the parsed ELF header.
		Elf_Ehdr* success = (Elf_Ehdr*) load_file(pcb_pointer, file_p);

		if (!success)
		{
			return (pcb*) -1;
		}

		//Debug, should be removed once scheduler works
		os_printf("THIS IS R13: %X \n", pcb_pointer->R13);

		//fill the free space with a pcb pointer
		*free_space_in_pcb_table = (uint32_t) pcb_pointer;
		//initialize PCB		
		pcb_pointer->PID = ++GLOBAL_PID;
		//4-13-15: function pointer should point to main() of file pointer.
		//         TODO: Eventually should be able to pass parameters. Put them on the stack (argv/argc)
		pcb_pointer->R15 = success->e_entry;

		os_printf("%X ENTRY: %X \n", file_p, success->e_entry);

		pcb_pointer->current_state = PROCESS_NEW;

		pcb_pointer->has_executed = 0;
		return pcb_pointer;

	}
	else
	{
		os_printf("Out of memory in pcb table\n");
		return 0;
	}
}*/

uint32_t kthread_start(kthread_handle * kthread)
{
	sched_task * task = sched_create_task(kthread);
	sched_add_task(task);
}

