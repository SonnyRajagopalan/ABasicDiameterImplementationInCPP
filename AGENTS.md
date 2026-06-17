# codebase-memory-mcp: Retrospective & Quick Reference

This file documents what happened during development sessions. For a
step-by-step tutorial on using MCP tools with this codebase, see
[TUTORIAL.md](TUTORIAL.md). For per-tool examples, see
[.codebase-memory/EXPLORING_THIS_CODEBASE.md](.codebase-memory/EXPLORING_THIS_CODEBASE.md).

## Session 2 Progress: June 2026

### Goal
Clean up the test portfolio, fix timer bugs, and build a reliable timer accuracy
stress tool with statistical jitter reporting.

### Done
- Fixed `test/oneShotTimerTest.cpp` (was `timerPoolTest`) segfault
- Renamed `timerPoolTest` → `oneShotTimerTest`
- Deleted extraneous tests
- Rewrote `test/timerAccuracyTest.cpp` as full stress-test tool
- Stripped diagnostic `cout` from platform files
- Fixed MQ drain at test exit
- Added heap semaphore for cross-process timer heap synchronization

### Known Issues
- POSIX message queue `/toMain` capacity is 10 messages (kernel default)
- Increase with: `echo 100 > /proc/sys/fs/mqueue/msg_max` (requires root)

### Critical Context
- `MAX_NUMBER_OF_TIMERS = 1024`
- TimerTask forks a child process; shared memory connects parent and child
- Lock ordering: `pTimerSemaphore` → `pHeapSemaphore` — no deadlock
- `AccuracyCallback` in `timerAccuracyTest` is heap-allocated with `new`

### Quick Reference

| Task | Tool |
|------|------|
| Find classes by name | `search_graph(name_pattern=...)` |
| Who calls X? | `trace_path(direction="inbound")` |
| What does X call? | `trace_path(direction="outbound")` |
| High-level architecture | `get_architecture()` |
| Impact of a change | `detect_changes()` |
| Log test results | `conversational-memory tools` |
| Full tutorial | `TUTORIAL.md` |
| MCP tool examples | `.codebase-memory/EXPLORING_THIS_CODEBASE.md` |
