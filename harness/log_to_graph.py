#!/usr/bin/env python3
"""
Coding Harness: Log Test Results to Knowledge Graph

Demonstrates how a test harness can integrate with both:
  - codebase-memory MCP (graph-based code queries)
  - conversational-memory MCP (session history as a knowledge graph)

Usage:
    # After running tests, log results to the knowledge graph:
    python harness/log_to_graph.py --results test/results.json

    # Query the knowledge graph for test history:
    python harness/log_to_graph.py --query "show me recent test sessions"

    # Annotate code entities with test coverage info:
    python harness/log_to_graph.py --annotate --entity Timer::scheduleOneShotAtTimeout
"""

import argparse
import json
import os
import sys
import time


def try_mcp_tool(tool_name, kwargs):
    """Attempt to call an MCP tool. Returns result or None if unavailable."""
    try:
        if tool_name.startswith("search_graph"):
            from codebase_memory import search_graph
            return search_graph(**kwargs)
        elif tool_name.startswith("add_observations"):
            from conversational_memory import add_observations
            return add_observations(**kwargs)
        elif tool_name.startswith("create_entities"):
            from conversational_memory import create_entities
            return create_entities(**kwargs)
        elif tool_name.startswith("trace_path"):
            from codebase_memory import trace_path
            return trace_path(**kwargs)
        else:
            return None
    except (ImportError, Exception) as e:
        print(f"[MCP] Tool '{tool_name}' unavailable: {e}")
        return None


def log_results(results_path):
    """Load test results JSON and log to conversational-memory."""
    if not os.path.exists(results_path):
        print(f"File not found: {results_path}")
        return

    with open(results_path) as f:
        data = json.load(f)

    session_id = f"test-session-{data.get('timestamp', int(time.time()))}"
    total = 0
    passed = 0
    obs = [
        f"timestamp: {data.get('timestamp', 'unknown')}",
        f"source: {results_path}",
    ]
    for suite in data.get("suites", []):
        for r in suite.get("results", []):
            total += 1
            if r.get("passed"):
                passed += 1
            obs.append(f"{r.get('name', 'unknown')}: {'PASS' if r.get('passed') else 'FAIL'} ({r.get('duration', 0):.1f}s)")
    obs.append(f"total: {total}")
    obs.append(f"passed: {passed}")
    obs.append(f"pass_rate: {passed/max(total,1):.0%}")

    result = try_mcp_tool("create_entities", {
        "entities": [{
            "name": session_id,
            "entityType": "TestSession",
            "observations": obs,
        }]
    })
    if result:
        print(f"[conversational-memory] Created entity '{session_id}' with {len(obs)} observations")
    else:
        print("[conversational-memory] MCP not available. Would have logged:")
        for o in obs:
            print(f"  {o}")


def query_graph(query):
    """Query the codebase-memory graph for test-relevant info."""
    result = try_mcp_tool("search_graph", {
        "project": os.path.basename(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
        "query": query,
    })
    if result:
        print(f"Graph results for '{query}':")
        for item in result.get("results", []):
            print(f"  {item.get('name', '?')} ({item.get('file', '?')})")
    else:
        print(f"[search_graph] Would search for: {query}")


def annotate_entity(entity_name, observations):
    """Add observations to an existing graph entity."""
    result = try_mcp_tool("add_observations", {
        "observations": [{
            "entityName": entity_name,
            "contents": observations,
        }]
    })
    if result:
        print(f"[conversational-memory] Added {len(observations)} observations to '{entity_name}'")
    else:
        print(f"[conversational-memory] Would annotate '{entity_name}' with:")
        for o in observations:
            print(f"  {o}")


def trace_code(function_name):
    """Trace callers/callees of a function."""
    result = try_mcp_tool("trace_path", {
        "function_name": function_name,
        "project": os.path.basename(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
        "direction": "both",
        "depth": 2,
    })
    if result:
        print(f"Call graph for '{function_name}':")
        for hop in result.get("path", []):
            print(f"  {hop}")
    else:
        print(f"[trace_path] Would trace: {function_name}")


def main():
    parser = argparse.ArgumentParser(description="Knowledge Graph Test Harness")
    parser.add_argument("--results", type=str, help="Path to test results JSON")
    parser.add_argument("--query", type=str, help="Query the codebase graph")
    parser.add_argument("--annotate", action="store_true",
                        help="Annotate a graph entity")
    parser.add_argument("--entity", type=str, help="Entity name to annotate",
                        default="Timer::scheduleOneShotAtTimeout")
    parser.add_argument("--trace", type=str, help="Trace a function's call graph")
    args = parser.parse_args()

    if args.results:
        log_results(args.results)

    if args.query:
        query_graph(args.query)

    if args.annotate:
        observations = [
            f"last_tested: {time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())}",
            "covered_by: timer test suite",
        ]
        annotate_entity(args.entity, observations)

    if args.trace:
        trace_code(args.trace)

    if not any([args.results, args.query, args.annotate, args.trace]):
        parser.print_help()


if __name__ == "__main__":
    main()
