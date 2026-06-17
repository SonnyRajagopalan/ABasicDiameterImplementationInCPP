FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    libuuid-dev \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /diameter

COPY _include.mk Makefile ./
COPY include/ ./include/
COPY avps/ ./avps/
COPY base/ ./base/
COPY common/ ./common/
COPY logging/ ./logging/
COPY messages/ ./messages/
COPY net/ ./net/
COPY platform/ ./platform/
COPY test/ ./test/

RUN make all

FROM ubuntu:22.04 AS runner

RUN apt-get update && apt-get install -y --no-install-recommends \
    libuuid1 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /diameter/server /diameter/
COPY --from=builder /diameter/testServer /diameter/
COPY --from=builder /diameter/oneShotTimerTest /diameter/
COPY --from=builder /diameter/periodicTimerTest /diameter/
COPY --from=builder /diameter/timerAccuracyTest /diameter/
COPY --from=builder /diameter/cerCeaTest /diameter/
COPY --from=builder /diameter/avpMixTest /diameter/
COPY --from=builder /diameter/groupedAvpTest /diameter/
COPY --from=builder /diameter/prep.sh /diameter/

WORKDIR /diameter

CMD ["./server"]
