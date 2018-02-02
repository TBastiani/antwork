# Antwork

## What's this?

The idea is to enable concurrent execution of an arbitrary set of tasks that 
have arbitrary dependencies on each other - i.e a graph of tasks.

This project is mostly a toy that I built as a proof-of-concept in my spare 
time. Recently, I've become aware of TBB's "flow graph" which looks like it 
might have similar goals.

[https://www.threadingbuildingblocks.org/tutorial-intel-tbb-flow-graph](https://www.threadingbuildingblocks.org/tutorial-intel-tbb-flow-graph)

## Future work

Depending on the actual work that we're trying to parallelise and the number of 
hardware threads available on the target machine, the execution order of the 
graph might affect throughput quite a lot. I have been toying with the idea to 
bolt on a task-scheduler that would affect the order in which the graph is 
traversed in order to maximise performance.
