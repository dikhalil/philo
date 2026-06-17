# Philosophers


<p align="center">
  <i>Dining Philosophers Problem — 42 Network</i>
</p>

## Overview

The classic synchronization problem solved in C using **threads and mutexes** (mandatory) and **processes and semaphores** (bonus). The goal is to prevent deadlock and starvation while ensuring every philosopher gets to eat.

## Project Structure

```
├── philo/              # Mandatory — threads + mutexes
│   ├── include/philo.h
│   ├── src/
│   │   ├── philo.c        # Entry point
│   │   ├── check.c        # Argument validation
│   │   ├── init.c         # Data and mutex initialization
│   │   ├── threads.c      # Thread creation, joining, monitoring
│   │   ├── routine.c      # Philosopher lifecycle (eat/sleep/think)
│   │   ├── time.c         # Timestamp utilities
│   │   ├── utils.c        # Parsing and logging
│   │   └── exit.c         # Cleanup and error handling
│   └── Makefile
│
├── philo_bonus/         # Bonus — processes + semaphores
│   ├── include/philo_bonus.h
│   ├── src/
│   │   ├── philo_bonus.c
│   │   ├── check_bonus.c
│   │   ├── init_bonus.c
│   │   ├── process_bonus.c # Fork, process management, per-child monitoring
│   │   ├── routine_bonus.c
│   │   ├── time_bonus.c
│   │   ├── utils_bonus.c
│   │   └── exit_bonus.c
│   └── Makefile
│
└── README.md
```

## Build

```bash
# Mandatory
make -C philo

# Bonus
make -C philo_bonus

# Clean build
make -C philo re
make -C philo_bonus re

# Clean artifacts
make -C philo fclean
make -C philo_bonus fclean
```

## Usage

```
./philo <number> <time_to_die> <time_to_eat> <time_to_sleep> [meals_count]
```

All time values are in milliseconds.

| Argument | Description |
|---|---|
| `number` | Number of philosophers and forks |
| `time_to_die` | Max ms without eating before death |
| `time_to_eat` | Time spent eating (holds 2 forks) |
| `time_to_sleep` | Time spent sleeping |
| `meals_count` | Optional — simulation ends when each philosopher eats this many |

## Examples

```bash
# Single philosopher — can't get second fork, dies
./philo 1 800 200 200

# 5 philosophers with ample time — runs indefinitely
./philo 5 800 200 200

# 5 philosophers, each eats 7 times then stops
./philo 5 800 200 200 7

# Tight timing — a philosopher eventually starves
./philo 4 310 200 100
```

## Output

```
<timestamp_ms> <id> <action>
```

Actions: `has taken fork`, `is eating`, `is sleeping`, `is thinking`, `died`

The death message is printed exactly once and is immediately followed by program termination.

## Implementation Details

### Mandatory (`philo/`)

| Component | Approach |
|---|---|
| Concurrency | 1 thread per philosopher |
| Fork protection | 1 mutex per fork |
| State protection | `data_lock` mutex guards `stop`, `meals_count`, `last_meal` |
| Logging | `print_lock` mutex ensures atomic output |
| Deadlock prevention | Even IDs pick right fork first; odd IDs pick left fork first |
| Monitor | Main thread polls all philosophers; kills simulation on death |
| Meal limit | Optional 6th argument; monitor stops simulation when all have eaten enough |

### Bonus (`philo_bonus/`)

| Component | Approach |
|---|---|
| Concurrency | 1 process per philosopher |
| Fork protection | Named semaphore (`/forks`) with N permits |
| Eater limit | Named semaphore (`/eat`) with N/2 permits prevents deadlock |
| Logging | Named semaphore (`/print`) ensures atomic output |
| Monitor | 1 thread per child process checks death/meal completion |
| Cleanup | Parent waits via `waitpid`; children exit with status 0 (full) or 1 (death) |
| Shared state | `t_data` is shared across processes via `sem_open` named semaphores |
