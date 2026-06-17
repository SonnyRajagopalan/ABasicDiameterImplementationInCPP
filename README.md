# Diameter Protocol Implementation

A C++ implementation of the Diameter protocol (RFC 3588 / RFC 4006) with
portable POSIX IPC primitives, timer management, and peer state machine.

## Prerequisites

```
libuuid-dev      (bundled as uuid-dev_2.41.3-3ubuntu2_amd64.deb)
g++              (with C++11 support)
pthreads
librt            (POSIX real-time extensions)
```

Ensure local include/library path has the uuid headers and `.a`/`.so`:

```
ls /tmp/local_include/libuuid.a    # should exist
ls /tmp/local_include/uuid/uuid.h  # should exist
```

The local library path defaults to `/tmp/local_include`. Override with the
`LOCAL_INCLUDE_DIR` environment variable if your uuid installation lives
elsewhere.

## Build

All Makefiles use `PROJECT_HOME` to locate source and include directories.
It defaults to the current directory — just run `make` from the project root:

```bash
make all      # Build all targets
make server   # Build only the server
make oneShotTimerTest  # One-shot timer test
make periodicTimerTest # Periodic timer test
make timerAccuracyTest # Timer accuracy stress test
make testserver        # Diameter protocol test server
make cerCeaTest        # CER/CEA handshake test
make avpMixTest        # All AVP types test
make groupedAvpTest    # Nested Grouped AVP test
make index             # Index into codebase-memory knowledge graph (requires MCP)
make clean             # Remove binaries and object files
```

To build from a different working directory, set `PROJECT_HOME`:

```bash
export PROJECT_HOME=/path/to/diameter
cd /some/other/dir && make -f $PROJECT_HOME/Makefile
```

## Quick Start

Clean stale IPC objects, then start the server:

```bash
./prep.sh
./server
```

The server listens on port 3868.  All timer tests are self-contained
and do not require the server to be running.  Diameter protocol tests
(`testServer`, `cerCeaTest`, `avpMixTest`, `groupedAvpTest`) require the
server to be running.

## Cleanup

```bash
./prep.sh          # Kills stale processes, removes shared memory and message queues
```

---

## 1. Diameter Server (`server`)

**Source:** `test/server.cpp` + `net/DiameterServer.cpp`

Listens on TCP port 3868 (the IANA-assigned Diameter port), accepts client
connections in an infinite loop. For each connection, creates/looks up a
`DiameterPeer` entry by the remote IP, reads raw bytes, deserializes a
`DiameterHeader` + `DiameterAvpList`, and dumps the data to `/tmp/server.bin`.

**Expects:** clean IPC state (`./prep.sh` first).
Runs until Ctrl-C. To change the port, edit `include/common/DiameterConstants.h`.

---

## 2. One-Shot Timer Test (`oneShotTimerTest`)

**Source:** `test/oneShotTimerTest.cpp`

One-shot timers: spawns a background thread on a POSIX message queue, forks a
`TimerTask` child that manages a min-heap in shared memory. Allocates N one-shot
timers at t, 2t, ..., N*t ms. Cancels the timer at index 2.

```bash
./prep.sh
./oneShotTimerTest -c <count> -t <timeout_ms>
```
- `-c <count>` — number of one-shot timers (required, max 14)
- `-t <timeout_ms>` — base timeout (required)

**Expects:** clean IPC state (`./prep.sh` first). Self-contained; no server needed.
Examples: `-c 5 -t 1000` (5 timers at 1s, 2s, ..., 5s).
Ctrl-C to stop; run `./prep.sh` afterwards.

---

## 3. Periodic Timer Test (`periodicTimerTest`)

**Source:** `test/periodicTimerTest.cpp`

Periodic repeating timers: creates a timer at `t` ms that fires `c` times,
then a second at `10*t` ms that fires `c` times. The message queue thread
re-schedules via `changeFiringTime()` until fired count is met.

```bash
./prep.sh
./periodicTimerTest -c <count> -t <timeout_ms>
```
- `-c <count>` — number of times each timer fires (required)
- `-t <timeout_ms>` — base timeout (required)

**Expects:** clean IPC state (`./prep.sh` first). Self-contained; no server needed.
Example: `-c 5 -t 2000` (2s timer fires 5 times, then 20s timer fires 5 times).
Ctrl-C to stop; run `./prep.sh` afterwards.

---

## 4. Timer Accuracy Test (`timerAccuracyTest`)

**Source:** `test/timerAccuracyTest.cpp`

Stress test with microsecond jitter measurement. Supports one-shot and
periodic modes, statistical reporting (min/max/mean/stddev/p50/p90/p95/p99),
and direction breakdown (early/on-time/late).

```bash
./prep.sh
# One-shot mode: -c <count> -t <timeout_ms>
./timerAccuracyTest -c 10 -t 500

# Periodic mode: -c <count> -p <period_ms> -f <times> -n <total_fires>
./timerAccuracyTest -c 100 -p 100 -f 10 -n 1000

# Quiet mode (machine-readable stats only):
./timerAccuracyTest -q -c 10 -t 500
```
- `-c <count>` — number of timers (required, max 1000)
- `-t <timeout_ms>` — base timeout for one-shot mode
- `-p <period_ms>` — period for periodic mode
- `-f <times>` — number of fires per timer (periodic mode)
- `-n <total_fires>` — exit after N total fires (periodic mode)
- `-q` — quiet: print stats only (no per-timer output)

**Expects:** clean IPC state (`./prep.sh` first). Self-contained; no server needed.
Ctrl-C to stop; run `./prep.sh` afterwards.

### Example Output
```
=== Timer Accuracy Results ===
Timers:   10  |  Mode: ONE_SHOT
Expected: 500 ms per timer
Total fires: 10 / 10 (100.0%)

Jitter (ms):
  Min:  -0.012 |  Max:  0.187  |  Mean:  0.084
  Stddev: 0.061
  P50:   0.091 |  P90:  0.152  |  P95:  0.170  |  P99:  0.184

Direction:
  Early:  0 (0.0%)  |  On-time:  0 (0.0%)  |  Late:  10 (100.0%)
```

---

## 5. IPC Objects Cleaned By `prep.sh`

```
/dev/shm/sem.*              POSIX semaphores
/dev/shm/timer*             Timer shared memory pools
/dev/shm/test*              TestCallback shared memory pools
/dev/mqueue/*               POSIX message queues
```

---

## 6. Architecture Overview

```
                     ┌─────────────────────┐
                     │  DiameterServer     │  TCP port 3868
                     │  (accept loop)      │
                     └──────────┬──────────┘
                                │ new connection
                     ┌──────────▼──────────┐
                     │  DiameterPeer       │
                     │  (FSM per peer)     │
                     │  + DiameterPeerDb   │
                     └─────────────────────┘

                     ┌─────────────────────┐
                     │  TimerTask (child)  │  manages min-heap
                     │  while(1) loop      │  fires timers, sends
                     │                     │  messages to /toMain
                     └──────────┬──────────┘
                                │ POSIX message queue
                     ┌──────────▼──────────┐
                     │  getMessages thread │  receives expiry msgs
                     │  (parent process)   │  executes callbacks
                     └─────────────────────┘

IPC primitives (all use POSIX IPC):
  - /dev/shm/timerObjectPool       Timer object memory pool
  - /dev/shm/testCallbackPool      TestCallback memory pool
  - /dev/shm/timerMinheapPool      Timer min-heap (shared)
  - /dev/shm/sem.timerSemaphore    Timer pool mutex
  - /dev/shm/sem.testCallbackSemaphore  Callback pool mutex
  - /dev/mqueue/toMain             Timer expiry message queue
```

### Directory Layout

```
include/          Header files (avps, base, common, logging, messages, net,
                  platform, test)
├── avps/         Diameter AVP classes (Unsigned32, OctetString, Grouped, etc.)
├── base/         Peer management, FSM states
├── common/       Types, Util, DiameterConstants, ResultCodes
├── logging/      Diagnostic logging
├── messages/     DiameterHeader, DiameterMessage, DiameterCER
├── net/          Socket, ServerSocket, ClientSocket, DiameterServer/Client
├── platform/     Timer, TimerTask, MemoryPool, Semaphore, MessageQueue
└── test/         TestCallback, PooledObject, SimpleObject

avps/             AVP source files
base/             Peer, PeerDb, FSM state source files
common/           Utility source files
helpers/          Perl scripts, RFC 3588/4006 text, utility scripts
logging/          Logging source files
messages/         Message source files
net/              Network source files
platform/         Timer, MemoryPool, semaphore, etc. source files
test/             Test programs
```

---

## 7. Python Test Runner

A Python test runner is available for automated execution of all timer tests:

```bash
python test/runner.py                     # Run all timer tests with defaults
python test/runner.py --one-shot           # Only one-shot tests
python test/runner.py --periodic           # Only periodic tests
python test/runner.py --accuracy           # Only accuracy tests
python test/runner.py --all --junit        # JUnit XML output
python test/runner.py --help               # Full usage
```

Requires: `python3`, no extra packages.

---

## 8. Automatic Knowledge Graph Indexing

If you have the codebase-memory MCP tools available:

```bash
make index
```

This indexes the project into the knowledge graph, enabling `search_graph`,
`trace_path`, and `get_architecture` queries. The artifact is persisted at
`.codebase-memory/graph.db.zst` for team sharing.

---

## 9. Docker

```bash
docker build -t diameter-study .
docker run --rm diameter-study
```

Builds in Ubuntu 22.04 with all dependencies. The `runner` stage produces
a minimal image (~8 MB + libuuid).

---

## 10. Troubleshooting

### "Fatal: PosixSemaphore failed" or "Assertion failed" on startup

Stale IPC objects from a previous run. Run cleanup:

```bash
./prep.sh
```

If the problem persists, manually remove:

```bash
rm -f /dev/shm/sem.* /dev/shm/timer* /dev/shm/test* /dev/mqueue/*
```

### Stale `oneShotTimerTest` / `periodicTimerTest` processes in `ps`

Kill them by name:

```bash
pkill -9 -x oneShotTimerTest
pkill -9 -x periodicTimerTest
```

Or use `prep.sh`.

### "mq_send failed: errno=28 (No space left on device)"

The POSIX message queue limit is exhausted. Check and raise:

```bash
cat /proc/sys/fs/mqueue/msg_max
```

### Port 3868 already in use

The previous server instance didn't shut down cleanly. Kill it:

```bash
pkill -9 -x server
```
