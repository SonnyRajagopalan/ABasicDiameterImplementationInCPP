# Tutorial: Exploring the Diameter Codebase with MCP

This tutorial teaches you how to use codebase-memory and conversational-memory
MCP tools by exploring a real C++ Diameter protocol implementation.

## Setup

```bash
# Index the project into the knowledge graph
make index

# Or manually:
# codebase-memory_mcp index_repository \
#   --repo_path /path/to/diameter/working \
#   --mode moderate --persistence true
```

## Step 1: High-Level Orientation

```python
# What modules exist?
get_architecture(project="diameter-project")
```

This reveals three clusters:
- **Platform** — timer management, IPC primitives (semaphores, shared memory, MQ)
- **Network** — TCP sockets, Diameter server/client
- **Base** — peer FSM, 8 states, ~25 event types

## Step 2: Find a Specific Class

```python
# I know there's a timer heap, but what's it called?
search_graph(project="diameter-project",
             name_pattern=".*MinHeap.*")
# Returns: TimerMinHeap (class), TimerMinHeap.h, TimerMinHeap.cpp
```

## Step 3: Trace a Call Chain

```python
# How does a timer get scheduled?
trace_path(project="diameter-project",
           function_name="Timer::scheduleOneShotAtTimeout",
           direction="outbound",
           depth=3)
```

Follows the path: `scheduleOneShotAtTimeout` → `schedule` → `TimerMinHeap::addToHeap` → `upHeapify`

## Step 4: Cross-Process Tracing

```python
trace_path(project="diameter-project",
           function_name="TimerTask::run",
           direction="outbound",
           depth=4)
```

Shows the child process path: `run()` → `getInstance()` → peek root → `removeFromHeap()` → `mq_send()`

## Step 5: Custom Graph Queries

```python
query_graph(project="diameter-project",
            query="""
            MATCH (f:Function)-[:CALLS]->(g:Function)
            WHERE f.name CONTAINS 'schedule'
            RETURN f.name, g.name
            """)
```

Returns every function called by any "schedule" function.

## Step 6: Log Your Progress

```python
conversational_memory.create_entities(entities=[{
    "name": "my-exploration-session",
    "entityType": "StudySession",
    "observations": [
        "explored: timer scheduling path",
        "explored: FSM state pattern",
        "found: 7 timer-related classes",
    ]
}])
```

## Step 7: Run the Tests

```bash
python test/runner.py --all --conversational
```

The `--conversational` flag logs results to the knowledge graph so you can
track pass rates over time.

## Step 8: Impact Analysis

After editing a file, check what's affected:

```python
detect_changes(project="diameter-project",
               depth=2)
```

## What You've Learned

- `search_graph` — find code by name pattern
- `trace_path` — follow call chains across processes
- `get_architecture` — discover module boundaries
- `query_graph` — custom Cypher for complex questions
- `conversational-memory` — log sessions as a knowledge graph
- `detect_changes` — understand impact before committing
- `search_code` — graph-enriched text search
- `get_code_snippet` — read function bodies without opening files
