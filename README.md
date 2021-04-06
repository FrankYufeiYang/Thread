# Thread
Parallel with the Thread library

### Part 1 includes calculations of certain values from an array 
### Part 2 includes a matrix matrix multiplication algorithm 

## Introduction:
This Homework mainly focuses on parallelization using threads in C++. The goal is to find effective ways to parallelize two provided tasks so the execution time can be decreased. 

## System Model:
Every task is run on the RHEL7 interactive desktop. Unless specified, the core number is set to 20 by default for each test run. L1, L2, and L3 cache sizes are 32KB, 256KB, 30720KB respectively. Memory size is set separately to 1GB in the pbs file. 

We also need to include the thread library and add -pthread flag to makefile.

## Problem Statement:
In the first task, the provided code can calculate standard deviation and find certain values from an array. we need to thread the code and maintain the functional accuracy.

The second task requires us to thread the basic and the serially optimized version of matrix multiply algorithm. While maintaining the functionality, we need to observe the performance of original and optimized code. 
## Task 1:
From testing with different numbers of thread P and data size N, we can observe that the serial code execution time of part 1, namely, standard deviation and part 2, namely, threshold values are always at the same precision level. And the threaded code execution time of part 1 and part 2 are always at the same precision level as well. In fact, from table 1 below, the serial code of two parts have exactly the same execution time when N is 3500. This is probably because the system utilizes a very similar portion of computing elements to compute the two parts. So we can focus only on part 1, and effectively show the whole picture. 

![Table 1](https://github.com/FrankYufeiYang/Thread/blob/main/pictures/table1.jpg)

### _A. Non-associativity of floating-point calculation_
When calculating two floating-point numbers A and B, we don’t always get the same result. For example,  and  don’t give the same result as expected from regular arithmetic principles. If we look into the two results, the difference often doesn’t show up until after many digits after the decimal point. This means the two results effectively have the same value. However, because binary encoding for floating-point numbers is so different from decimal representation, the computer will always recognize them as two different numbers. This requires us to implement a comparison function for floating-points, so that within a certain range of error, ε, the function will give a positive answer. We implemented such a function in main.cpp.

### _B. Threading_
Threading the code is actually very straightforward, first, we recognize each individual subroutine, and do the five-step for each subroutine. For example, part 1 has 3 subroutines, that is,  finding mean, finding sum, finding top values. On top of that, we need to address the issue when N is not a multiple of P. We solve it by adding a check at the end of each subroutine. 
### _C. Scalability_
To accommodate different levels of memory, we set N to be 3500, 27000, 379000, and 1000000. Each N will occupy a level of cache. For example, when N is set to 27000, there will be 27000 long typed integers in the array, which is 216 KB in total. This takes most of the L2 cache. With that, P is also set to 5 different numbers, 4, 10, 20, 50, and 100. Having 4 threads is probably the situation we would encounter a lot in daily life, and having 20 threads should result in the best performance since our system has 20 cores. 

Before any observation on the actual result, We can reasonably predict that serial code should have very little run time on both level 1 and level 2 cache, considering their fast memory accessing speed; and the threaded optimization should really kick in on level 3 cache and memory level, because that is when memory latency become a huge bottleneck for performance. The same aspect should also apply on different numbers of threads, when the data size is too small to effectively use threading, having more threads will cause more overhead, therefore drag the performance of threading. When data size is large enough, memory latency will outweigh the overhead of creating and maintaining threads. 

It is indeed the case, when data is only occupying the L1 cache, having 20 threads isn’t really the best option. In fact, after observing table 2, we find that the execution time scales linearly with the number of threads. When having 100 threads, the run time is 21 times slower compared to having only 4 threads; 10 times slower compared to having 10 threads; 2 times slower compared to having 50 threads. Because of the linearity, the best option here is serialization, as P set to 1

![Table 2](https://github.com/FrankYufeiYang/Thread/blob/main/pictures/table2.jpg)

When data size grows, especially when it reaches RAM level, we don’t see that linear relationship anymore. Instead, having too little threads is as worse as having too many threads in this situation. We can observe that easily from table 3. Notice the result when P is 20, 0.007909 seconds, from table 1. It indeed has the best performance among 5 results. Also notice that, having 4 threads and having 100 threads result in almost the same run time. Why? It is because 4 and 100 both have a scaling relationship of 5 with 20. With only 4 threads, the system has very few threads to spread the data, so that it is again having bad memory latency because each thread has to deal with too much data. With 100 threads, each thread doesn’t have so much data anymore, but instead, the system needs to use extra computing power to create so many threads and maintain data consistency.

![Table 3](https://github.com/FrankYufeiYang/Thread/blob/main/pictures/table3.jpg)

## Task 2:
We use the same 5-step strategy to thread the basic and the serially optimized matrix multiply algorithm. We also need to address the same issue of N not being a multiple of P with the same solution. When N is set to 2000, and P is set to 20 according to 20 cores we have,  Figure 1 shows a clear comparison of performance between threaded and unthreaded algorithms. 

![Figure 1](https://github.com/FrankYufeiYang/Thread/blob/main/pictures/figure1.jpg)

We can see that the biggest bottleneck of matrix multiplication is the memory latency. By serial optimization alone, the execution time already dropped significantly to 3.85 seconds. However, by utilizing more computing power, we kind of brutally calculated the matrix multiplication at an even faster speed. Serial optimization alone reduced the run time by 7 times; threading alone reduced the run time by 16 times; but with both techniques, the run time was reduced by 64 times, 0.42 seconds. The optimization result is significant. 
### _A. Effectiveness of P and N_
From observations of task 1, we can make a wild guess: for what number of P and N, we would get a similar execution time from serial optimization and threading optimization? My guess is that setting P to a relatively small number, namely 4, and reducing N  to a point so that overall data size is somewhere between L2 and L3 cache, namely 110. After testing, the result for serial optimization is 0.00545, the result for threading optimization is 0.000511. The similar result further proved our observation on task 1, and we can conclude that task 2 would have the similar behaviors on different N and P as task 1. 
After doing more tests with various N and P on task 2, the result confirms this point. 

## Conclusion:
When optimizing for best performance, we need a balance between parallelization and serial optimization. We also need to choose data size and number of threads according to the real situation. 

