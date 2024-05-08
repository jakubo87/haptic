# Petri-Net simulated cell interactions

planned:
bosst graph library (parallel?) for adjacency considerations to not go through ALL the transitions, but only those that after a transition can still fire.
Will not always work, but should be good enough to speed up through transitions, without losing randomness




## (basic modeling idea)
### class "node"
Simple PL42 example:
``` python
p1 = {
    label : 'Node 1',
    type: 'NUMERIC'
};

p2 = {
    label : 'Node 2',
    type: 'NUMERIC'
};

t  = {
    label : 'transition',
    context = "general":{
        p2 : passForward(p1)
    }
};

STATE {
    p1: 2,
    p2: 3
};
```

## current v0 (naive data-centric implementation - no graphs and no adjacency, yet)
Type...
```
make
```
...to compile, then...
```
make run
```
to run the program with fixed number of nodes and transitions. Currently 10000 Nodes/transitions/starting tokens with 20k repetitions scans through all the transitions, checking whether they execute and execute if the do. It takes an Intel i5 5200U laptop CPU around 7,8s to finish - average of 5 runs.
Program is single threaded. std::execute::unseq gives no guarantee and seems slightly detrimental to performance (-0.1s). ..::par - versions fail to compile so far.
Distributing data across threads on shared memory system should be rather easy. Just hints to performance, no meaningful benchmark yet.

Currently there are only 1:1 transitions, i.e. 1 token from a source node goes to 1 target node. And there are no hints after transitions that calculate changed ready transition. It just naively scans transitions from a list. But uses locks.

### Data centric model instead of object oriented
store properties in contiguous dense vectors instead of vector of objects for better locality and cache reuse. Task will be very memory IO intensive as it is.

Transitions are nodes too, if they have more then one in or output. The edges and their annotiations are functions, which can be called, or parameters to functions that can be called.

## Planned Use with Frontend(?)
- Frontend for instance in python and QT, or JS to output a JSON graph with all annotations and parameters. 
- read JSON (JSON simd) with C++ program and do the heavy lifting
- output desired states to the user via some graph tool (likely graphviz -> dot)

## Caution
- avoid central locking mechanism -> performance
    - have existing transitions ready
        - have "ready" transitions ready (quicker scheduling)
- avoid transition cascades -> bad simulation / lack of randomness



## Voting system
A voting system for each node would determine to which thread a node should belong. With each access each of the threads would leave behind a little mark (+1) to the vector of visiting threads. [+1,+4,+7,+1,+22] for 5 threads for instance. ThID4 would be most frequent here.

if a transition combining 2 or mostly red nodes, it turn red itself and gets exchanged to a pool of a corresponding thread.

Bio algorithm: ants work change

### pro
- would increase temporal locality
- less interaction between threads -> fewer lock waits / false sharing 
- 

### con
- a lot of overhead, memory/IO
- would largely defy benefits of property map / data driven storage in contiguous containers -> spatial locality -> better array of objects..?
- with larger problem size, spatial locality may not be as important any more
- areas may have different speeds... quality of simulation?
- problem may be too dynamic as to establish some seemingly static regions -> slowdown -> critical paths
- more nodes would have to stay locked for longer

### possible workarounds
- making a sum to normalize transition pool size against frequency.
- 


## Open quetions
What kind of hardware is it going to be used on? Accelerators? Shared memory? distributed/OpenMPI? locally/remote?

Will running out of memory become a problem? Overflow?

Is it worth it to recompile before execution or make everything generic enough to compile once and deal with inputs?
- recompiling could allow to make variables from compiletime constants. (number of nodes, transitions and locks) 

How sparse is the system? How many overlaps (lock interferences) will there be between threads? Deadlocks?

Multiset or explicit?

Explicit Preload?


## Further experiences made:
- try_lock is probably not necessary due to scale factors (too few collision) - according to cpprefcom lock uses deadlock algorithm if repeated locking attempts fail. But apparently, it doesn't... lock guarantees execution. 
- Hypothesis:  self loops cause deadlocks with lock guard CONFIRMED! buck deadlocks can still occur!
- shared mutexes may provide benefits for some read only instances. it is however not possible to upgrade shared acces to an exclusive one anymore (booo!) -> non blocking seems best for now

## Correctness Pitfalls
- not random enough
- Floats representing concentrations can grow and then shirink. Shrinking can lead to numeric problems. 34bit mantissa (roughly 1^9) may not be enough to model and hold vastly differing concentration levels when subtracting (addition, multiplication and division would not be a problem) in a long running and dynamic simulation.

## Performance considerations
### division into cells
...and assigning a task/processor/ to it. 
if the cell is small enough 1 is enough and only few nodes need to interact outside the cells, ie not needing synchronisation and locking.
Outside would mean one level down the hierarchy. Only those cells need locking. Maybe a cell wide lock would be necessary.

In some cases the Outside may be device or even network interaction.
