*This project has been created as part of the 42 curriculum by dioppolo.*

# Codexion

Master the race for resources before the deadline masters you.

## Description

Codexion is a multithreaded C simulation of coders competing for pairs of shared dongles. Each coder repeatedly acquires its left and right dongles, compiles, releases the dongles, debugs, and refactors. A coder has a burnout deadline, and the simulation stops when a coder burns out or, when a completion target is specified, every coder reaches that target.

Requests for dongle pairs are ordered using one of two scheduling policies: FIFO (first request first) or EDF (earliest deadline first). A configurable cooldown prevents released dongles from being reused immediately.

## Instructions

### Requirements

- A C compiler such as `cc` or `gcc`
- GNU Make
- POSIX threads (`pthread`), available on the target POSIX system

### Build

From the repository root, run:

```sh
make
```

This creates the `codexion` executable. To remove the executable and generated object files, run `make fclean`.

### Run

```sh
./codexion <num_coders> <t_burnout> <t_compile> <t_debug> <t_refactor> <req_compiles> <cooldown> <fifo|edf>
```

All time values are in milliseconds. `num_coders`, `t_burnout`, and `t_compile` must be positive integers. Debug and refactor durations, the requested compilation count, and cooldown may be zero. A requested compilation count of `0` means the simulation has no completion target and runs until burnout.

Example:

```sh
./codexion 2 800 100 100 100 2 50 fifo
```

This runs two coders with an 800 ms burnout deadline, 100 ms compile/debug/refactor phases, two required completions per coder, a 50 ms dongle cooldown, and FIFO scheduling.

Output lines contain elapsed milliseconds, coder ID, and status, for example:

```text
1 1 has taken the left dongle
1 1 has taken the right dongle
1 1 is compiling
```

## Blocking cases handled

- **Deadlock prevention and Coffman's conditions:** A coder requests its pair as a unit. Under the pair mutex, the program checks that both dongles are free and then marks both as in use before another requester can proceed. Coders do not hold one dongle while waiting for the other, eliminating the hold-and-wait condition; atomic pair reservation also prevents circular wait between neighboring coders. Initial acquisition is staggered in phases to reduce contention at startup.
- **Queue fairness and starvation:** FIFO mode orders waiters by request time, with coder ID as a tie-breaker, so later requests do not overtake earlier queued requests. EDF mode instead prioritizes the earliest burnout deadline (then request time and coder ID). EDF is priority-based and is not a strict starvation-free guarantee for every possible workload.
- **Cooldown:** Both dongles in a pair receive the same `available_at` time when released. The next eligible request waits on the pair condition variable until the cooldown expires, then rechecks availability while holding the pair mutex.
- **Burnout detection:** The monitor checks active coder deadlines and stops the simulation when a deadline has passed and the coder has not met its completion target. Deadlines are stored in milliseconds and the monitor polls approximately every millisecond; therefore detection and printed status are subject to that polling interval and system scheduling.
- **Serialized output:** Status lines are protected by a write mutex, so concurrent coder and monitor output does not interleave. Logging also checks the simulation state before printing, preventing routine status messages after shutdown.
- **Shutdown wakeups:** When the simulation stops, the monitor broadcasts on the start, pair, and per-dongle condition variables so threads blocked during phase coordination or resource waiting can recheck the stopped state and exit.

## Thread synchronization mechanisms

Codexion uses POSIX `pthread_mutex_t` and `pthread_cond_t` primitives, together with a custom event-like coordination flow built from a priority heap and condition-variable notifications. It does not use a separate event-library abstraction.

- **Dongle-pair access:** `pair_mutex` protects the shared pair-request heap and the `is_in_use` and `available_at` state used to grant both adjacent dongles together. A coder adds its request to the heap, then waits on `pair_cond` unless it is the highest-priority eligible request. Releasing a pair updates both dongles and broadcasts on `pair_cond`, prompting waiting coders to recheck the conditions. The individual dongle mutexes and condition variables are initialized and included in shutdown wakeups; pair assignment itself is serialized by `pair_mutex`.
- **Initial phases:** `start_mutex` protects the initial-phase counters. Coders wait on `start_cond` until their phase is enabled; completing a phase advances the counter and broadcasts to the next group.
- **Simulation and monitor state:** `sim_mutex` protects the running flag, coder deadlines, and compilation counters. The monitor reads a coder's shared state while holding this mutex. To stop the simulation, it changes the running flag under the mutex and broadcasts to waiting threads; workers check the same flag before proceeding.
- **Logging:** `write_mutex` serializes complete output lines. Logging checks the running flag under `sim_mutex` while holding the write lock, so a status line cannot be partially interleaved with another thread's output.
- **Cooldown notifications:** The next release time is calculated for the pair, and a timed wait on `pair_cond` avoids busy-waiting through the cooldown. After a timed wait or notification, the requester loops and checks the shared state again under the mutex.

These locks and predicate-checking loops prevent unsynchronized updates to shared state and handle condition-variable wakeups by always rechecking the relevant condition.

## Resources

- `pthread_create`, mutexes, and condition variables: [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/)
- [The Open Group Base Specifications: pthread mutex functions](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html)
- [The Open Group Base Specifications: pthread condition variables](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html)
- Dijkstra, E. W., [Cooperating Sequential Processes](https://www.cs.utexas.edu/users/EWD/transcriptions/EWD01xx/EWD123.html), background on synchronization and resource sharing.

### AI use

AI assistance was used to draft and organize this README and to cross-check its descriptions of build/run usage, scheduling, and synchronization against the project source files. AI was not used to implement or modify the C simulation as part of this README task.