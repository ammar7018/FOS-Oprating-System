# FOS: Operating System Simulator

FOS is an operating system simulator that demonstrates how an OS manages memory, CPU scheduling, threads, and file systems using data structures like linked lists and algorithms like LRU and Round Robin. It is designed to perform OS tasks efficiently and achieve great performance and speed, with all operations completed within just a few milliseconds during testing.

## Implementations

### 1. Kernel Heap

**Problem:**

The kernel virtual space `[KERNEL_BASE, 4 GB)` is currently one-to-one mapped to the physical memory `[0, 256 MB)`. This restricts the kernel's use of physical memory to just 256 MB. As a result, the kernel cannot utilize any physical memory beyond the 256 MB limit.

**Solution:**

To overcome this limitation, the one-to-one mapping is replaced with an ordinary mapping, similar to how user space allocation is handled. This change allows the kernel to allocate frames anywhere in physical memory and access them by storing the frame number in the kernel's page table.

**Functions:**

- **`Kmalloc()`**: Dynamically allocates a space of the specified size using the NEXT FIT and Best Fit strategies and maps it to memory as illustrated in the figure below.

- **`Kfree()`**: Frees a previously allocated space by removing all its pages from the Kernel Heap.

- **`kheap_virtual_address()`**: Retrieves the kernel virtual address corresponding to a given physical address.

- **`kheap_physical_address()`**: Retrieves the physical address corresponding to a given kernel virtual address.

[Kernel Heap Mapping]![image](https://github.com/user-attachments/assets/0eb19c74-dc06-47a5-8310-045cb8eb2538))

### 2. Fault Handler

**Problem:**

When a process needs a file that is not in its working set, the CPU handles this case by determining if there is space in the working set or if it needs to select a page for removal based on the algorithm implemented in our project (Nth Chance Clock). The MMU assists the OS by handling the fault and loading the required page from disk (such as a swap file or a memory-mapped file) into RAM. The figure below explains the process in more detail.

**solution**

We will allocate a new page in the main memory for the faulted page. Then, load the faulted page back into the main memory from the page file, if the page exists. Otherwise (for new pages), add it to the page file (for new stack pages only). This process will be carried out if there is space in the working set. If the process exceeds the working set size, we will need to replace pages by applying the Nth Chance Clock algorithm.

Steps


**Functions:**

- **`Placement()`**: 
- **`RePlacement()`**:

### 3. CPU Scheduling by MLFQ

**Problem:**

Many processes need to use the CPU, and various problems can arise if we don't manage CPU utilization effectively. For example, starvation occurs when a process uses the CPU for an extended period without considering other shorter processes, which negatively affects user experience.

**solution**

There are many algorithms and techniques that can be used to solve this problem, such as applying Round Robin (RR), Shortest Remaining Time, or Multi-Level Feedback Queue (MLFQ). Each algorithm has its drawbacks. For example, RR can produce poor results for I/O-bound processes, lead to inefficient use of I/O devices, and increase the variance in response times. You should implement the algorithm that is most suitable for your needs, so we will use MLFQ. The figure will demonstrate how it works.


**Functions:**

- **`Initialize the MLFQ (sched_init_MLFQ)`**: 
- **`Handle the Scheduler (fos_scheduler)`**:



1. Initialize the MLFQ (sched_init_MLFQ)
2. Handle the Scheduler (fos_scheduler)
