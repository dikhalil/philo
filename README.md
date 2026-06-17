# Philosophers

> 42 School project — solving the Dining Philosophers Problem with threads/mutexes and processes/semaphores.

## The Problem

N philosophers sit at a round table with a bowl of spaghetti. Each philosopher alternates between **eating**, **thinking**, and **sleeping**. There are N forks — one between each pair of philosophers. A philosopher needs **two forks** to eat. The challenge is to prevent deadlock (everyone holding one fork and waiting) and starvation (a philosopher never gets to eat) while ensuring no philosopher starves to death.

## Project Structure

```
├── philo/          # Mandatory part: threads + mutexes
│   ├── include/    # Header files
│   ├── src/        # Source files
│   └── Makefile
├── philo_bonus/    # Bonus part: processes + semaphores
│   ├── include/
│   ├── src/
│   └── Makefile
└── README.md
```

## Compilation

```bash
# Mandatory
cd philo && make

# Bonus
cd philo_bonus && make

# Clean build
make re

# Clean objects
make clean

# Full clean (objects + binary)
make fclean
```

## Usage

```
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]
```

| Argument | Description |
|----------|-------------|
| `number_of_philosophers` | Number of philosophers (and forks) |
| `time_to_die` | Time in ms before a philosopher dies from starvation |
| `time_to_eat` | Time in ms a philosopher spends eating |
| `time_to_sleep` | Time in ms a philosopher spends sleeping |
| `number_of_meals` | Optional: stop after each philosopher eats this many times |

### Examples

```bash
# 1 philosopher dies after 800ms (can't eat with 1 fork)
./philo 1 800 200 200

# 5 philosophers survive indefinitely
./philo 5 800 200 200

# 5 philosophers, each eats at least 7 times then stops
./philo 5 800 200 200 7

# 4 philosophers with tight timing — someone dies
./philo 4 310 200 100
```

## Output Format

```
<timestamp_ms> <philosopher_id> <action>
```

Actions: `has taken fork`, `is eating`, `is sleeping`, `is thinking`, `died`

## Implementation

### Mandatory (`philo/`)

- Each philosopher runs in a **separate thread**
- **Mutexes** protect each fork from concurrent access
- A **monitor thread** periodically checks if any philosopher has exceeded `time_to_die` since their last meal
- Even-numbered philosophers start with a slight delay (`usleep(50)`) to prevent deadlock
- Mutexes for `print_lock` and `data_lock` ensure race-free logging and state access

### Bonus (`philo_bonus/`)

- Each philosopher runs in a **separate process** (via `fork`)
- **Named semaphores** replace mutexes for fork management and synchronization
- Each child process spawns a **monitor thread** to detect death or meal completion
- An `eat` semaphore limits concurrent eaters to `N / 2` as a deadlock prevention strategy
- The parent process waits for children using `waitpid` and handles cleanup

## Known Issues

### Bonus Part

The bonus implementation is **not fully correct** and has the following known issues:

1. **Semaphore double-close** (`process_bonus.c:end_philos`): `free_data` is called for each philosopher in a loop, but all share the same `t_data` pointer, causing `sem_close` to be called multiple times on the same semaphore — undefined behavior.

2. **Semaphore leak race** (`routine_bonus.c:eating`): If the monitor signals simulation stop between the fork-acquisition check and the stop check in `eating`, the acquired semaphores (forks and eat) are never released, eventually blocking all philosophers.

3. **Mutex destroy on failed init** (`init_bonus.c:create_philos`): `pthread_mutex_destroy` is called on a mutex whose `pthread_mutex_init` has just failed — undefined behavior.

4. **Signal harshness** (`process_bonus.c:end_philos`): Uses `SIGKILL` instead of `SIGTERM`, preventing child processes from performing any cleanup.
