# FOS: Operating System Simulator

FOS is an operating system simulator that demonstrates how an OS manages memory, CPU scheduling, threads, and file systems using data structures like linked lists and algorithms like LRU and Round Robin. It is designed to perform OS tasks efficiently and achieve great performance and speed, with all operations completed within just a few milliseconds during testing.

## Implementations

### 1. Kernel Heap

**Problem:**

The kernel virtual space `[KERNEL_BASE, 4 GB)` is currently one-to-one mapped to the physical memory `[0, 256 MB)`. This restricts the kernel's use of physical memory to just 256 MB. As a result, the kernel cannot utilize any physical memory beyond the 256 MB limit.

**Solution:**

To overcome this limitation, the one-to-one mapping is replaced with an ordinary mapping, similar to how user space allocation is handled (using page directory and page table more details in figure). This change allows the kernel to allocate frames anywhere in physical memory and access them by storing the frame number in the kernel's page table.

**Functions:**

- **`Kmalloc()`**: Dynamically allocates a space of the specified size using the NEXT FIT and Best Fit strategies and maps it to memory as illustrated in the figure below.

- **`Kfree()`**: Frees a previously allocated space by removing all its pages from the Kernel Heap.

- **`kheap_virtual_address()`**: Retrieves the kernel virtual address corresponding to a given physical address.

- **`kheap_physical_address()`**: Retrieves the physical address corresponding to a given kernel virtual address.

[Kernel Heap Mapping]![image](https://github.com/user-attachments/assets/0eb19c74-dc06-47a5-8310-045cb8eb2538))

**File Path**
  FOS_PROJECT/kern/kheap.c

### 2. Fault Handler

**Problem:**

When a process needs a file that is not in its working set the OS handles this case by selecting what will be removed based on Algorthim that is implemented, In our project, we implement (Nth chance clock). Note if there is space in the working set we don't need to remove any page, The MMU helps the OS by handling the fault by loading the required page from disk (such as a swap file or a memory-mapped file) into RAM, the figure will explain the process in more detail.

[page fault]![image](https://github.com/user-attachments/assets/fd6334ee-a4c0-4d8c-b45c-726e1be11581)

**solution**

We will allocate a new page in the main memory for the faulted page. Then, load the faulted page back into the main memory from the page file, if the page exists. Otherwise (for new pages), add it to the page file (for new stack pages only). This process will be carried out if there is space in the working set. If the process exceeds the working set size, we will need to replace pages by applying the Nth Chance Clock algorithm.

**Functions:**

- **`Placement()`**: 
- **`RePlacement()`**:

**file path**
  kern/trap.c

### 3. CPU Scheduling by MLFQ

**Problem:**

Many processes need to use the CPU, and various problems can arise if we don't manage CPU utilization effectively. For example, starvation occurs when a process uses the CPU for an extended period without considering other shorter processes, which negatively affects user experience.

**solution**

There are many Algorithms and techniques that can used to solve this problem like applying RR, Shortest Remaining Time, or MLFQ. Each algorithm has its drawbacks. For example, RR can produce poor results for I/O-bound processes, leading to an increase in the variance in response times. You should implement the algorithm that is suitable for your needs, so we will use MLFQ. The figure will show how it works.


[MLFQ]![image](https://github.com/user-attachments/assets/a11b33a4-044e-49e6-9f3b-f30d368d8902)

**Functions:**

- **`Initialize the MLFQ (sched_init_MLFQ)`**: 
- **`Handle the Scheduler (fos_scheduler)`**:

**file path**
  kern/sched.c

## Conclusion

The FOS simulator provides a comprehensive platform to explore and understand various aspects of operating system functionality. By implementing key features such as the Kernel Heap, Fault Handler with Nth Chance Clock, and CPU Scheduling with MLFQ, FOS demonstrates effective strategies for managing memory and CPU resources. The chosen algorithms and techniques address critical challenges like memory management and CPU utilization, optimizing performance, and ensuring efficient operation. The figures included offer visual insights into how these systems work, aiding in the understanding of complex concepts and implementations.

With these implementations, FOS not only simulates core OS functionalities but also serves as a valuable tool for testing and evaluating operating system behaviors and performance under various conditions.

## Credits
- [Ammar Hassan](https://www.linkedin.com/in/ammar-hassan-5a16551a1/)
- [Youssef El-Deeb](https://www.linkedin.com/in/youssef-eldeeb-1a4269253/)
- [Nadine El-Kady](https://www.linkedin.com/in/nadine-elkady-4b45792b1/)
- [Reem Tarek](https://www.linkedin.com/in/reem-mohamed-66633124a/?utm_source=share&utm_campaign=share_via&utm_content=profile&utm_medium=ios_app)

