#!/usr/bin/env python3
"""
Diameter Test Runner

Orchestrates all timer tests and protocol tests, collects pass/fail/jitter
stats, and optionally logs to conversational-memory knowledge graph.

Usage:
    python test/runner.py                       # Run all timer tests
    python test/runner.py --one-shot             # Only one-shot tests
    python test/runner.py --accuracy --periodic  # Specific test groups
    python test/runner.py --all --junit          # JUnit XML output
    python test/runner.py --conversational       # Log results to conversational-memory
"""

import argparse
import json
import os
import re
import signal
import subprocess
import sys
import time
import xml.etree.ElementTree as ET
from dataclasses import dataclass, field, asdict
from typing import Optional


PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TIMEOUT_DEFAULT = 120


@dataclass
class TestResult:
    name: str
    passed: bool
    duration: float
    details: str = ""
    jitter: Optional[dict] = None


@dataclass
class TestSuite:
    name: str
    results: list = field(default_factory=list)

    @property
    def passed(self):
        return all(r.passed for r in self.results)

    @property
    def total(self):
        return len(self.results)

    def to_junit(self):
        ts = ET.Element("testsuite", name=self.name, tests=str(self.total),
                        failures=str(self.total - sum(1 for r in self.results if r.passed)))
        for r in self.results:
            tc = ET.SubElement(ts, "testcase", name=r.name, time=f"{r.duration:.3f}")
            if not r.passed:
                ET.SubElement(tc, "failure", message=r.details)
            if r.jitter:
                props = ET.SubElement(tc, "properties")
                for k, v in r.jitter.items():
                    ET.SubElement(props, "property", name=k, value=str(v))
        return ts


def run_prep():
    subprocess.run([os.path.join(PROJECT_ROOT, "prep.sh")],
                   capture_output=True, timeout=30)


def build_all():
    result = subprocess.run(["make", "all"], cwd=PROJECT_ROOT,
                            capture_output=True, timeout=TIMEOUT_DEFAULT)
    return result.returncode == 0


def run_test(binary, args, timeout=TIMEOUT_DEFAULT, name=None) -> TestResult:
    bin_path = os.path.join(PROJECT_ROOT, binary)
    if not os.path.exists(bin_path):
        return TestResult(name=name or binary, passed=False, duration=0,
                          details=f"Binary not found: {bin_path} (build first)")

    start = time.time()
    try:
        proc = subprocess.run([bin_path] + args, capture_output=True,
                              timeout=timeout)
        duration = time.time() - start
        stdout = proc.stdout.decode(errors="replace")
        stderr = proc.stderr.decode(errors="replace")
        passed = proc.returncode == 0

        jitter = parse_jitter(stdout) if passed else None

        details = ""
        if not passed:
            details = (stdout + stderr)[:2000]

        return TestResult(name=name or binary, passed=passed,
                          duration=duration, details=details, jitter=jitter)
    except subprocess.TimeoutExpired:
        return TestResult(name=name or binary, passed=False,
                          duration=timeout,
                          details=f"Timed out after {timeout}s")
    except FileNotFoundError:
        return TestResult(name=name or binary, passed=False, duration=0,
                          details=f"Binary not found: {bin_path}")


JITTER_RE = re.compile(
    r"(?P<key>[A-Za-z0-9_ /%-]+):\s+(?P<value>[-0-9.]+)",
    re.MULTILINE
)


def parse_jitter(stdout: str) -> Optional[dict]:
    if "=== Timer Accuracy Results ===" not in stdout:
        return None
    result = {}
    for line in stdout.split("\n"):
        if ":" in line:
            parts = line.split(":", 1)
            key = parts[0].strip().rstrip()
            val = parts[1].strip()
            try:
                if "/" in val:
                    nums = [float(v.strip().split()[0]) for v in val.split("/")]
                    result[key] = val
                elif val.replace(".", "").replace("-", "").isdigit():
                    result[key] = float(val.split()[0])
                else:
                    result[key] = val
            except ValueError:
                result[key] = val
    return result if result else None


def get_timer_suites(args):
    suites = []

    if args.one_shot or args.all:
        suite = TestSuite("one_shot")
        variants = [
            ("OneShot (5×1s)", ["-c", "5", "-t", "1000"]),
        ]
        for name, cmd_args in variants:
            suite.results.append(
                run_test("oneShotTimerTest", cmd_args, name=name))
        suites.append(suite)

    if args.periodic or args.all:
        suite = TestSuite("periodic")
        variants = [
            ("Periodic (5×2s)", ["-c", "5", "-t", "2000"]),
        ]
        for name, cmd_args in variants:
            suite.results.append(
                run_test("periodicTimerTest", cmd_args, name=name))
        suites.append(suite)

    if args.accuracy or args.all:
        suite = TestSuite("accuracy")
        variants = [
            ("Accuracy OneShot 10×500ms",
             ["-c", "10", "-t", "500", "-q"]),
            ("Accuracy OneShot 50×200ms",
             ["-c", "50", "-t", "200", "-q"]),
        ]
        if args.stress:
            variants.append(
                ("Accuracy Periodic 100×100ms×10",
                 ["-c", "100", "-p", "100", "-f", "10", "-n", "1000", "-q"]))
        for name, cmd_args in variants:
            suite.results.append(
                run_test("timerAccuracyTest", cmd_args, name=name,
                         timeout=180))
        suites.append(suite)

    return suites


def print_report(suites):
    passed = sum(1 for s in suites for r in s.results if r.passed)
    total = sum(s.total for s in suites)
    print(f"\n{'='*60}")
    print(f"  TEST REPORT  ({passed}/{total} passed)")
    print(f"{'='*60}")
    for suite in suites:
        status = "PASS" if suite.passed else "FAIL"
        print(f"\n  [{status}] {suite.name} ({sum(1 for r in suite.results if r.passed)}/{suite.total})")
        for r in suite.results:
            sym = "+" if r.passed else "x"
            dur = f"{r.duration:.1f}s"
            print(f"    {sym} {r.name} ({dur})")
            if r.jitter:
                for k, v in r.jitter.items():
                    print(f"       {k}: {v}")
        if not suite.passed:
            for r in suite.results:
                if not r.passed and r.details:
                    print(f"    Details: {r.details[:200]}")
    print()

    return passed == total


def write_junit(suites, path):
    root = ET.Element("testsuites")
    for suite in suites:
        root.append(suite.to_junit())
    tree = ET.ElementTree(root)
    tree.write(path, encoding="utf-8", xml_declaration=True)
    print(f"JUnit XML written to {path}")


def log_to_conversational_memory(suites):
    """Log test results to conversational-memory knowledge graph.

    This function calls the conversational-memory MCP tools to record
    test session outcomes as observations on entities in the knowledge graph.
    It demonstrates how a coding harness can integrate with MCP.
    """
    try:
        from conversational_memory import add_observations, create_entities
    except ImportError:
        print("[conversational-memory] MCP tools not available in this context. Skipping.")
        return

    session_id = f"test-session-{int(time.time())}"
    entities = [
        {
            "name": session_id,
            "entityType": "TestSession",
            "observations": [
                f"timestamp: {time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())}",
                f"passed: {all(s.passed for s in suites)}",
                f"total_tests: {sum(s.total for s in suites)}",
                f"passed_tests: {sum(1 for s in suites for r in s.results if r.passed)}",
            ]
        }
    ]
    for suite in suites:
        for r in suite.results:
            obs = [
                f"passed: {r.passed}",
                f"duration: {r.duration:.3f}s",
            ]
            if r.jitter:
                obs.append(f"jitter_min_ms: {r.jitter.get('Min', 'N/A')}")
                obs.append(f"jitter_max_ms: {r.jitter.get('Max', 'N/A')}")
            entities.append({
                "name": f"{session_id}-{r.name}",
                "entityType": "TestResult",
                "observations": obs,
            })

    create_entities(entities=entities)
    print(f"[conversational-memory] Logged {sum(s.total for s in suites)} results as session '{session_id}'")


def main():
    parser = argparse.ArgumentParser(description="Diameter Test Runner")
    parser.add_argument("--all", action="store_true", help="Run all tests")
    parser.add_argument("--one-shot", action="store_true", help="Run one-shot timer tests")
    parser.add_argument("--periodic", action="store_true", help="Run periodic timer tests")
    parser.add_argument("--accuracy", action="store_true", help="Run timer accuracy tests")
    parser.add_argument("--stress", action="store_true", help="Include stress tests (accuracy only)")
    parser.add_argument("--skip-build", action="store_true", help="Skip build step")
    parser.add_argument("--skip-prep", action="store_true", help="Skip prep.sh cleanup")
    parser.add_argument("--junit", type=str, help="Write JUnit XML to path")
    parser.add_argument("--conversational", action="store_true",
                        help="Log results to conversational-memory knowledge graph")
    parser.add_argument("--json", type=str, help="Write JSON results to path")
    args = parser.parse_args()

    if not any([args.all, args.one_shot, args.periodic, args.accuracy]):
        args.all = True

    if not args.skip_prep:
        print("Cleaning IPC state...")
        run_prep()

    if not args.skip_build:
        print("Building...")
        if not build_all():
            print("Build failed. Aborting.")
            sys.exit(1)

    suites = get_timer_suites(args)
    all_passed = print_report(suites)

    if args.junit:
        write_junit(suites, args.junit)

    if args.json:
        data = {
            "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
            "suites": [
                {
                    "name": s.name,
                    "passed": s.passed,
                    "results": [asdict(r) for r in s.results]
                }
                for s in suites
            ]
        }
        with open(args.json, "w") as f:
            json.dump(data, f, indent=2, default=str)
        print(f"JSON results written to {args.json}")

    if args.conversational:
        log_to_conversational_memory(suites)

    sys.exit(0 if all_passed else 1)


if __name__ == "__main__":
    main()
