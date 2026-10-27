*This project has been created as part of the 42 curriculum by edsole-a.*

# Codexion

## Description

Codexion simulates N coders sitting in a circle with N USB dongles, one
between each pair of neighbours. A coder needs both adjacent dongles to
compile, then debugs, then refactors, and must start compiling again within
`time_to_burnout` ms of its previous compile start or it burns out and the
simulation stops. The goal is to schedule access to the dongles so that
nobody deadlocks or starves, with POSIX threads and mutexes only.

One thread per coder, plus one monitor thread that detects burnout and the
end of the simulation. No global variables: all shared state lives in `t_sim`.

## Instructions

    make # builds ./codexion
    	./codexion
		number_of_coders
		time_to_burnout
		time_to_compile
        time_to_debug
		time_to_refactor
		number_of_compiles_required
		dongle_cooldown
		scheduler # scheduler: fifo | edf
    make clean | fclean | re

Example:

    $ ./codexion 3 800 100 100 100 2 10 fifo
    0 1 has taken a dongle
    0 1 has taken a dongle
    0 1 is compiling
    100 1 is debugging
    110 2 has taken a dongle
    110 2 has taken a dongle
    110 2 is compiling
    ...

    $ ./codexion 1 200 100 100 100 3 10 fifo    # one dongle: cannot compile
    201 1 burned out

Invalid input (negative, non-integer, overflow, unknown scheduler, wrong
argument count) prints an error on stderr and exits with status 1.

## Blocking cases handled

- **Deadlock (Coffman's circular wait).** Coder i uses dongles i and
  (i+1) mod n. If everybody took "left then right" they could all hold one
  dongle and wait forever. Instead every coder requests its dongles in
  ascending index order; only the last coder's pair wraps around, so it
  requests dongle 0 first. A global order on resources removes circular
  wait. With one coder there is one dongle, so it can never compile and
  burns out, as the subject implies.
- **Starvation.** Each dongle has its own waiting queue. A request is
  granted only to the head of the queue, ordered by arrival (`fifo`) or by
  burnout deadline `last_compile_start + time_to_burnout` (`edf`), so a
  waiting coder cannot be overtaken indefinitely.
- **Cooldown.** On release a dongle records `cooldown_until`; a request is
  only granted once `now >= cooldown_until`. Nobody signals when a cooldown
  ends, so waiters re-check with `pthread_cond_timedwait` (5 ms cap) instead
  of sleeping until a broadcast that may never come.
- **Precise burnout detection.** The monitor checks every deadline each
  millisecond. The burnout line is printed and the stop flag set in the same
  critical section (`log_burnout`), so no other line can follow it.
- **Prompt shutdown.** Stopping sets the flag and broadcasts every dongle
  condition variable. Phase sleeps (`sleep_ms`) poll the flag every 0.5 ms,
  so a coder in a long phase exits immediately instead of finishing it.
- **Log serialization.** All output goes through `log_msg` under `log_lock`,
  one `printf` per line.
- **Resource failure.** `malloc`/`pthread_create` failures are checked,
  only created threads are joined, and the error is reported.

## Thread synchronization mechanisms

| Primitive | Protects |
|---|---|
| `d_mutex` (per dongle) | `held`, `cooldown_until`, the waiting queue |
| `cond` (per dongle) | sleeping until a dongle may be granted (broadcast on release, `timedwait` for cooldown expiry) |
| `log_lock` | stdout, the `stopped` flag, and each coder's `last_compile_start` / `compile_count` (also read by the monitor) |
| `priority_lock` | the global FIFO arrival counter |

Lock order is always `d_mutex` then `priority_lock` or `log_lock`; the
monitor never holds `log_lock` while taking a `d_mutex`, so no lock cycle
exists. `pthread_cond_timedwait` releases `d_mutex` atomically, so a release
that broadcasts cannot be missed between the check and the sleep. The
coder/monitor channel is the stop flag, only ever read through
`sim_is_stopped` and written under `log_lock`.

Each dongle's waiting queue is an array-backed binary min-heap. The key is
the arrival counter (`fifo`) or the burnout deadline (`edf`), ties broken
by coder id. `push` appends and sifts up, `pop` moves the last element to
the root and sifts down, `remove` (used when the simulation stops while a
coder is waiting) replaces the element with the last one and sifts both
ways. Push and pop are O(log k) and peek is O(1); the heap is only ever
touched while holding that dongle's `d_mutex`.

## Testing

Manual scenarios (feasible runs, forced burnouts, 1 to 200 coders, both
schedulers), valgrind and ThreadSanitizer/AddressSanitizer builds, and the
third-party `pacodexion` suite (34/38). Its 4 failures are not program
faults: three print more than 64 KB while the tester only drains the pipe
after the process exits, and `large_edf` needs at least 14.6 s against the
tester's 15 s limit.

## Resources

- CodeVault on YouTube
- geeksforgeeks
- `man 7 pthreads`, `pthread_cond_timedwait(3)`, `gettimeofday(2)`
- Dijkstra, dining philosophers; Coffman et al., "System Deadlocks" (1971)
- Liu & Layland, EDF scheduling (1973)

AI usage: *AI was used to explain concepts, create tests, and redact this README.*