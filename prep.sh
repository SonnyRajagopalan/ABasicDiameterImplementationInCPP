#!/bin/bash
# prep.sh -- Clean up stale processes and POSIX IPC objects left from
#            previous runs of the Diameter stack (test executables, etc).
# Run this before starting the server.

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
echo "=== prep.sh: cleaning stale Diameter resources ==="

# 1. Kill any leftover test/daemon processes by exact basename match
#    Use the full path to avoid matching unrelated processes like sshd, Xorg, etc.
for proc in oneShotTimerTest periodicTimerTest; do
    # Match exact basename; ignore our own grep/pkill
    if pgrep -x "$proc" >/dev/null 2>&1; then
        pkill -9 -x "$proc" 2>/dev/null
        echo "Killed stale process: $proc"
    fi
done
# Kill server/client only when they match the Diameter binary specifically
for bin in "$PROJECT_DIR/server" "$PROJECT_DIR/client"; do
    if [ -x "$bin" ]; then
        pkill -9 -f "^$bin" 2>/dev/null && echo "Killed stale: $bin" || true
    fi
done

# 2. Remove POSIX shared memory objects (under /dev/shm)
for shm in /dev/shm/sem.timerSemaphore \
           /dev/shm/sem.testCallbackSemaphore \
           /dev/shm/sem.heapSemaphore \
           /dev/shm/timerMinheapPool \
           /dev/shm/timerObjectPool \
           /dev/shm/testCallbackPool; do
    if [ -e "$shm" ]; then
        rm -f "$shm"
        echo "Removed: $shm"
    fi
done

# 3. Remove POSIX message queue (under /dev/mqueue)
if [ -e /dev/mqueue/toMain ]; then
    rm -f /dev/mqueue/toMain
    echo "Removed: /dev/mqueue/toMain"
fi

echo "=== prep.sh done ==="
