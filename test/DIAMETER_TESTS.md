# Diameter Protocol Tests

These tests validate the Diameter protocol stack at the wire level. They
expect a server to be running (typically `testServer`) and perform real
TCP exchanges using CER/CEA handshake, AVP type coverage, and nested
Grouped AVP serialization.

## Prerequisites

All tests require the server to already be listening. Start the test
server in a separate terminal:

```
./testServer [port]
```

Default port is 3868. All client tests accept optional `[serverIp [port]]`
arguments, defaulting to `127.0.0.1:3868`.

---

## `testServer` — CER/CEA Responder

**Source:** `test/testServer.cpp`

Listens on a TCP port (default 3868). For each connection:
1. Receives raw bytes
2. Deserializes the Diameter header and AVP list
3. If it is a Capabilities-Exchange-Request (command code 257), sends back a
   Capabilities-Exchange-Answer with mandatory AVPs per RFC 3588 §5.3.2:
   - Result-Code (2001 = DIAMETER_SUCCESS)
   - Origin-Host, Origin-Realm, Host-IP-Address, Vendor-Id, Product-Name
4. Logs all received AVPs and connection state

Signal-safe: handles SIGINT/SIGTERM for graceful shutdown.

```
make testserver
./testServer [port]
```

---

## `cerCeaTest` — CER/CEA Handshake

**Source:** `test/cerCeaTest.cpp`

Connects to the server, sends a CER, and waits for the CEA response.
Validates:
- Command code is 257 (CEA)
- REQ flag is not set (it is an answer, not a request)
- At least one AVP is present

Exits 0 on success, 1 on failure.

```
make cerCeaTest
./cerCeaTest [serverIp [port]]
```

---

## `avpMixTest` — All AVP Types

**Source:** `test/avpMixTest.cpp`

Connects to the server and sends a Diameter message containing 14 AVPs
covering every AVP type:

| # | Type | AVP Code | Value |
|---|------|----------|-------|
| 1 | Unsigned32 | 266 (Vendor-Id) | 42 |
| 2 | Integer32 | 429 (Exponent) | -1 |
| 3 | Integer64 | 447 (Value-Digits) | 0x12345678 |
| 4 | Unsigned64 | 287 (Acct-Sub-Session-Id) | 0x87654321 |
| 5 | Float32 | 9999 (unknown) | 3.14 |
| 6 | Float64 | 9998 (unknown) | 2.71828 |
| 7 | OctetString | 33 (Proxy-State) | "test-user" |
| 8 | Address | 257 (Host-IP-Address) | "127.0.0.1" |
| 9 | Time | 55 (Event-Timestamp) | "20260101T000000Z" |
| 10 | UTF8String | 269 (Product-Name) | "AVP Mix Test" |
| 11 | DiamIdent | 264 (Origin-Host) | "test-client.example.com" |
| 12 | DiamURI | 293 (Destination-Host) | "aaa://server.example.com" |
| 13 | Enumerated | 273 (Disconnect-Cause) | 0 |
| 14 | Grouped | 297 (Experimental-Result) | 1 × Unsigned32 child |

Validates that all types can be serialized, transmitted, and deserialized
without crash. Exits 0 on success.

```
make avpMixTest
./avpMixTest [serverIp [port]]
```

---

## `groupedAvpTest` — Nested Grouped AVPs

**Source:** `test/groupedAvpTest.cpp`

Connects to the server and sends a Diameter message with a 3-level deep
nested Grouped AVP structure (plus a flat baseline AVP):

```
PROXY_INFO (284)                ← level 1
  ├─ VENDOR_SPECIFIC_APP_ID (260)  ← level 2
  │   ├─ EXPERIMENTAL_RESULT (297)   ← level 3
  │   │   ├─ RESULT_CODE (268)
  │   │   └─ PROXY_STATE (33)
  │   └─ VENDOR_ID (266)
  └─ PROXY_STATE (33)

ORIGIN_STATE_ID (278)           ← flat (level 0)
```

Uses `DiameterGroupedAvp::addAnAvp()` (not the list constructor) to ensure
correct length computation. Exits 0 on success.

```
make groupedAvpTest
./groupedAvpTest [serverIp [port]]
```

---

## Running All Tests

```
# Start server in background
./testServer 3870 &
SERVER_PID=$!
sleep 1

# Run clients
./cerCeaTest 127.0.0.1 3870 && echo "PASS"
./avpMixTest 127.0.0.1 3870 && echo "PASS"
./groupedAvpTest 127.0.0.1 3870 && echo "PASS"

# Cleanup
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
```

---

## References

- RFC 3588: Diameter Base Protocol
  - §3: Diameter Header
  - §4: AVP Header and data formats
  - §5.3.1: Capabilities-Exchange-Request
  - §5.3.2: Capabilities-Exchange-Answer
