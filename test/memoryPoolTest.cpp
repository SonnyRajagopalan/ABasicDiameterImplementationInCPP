#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "platform/MemoryPool.h"

static int failures = 0;
static int tests = 0;

#define CHECK(cond, msg) do { \
    tests++; \
    if (!(cond)) { \
        std::cerr << "FAIL: " << msg << " (" << __LINE__ << ")" << std::endl; \
        failures++; \
    } else { \
        std::cout << "  PASS: " << msg << std::endl; \
    } \
} while(0)

const char* POOL_NAME = "memoryPoolTest";
const char* SHM_PATH = "/memoryPoolTestPool";

// All tests share one pool, created with O_EXCL.
// The destructor runs at exit (via atexit or cleanup).

static MemoryPool* gPool = NULL;

MemoryPool* get_pool() {
    return gPool;
}

void cleanup() {
    if (gPool) {
        delete gPool;
        gPool = NULL;
    }
}

void init_pool() {
    if (gPool) return;
    gPool = new MemoryPool(const_cast<char*>(POOL_NAME),
                           const_cast<char*>(SHM_PATH),
                           O_RDWR | O_CREAT | O_EXCL, PROT_READ | PROT_WRITE,
                           0666, 64, 100);
    atexit(cleanup);
}

// Test 1: Verify pool properties
void test_pool_properties() {
    std::cout << "\n--- Pool Properties ---" << std::endl;
    MemoryPool* pool = get_pool();

    CHECK(pool->getMaxNumberOfObjects() == 100, "max objects = 100");
    CHECK(pool->getObjectSize() == 64, "object size = 64");
    CHECK(pool->getNumberUsed() == 0, "initially 0 used");
}

// Test 2: Allocate and verify
void test_allocate() {
    std::cout << "\n--- Allocate Objects ---" << std::endl;
    MemoryPool* pool = get_pool();

    void* obj1 = pool->getFromPool();
    CHECK(obj1 != NULL, "allocate first object");
    CHECK(pool->getNumberUsed() == 1, "1 used after first alloc");

    void* obj2 = pool->getFromPool();
    CHECK(obj2 != NULL, "allocate second object");
    CHECK(pool->getNumberUsed() == 2, "2 used after second alloc");
    CHECK(obj1 != obj2, "two objects have different addresses");

    ptrdiff_t diff = static_cast<char*>(obj2) - static_cast<char*>(obj1);
    CHECK(diff == 64 || diff == -64, "objects are 64 bytes apart");
}

// Test 3: Index round-trip
void test_index_roundtrip() {
    std::cout << "\n--- Index Round-Trip ---" << std::endl;
    MemoryPool* pool = get_pool();

    void* obj = pool->getFromPool();
    CHECK(obj != NULL, "allocate for index test");

    UINT64 idx = pool->getIndexFromMemoryLocation(obj);
    CHECK(idx.uint64 != 0xFFFFFFFFFFFFFFFF, "index is valid");

    void* obj2 = pool->getMemoryLocationFromIndex(idx);
    CHECK(obj2 == obj, "round-trip index to location matches");
}

// Test 4: Free and re-allocate
void test_free_and_realloc() {
    std::cout << "\n--- Free and Re-Allocate ---" << std::endl;
    MemoryPool* pool = get_pool();

    void* obj = pool->getFromPool();
    CHECK(obj != NULL, "allocate for free test");
    CHECK(pool->getNumberUsed() != 0, "some used before free");

    UINT32 used_before = pool->getNumberUsed();
    pool->removeFromPool(obj);
    CHECK(pool->getNumberUsed() == used_before - 1, "count decreased after free");

    void* obj2 = pool->getFromPool();
    CHECK(obj2 != NULL, "re-allocate after free");
}

// Test 5: Pool exhaustion
void test_pool_exhaustion() {
    std::cout << "\n--- Pool Exhaustion ---" << std::endl;
    MemoryPool* pool = get_pool();

    // Drain everything already allocated (3 so far: test_allocate + test_index_roundtrip + test_free_and_realloc)
    UINT32 initial = pool->getNumberUsed();
    void* objects[100];
    UINT32 i;
    for (i = 0; i < 100 - initial; i++) {
        objects[i] = pool->getFromPool();
        if (objects[i] == NULL) break;
    }
    CHECK(i + initial == 100, "can exhaust all 100 slots");
    CHECK(pool->getNumberUsed() == 100, "100 used at exhaustion");
    CHECK(pool->getFromPool() == NULL, "NULL on exhausted pool");

    // Free one and verify we can allocate again
    pool->removeFromPool(objects[0]);
    CHECK(pool->getNumberUsed() == 99, "99 after one free");
    void* obj = pool->getFromPool();
    CHECK(obj != NULL, "allocate after free from exhausted");
}

// Test 6: Cross-process via fork
void test_cross_process() {
    std::cout << "\n--- Cross-Process (fork) ---" << std::endl;

    pid_t pid = fork();
    if (pid == 0) {
        // Child process inherits the parent's mmap'd shared memory via fork().
        // The parent's gPool pointer and its internal pShm mapping are both
        // valid in the child (MAP_SHARED is preserved across fork).
        void* obj = gPool->getFromPool();
        _exit(obj != NULL ? 0 : 2);
    } else {
        int status;
        waitpid(pid, &status, 0);
        bool child_ok = WIFEXITED(status) && WEXITSTATUS(status) == 0;
        CHECK(child_ok, "forked child can allocate from shared pool via inherited mapping");
    }
}

int main() {
    std::cout << "\n=== MemoryPool Test ===\n" << std::endl;

    init_pool();
    if (!gPool) {
        std::cerr << "FAILED to create memory pool" << std::endl;
        return 1;
    }

    test_pool_properties();
    test_allocate();
    test_index_roundtrip();
    test_free_and_realloc();
    test_cross_process();
    test_pool_exhaustion();

    cleanup();

    std::cout << "\n--- Results: " << tests << " tests, "
              << failures << " failures ---" << std::endl;
    return failures > 0 ? 1 : 0;
}
