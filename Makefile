include ./_include.mk

BACKUP_DIR ?= ./backup

all: obj oneShotTimerTest periodicTimerTest timerAccuracyTest memoryPoolTest fsmStateTest server \
	testserver cerCeaTest avpMixTest groupedAvpTest

server: test/server.cpp obj
	@echo "Now, will do the server ..."
	$(CPP) $(CPPFLAGS) $(INCLUDE) avps/*.o base/*.o common/*.o logging/*.o messages/*.o net/*.o platform/*.o test/server.cpp -o server $(LIB)

obj:
	for dir in $(DIRS); do (cd $$dir; make $1 || exit 1) || exit 1; done # DO NOT DELETE

oneShotTimerTest: test/oneShotTimerTest.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) test/oneShotTimerTest.cpp platform/MemoryPool.o test/TestCallback.o \
	platform/Timer.o platform/TimerCallback.o platform/PosixSharedMemory.o platform/PosixSemaphore.o \
	platform/Runnable.o \
	platform/TimerUtils.o platform/TimerMinHeap.o common/Util.o \
	platform/TimerTask.o platform/PosixMessageQueue.o platform/TimerExpiryMessage.o -o oneShotTimerTest $(LIB)

periodicTimerTest: test/periodicTimerTest.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) test/periodicTimerTest.cpp platform/MemoryPool.o test/TestCallback.o \
	platform/Timer.o platform/TimerCallback.o platform/PosixSharedMemory.o platform/PosixSemaphore.o \
	platform/Runnable.o \
	platform/TimerUtils.o platform/TimerMinHeap.o common/Util.o \
	platform/TimerTask.o platform/PosixMessageQueue.o platform/TimerExpiryMessage.o -o periodicTimerTest $(LIB)

timerAccuracyTest: test/timerAccuracyTest.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) test/timerAccuracyTest.cpp platform/MemoryPool.o test/TestCallback.o \
	platform/Timer.o platform/TimerCallback.o platform/PosixSharedMemory.o platform/PosixSemaphore.o \
	platform/Runnable.o \
	platform/TimerUtils.o platform/TimerMinHeap.o common/Util.o \
	platform/TimerTask.o platform/PosixMessageQueue.o platform/TimerExpiryMessage.o -o timerAccuracyTest $(LIB)

testserver: test/testServer.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) avps/*.o base/*.o common/*.o logging/*.o \
	messages/*.o net/*.o platform/*.o test/testServer.cpp -o testServer $(LIB)

cerCeaTest: test/cerCeaTest.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) avps/*.o base/*.o common/*.o logging/*.o \
	messages/*.o net/*.o platform/*.o test/cerCeaTest.cpp -o cerCeaTest $(LIB)

avpMixTest: test/avpMixTest.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) avps/*.o base/*.o common/*.o logging/*.o \
	messages/*.o net/*.o platform/*.o test/avpMixTest.cpp -o avpMixTest $(LIB)

memoryPoolTest: test/memoryPoolTest.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) test/memoryPoolTest.cpp platform/MemoryPool.o \
	platform/PosixSharedMemory.o platform/PosixSemaphore.o -o memoryPoolTest $(LIB)

fsmStateTest: test/fsmStateTest.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) test/fsmStateTest.cpp avps/*.o base/*.o common/*.o \
	logging/*.o messages/*.o net/*.o platform/*.o test/TestCallback.o test/PooledObject.o \
	-o fsmStateTest $(LIB)

groupedAvpTest: test/groupedAvpTest.cpp obj
	$(CPP) $(CPPFLAGS) $(INCLUDE) avps/*.o base/*.o common/*.o logging/*.o \
	messages/*.o net/*.o platform/*.o test/groupedAvpTest.cpp -o groupedAvpTest $(LIB)

tar:
	tar -cvjf diam-$(BUILDID).tgz \
	_include.mk Makefile README.md \
	include/apps/*.h  include/avps/*.h  include/base/*.h  include/common/*.h  \
	include/logging/*.h  include/messages/*.h  include/net/*.h  include/platform/*.h  include/test/*.h \
	avps/*.cpp  base/*.cpp  common/*.cpp  helpers/*  logging/*.cpp  messages/*.cpp  net/*.cpp  platform/*.cpp  test/*.cpp \
	avps/Makefile  base/Makefile  common/Makefile logging/Makefile  messages/Makefile  net/Makefile  platform/Makefile  test/Makefile

docs:
	doxygen Doxyfile

index:
	@echo "Indexing project into codebase-memory knowledge graph..."
	codebase-memory_mcp index_repository --repo_path $(PROJECT_HOME) --mode moderate --persistence true
	@echo "Done. Graph available via search_graph/trace_path tools."

clean:
	rm -rf ./server ./oneShotTimerTest ./periodicTimerTest \
	./timerAccuracyTest ./memoryPoolTest ./fsmStateTest \
	./testServer ./cerCeaTest ./avpMixTest ./groupedAvpTest \
	avps/*.o  base/*.o  common/*.o logging/*.o  messages/*.o  net/*.o  platform/*.o  test/*.o \
	avps/*.depends  base/*.depends  common/*.depends logging/*.depends  messages/*.depends  net/*.depends  platform/*.depends  test/*.depends \
	avps/*.P  base/*.P  common/*.P logging/*.P  messages/*.P  net/*.P  platform/*.P  test/*.P
	-rm -f include/apps/*.h~  include/avps/*.h~  include/base/*.h~  include/common/*.h~  \
	include/logging/*.h~  include/messages/*.h~  include/net/*.h~  include/platform/*.h~  include/test/*.h~ \
	avps/*.*~  base/*.*~  common/*.*~ logging/*.*~  messages/*.*~  net/*.*~  platform/*.*~  test/*.*~
tags:
	./genTags.sh

backup: tar
	cp diam-$(BUILDID).tgz $(BACKUP_DIR)/
