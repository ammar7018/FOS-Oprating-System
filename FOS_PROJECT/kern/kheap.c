#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//2022: NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)


//assume the maximum allocation is 1000
struct mem_info{
	int num_of_frames;
	uint32 start_address;
	uint32 end_address;
}all_prog[1000];

int idx=0;
int nxt_start=KERNEL_HEAP_START;
int tot_frames=(KERNEL_HEAP_MAX-KERNEL_HEAP_START)/PAGE_SIZE;

uint32 Get_pages(uint32 start_address,int number_frames){
//	cprintf("HERE %d",number_frames);
	int cnt=0;
	uint32 i=start_address;
	uint32 tmp=start_address;
	uint32 ret=start_address;

	while(1){
		uint32 *page_table=NULL;
		get_page_table(ptr_page_directory,(void*)i,&page_table);
		uint32 page=page_table[PTX(i)];
		if((page&PERM_PRESENT)==0){
			if(cnt==0){
				ret=i;
			}
			cnt++;
		}else{
			cnt=0;
		}

		if(cnt==number_frames){
			return ret;
		}

		i+=PAGE_SIZE;

		if(i>=KERNEL_HEAP_MAX){
			i=KERNEL_HEAP_START;
			cnt=0;
		}

		if(i==tmp){
			return 0;
		}
	}
}

int allocate_map_frame(int need_frames,uint32 st_address,int op){


	while(need_frames>0){

		uint32 *ptr_page_table=NULL;
		struct Frame_Info* ptr_frame=get_frame_info(ptr_page_directory,(void*)st_address,&ptr_page_table);

		if(ptr_frame!=NULL){
			return -1;
		}

		allocate_frame(&ptr_frame);
		int ret=map_frame(ptr_page_directory,ptr_frame,(void*)(st_address),PERM_WRITEABLE);
		if(ret==E_NO_MEM){
			free_frame(ptr_frame);
			return -1;
		}

		st_address+=PAGE_SIZE;
		if(st_address>=KERNEL_HEAP_MAX){
			st_address=KERNEL_HEAP_START;
		}

		need_frames--;
	}

	all_prog[idx].end_address=st_address;
	if(op==1){
		nxt_start=all_prog[idx].end_address;
	}

	idx++;
	return 1;
}


void *Next_fit(unsigned int size){

	//Get Number of needed frames
	int need_frames=(size+PAGE_SIZE-1)/PAGE_SIZE;
	all_prog[idx].num_of_frames=need_frames;

	//check if we have enough frames in mem
	if(need_frames>tot_frames){
		return NULL;
	}

	uint32 st_address=Get_pages(nxt_start,need_frames);

//	cprintf("the start address of program index %d is %d\n",idx,st_address);

	if(st_address==0){
		return NULL;
	}

	tot_frames-=need_frames;

	all_prog[idx].start_address=st_address;

	//allocate frames

	if(allocate_map_frame(need_frames,st_address,1)==-1){
		return NULL;
	}

	return (void*)all_prog[idx-1].start_address;

}


void* Best_fit(int size){
	int need_frames=(size+PAGE_SIZE-1)/PAGE_SIZE;
	all_prog[idx].num_of_frames=need_frames;
	int min=10000000;
    int cnt=0;
    uint32 found_st_add;
	uint32 cur_st_add;

    for(uint32 i=KERNEL_HEAP_START;i<KERNEL_HEAP_MAX;i+=PAGE_SIZE){
		uint32 *page_table=NULL;
		get_page_table(ptr_page_directory,(void*)i,&page_table);
		uint32 page=page_table[PTX(i)];
		if((page&PERM_PRESENT)==0){
			if(cnt==0){
				cur_st_add=i;
			}
			cnt++;
		}
		else{
			if(cnt>=need_frames && cnt<min){
				if(cnt<min){
					min=cnt;
				    found_st_add=cur_st_add;
				}
			}
			cnt=0;
		}
    }

    if(cnt>=need_frames && cnt<min){
    	if(cnt<min){
    		min=cnt;
    		found_st_add=cur_st_add;
    	}
	}

    if(found_st_add!=0){
    	tot_frames-=need_frames;
    	all_prog[idx].start_address=found_st_add;
        while(need_frames>0){
    		uint32 *ptr_page_table=NULL;
    		struct Frame_Info* ptr_frame=get_frame_info(ptr_page_directory,(void*)found_st_add,&ptr_page_table);
    		if(ptr_frame!=NULL){
    			return NULL;
    		}

    		allocate_frame(&ptr_frame);
    		int ret=map_frame(ptr_page_directory,ptr_frame,(void*)(found_st_add),PERM_WRITEABLE);
    		if(ret==E_NO_MEM){
    			free_frame(ptr_frame);
    			return NULL;
    		}
    		found_st_add+=PAGE_SIZE;
    		if(found_st_add>=KERNEL_HEAP_MAX){
    			found_st_add=KERNEL_HEAP_START;
    		}

    		need_frames--;
        }

		all_prog[idx].end_address=found_st_add;
    	nxt_start=all_prog[idx].end_address;
    	idx++;
    	return (void*)all_prog[idx-1].start_address;
    }
    else{
       return NULL;
    }

}

void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT 2024 - [1] Kernel Heap] kmalloc()

	//TODO: [PROJECT 2024 - MS1 - BONUS1] Implement a Kernel allocation strategy

	if(isKHeapPlacementStrategyNEXTFIT()){
		return Next_fit(size);
	}
	else if(isKHeapPlacementStrategyBESTFIT()){
		return Best_fit(size);
	}
	else{
		return NULL;
	}
}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT 2024 - MS1 - [2] Kernel Heap] kfree()

	uint32 va=ROUNDDOWN((uint32)virtual_address,PAGE_SIZE);

	for(uint32 i=0;i<idx;i++) {

		if(va==all_prog[i].start_address){

			int need_to_remove=all_prog[i].num_of_frames;
			uint32 st=all_prog[i].start_address;

			while(need_to_remove>0){
				unmap_frame(ptr_page_directory,(void*)st);
				st+=PAGE_SIZE;
				if(st>=KERNEL_HEAP_MAX){
					st=KERNEL_HEAP_START;
				}
				need_to_remove--;
			}

			tot_frames+=all_prog[i].num_of_frames;
			all_prog[i].num_of_frames=0;
			all_prog[i].start_address=0;
			all_prog[i].end_address=0;

			cprintf("I Remove Frames successfully %x \n",virtual_address);

			return;
		}
	}

	cprintf("can't Found Frames of va %x \n",virtual_address);
	return;
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{

	//TODO: [PROJECT 2024 - MS1 - [3] Kernel Heap] kheap_virtual_address()

	struct Frame_Info *ptr_info_ph = to_frame_info(physical_address);

	int FrameNumPh = to_frame_number(ptr_info_ph);
	int offset = physical_address-(FrameNumPh*PAGE_SIZE);

	for(uint32 i=KERNEL_HEAP_START;i<KERNEL_HEAP_MAX;i+=PAGE_SIZE){

		uint32 *ptr_page=NULL;
//		struct Frame_Info *ptr_frame=get_frame_info(ptr_page_directory,(void*)i,&ptr_page);

		get_page_table(ptr_page_directory,(void*)i,&ptr_page);

		if((ptr_page[PTX(i)]>>12)==FrameNumPh){
			return i+offset;
		}


	}

	return 0;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT 2024 - MS1 - [4] Kernel Heap] kheap_physical_address()
	unsigned int physical_address;
	uint32 *ptr_page_table = NULL;

	get_page_table(ptr_page_directory,(void*)virtual_address,&ptr_page_table);
	if(ptr_page_table!=NULL){
		uint32 table_entry = ptr_page_table[PTX(virtual_address)];

		int FrameNum = table_entry>>12;
		int offset = virtual_address % PAGE_SIZE;
		physical_address = (FrameNum*PAGE_SIZE)+offset;
	}else{
		cprintf("can't found page table\n");
	}

	return physical_address;
}
