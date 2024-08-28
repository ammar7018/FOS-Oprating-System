\documentclass{article}
\usepackage{graphicx}
\usepackage{amsmath}
\usepackage{hyperref}

\title{FOS: Operating System Simulator}
\author{}
\date{}

\begin{document}

\maketitle

\section*{Introduction}

\textbf{FOS} (Operating System Simulator) is a powerful tool designed to emulate the core functionalities of an operating system. It showcases how an OS manages crucial aspects such as memory, CPU scheduling, threads, and file systems. By utilizing advanced data structures like linked lists and implementing algorithms such as Least Recently Used (LRU) and Round Robin, FOS achieves remarkable efficiency. During testing, FOS demonstrated exceptional performance, completing all operations within just a few milliseconds.

\section*{Implemented Features}

The following sections describe key features implemented in FOS, highlighting how they contribute to the overall efficiency and effectiveness of the operating system.

\subsection*{1. Kernel Heap Management}

\paragraph{Problem Description:}
In the initial design, the kernel virtual space \([KERNEL\_BASE, 4 \, \text{GB})\) was one-to-one mapped to the physical memory range \([0, 256 \, \text{MB})\). This configuration severely limited the kernel's ability to utilize physical memory beyond 256 MB, restricting its operations and scalability. Specifically, the kernel code could not access or use any physical memory located after the 256 MB boundary, which posed significant limitations for memory-intensive tasks.

\paragraph{Proposed Solution:}
To overcome this constraint, the one-to-one mapping approach was replaced with a more flexible, ordinary mapping strategy, akin to the method used for allocating user space. This adjustment allows the kernel to allocate memory frames across the entire physical memory space, irrespective of their location. The kernel can then access these frames by storing their frame numbers in the kernel's page table, ensuring seamless memory management and eliminating the previous 256 MB limitation.

\paragraph{Functions Implemented:}
The kernel heap management in FOS is enhanced by implementing the following key functions:

\begin{itemize}
    \item \textbf{Kmalloc():} 
    This function dynamically allocates memory of a specified size using the NEXT FIT strategy, which efficiently finds free space by scanning from the last allocated block. The allocated space is then mapped to the virtual memory, allowing the kernel to use it as required. The process is visually depicted in Figure~\ref{fig:kmalloc}, demonstrating the memory mapping and allocation steps.

    \begin{figure}[h!]
    \centering
    \includegraphics[width=0.8\textwidth]{path/to/your/image.png}
    \caption{Memory allocation and mapping using Kmalloc. The NEXT FIT strategy ensures efficient space utilization.}
    \label{fig:kmalloc}
    \end{figure}

    \item \textbf{Kfree():}
    This function is responsible for freeing previously allocated memory. It does so by removing all pages associated with the allocated space from the Kernel Heap. This ensures that memory is properly managed and that no resources are wasted by holding onto unused memory.

    \item \textbf{kheap\_virtual\_address():}
    This function retrieves the kernel virtual address corresponding to a given physical address. It is essential for the kernel to translate between physical and virtual addresses accurately, ensuring correct memory access and management.

    \item \textbf{kheap\_physical\_address():}
    This function performs the inverse operation of \texttt{kheap\_virtual\_address()}, converting a given kernel virtual address back to its corresponding physical address. This is crucial for tasks that require direct manipulation of physical memory, such as device drivers or low-level system operations.
\end{itemize}

\section*{Conclusion}

The implementation of the Kernel Heap in FOS exemplifies how an operating system can efficiently manage memory in a constrained environment. By adopting a flexible mapping strategy and implementing key memory management functions, FOS ensures that the kernel can operate effectively, regardless of physical memory limitations. This, along with other features like CPU scheduling and thread management, makes FOS a highly performant and versatile operating system simulator.

\end{document}
