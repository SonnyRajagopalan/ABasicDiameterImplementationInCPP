#include <iostream>
#include <string.h>
#include "base/DPFSMBaseState.h"
#include "base/DPFSMClosedState.h"
#include "base/DPFSMClosingState.h"
#include "base/DPFSMROpenState.h"
#include "base/DPFSMIOpenState.h"
#include "base/DPFSMWaitConnAckState.h"
#include "base/DPFSMWaitICEAState.h"
#include "base/DPFSMWaitConnAckElectState.h"
#include "base/DPFSMWaitReturnsState.h"
#include "base/DiameterPeer.h"
#include "base/DiameterPeerEvent.h"
#include "base/DiameterPeerState.h"

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

void test_state_names() {
    std::cout << "\n--- State Names ---" << std::endl;

    DPFSMBaseState* closed = DPFSMClosedState::getInstance();
    CHECK(std::string(closed->getStateName()) == "Closed",
          "Closed state name");

    DPFSMBaseState* closing = DPFSMClosingState::getInstance();
    CHECK(std::string(closing->getStateName()) == "Closing",
          "Closing state name");

    DPFSMBaseState* ropen = DPFSMROpenState::getInstance();
    CHECK(std::string(ropen->getStateName()) == "ROpen",
          "ROpen state name");

    DPFSMBaseState* iopen = DPFSMIOpenState::getInstance();
    CHECK(std::string(iopen->getStateName()) == "IOpen",
          "IOpen state name");

    DPFSMBaseState* wca = DPFSMWaitConnAckState::getInstance();
    CHECK(std::string(wca->getStateName()) == "WaitConnAck",
          "WaitConnAck state name");

    DPFSMBaseState* wicea = DPFSMWaitICEAState::getInstance();
    CHECK(std::string(wicea->getStateName()) == "WaitICEA",
          "WaitICEA state name");

    DPFSMBaseState* wcae = DPFSMWaitConnAckElectState::getInstance();
    CHECK(std::string(wcae->getStateName()) == "WaitConnAckElect",
          "WaitConnAckElect state name");

    DPFSMBaseState* wr = DPFSMWaitReturnsState::getInstance();
    CHECK(std::string(wr->getStateName()) == "WaitReturns",
          "WaitReturns state name");
}

void test_peer_state_strings() {
    std::cout << "\n--- Peer State Strings ---" << std::endl;

    DiameterPeerState s_ro(DiameterPeerState::ROpen);
    CHECK(std::string(s_ro.getString()) == "ROpen", "ROpen as string");

    DiameterPeerState s_io(DiameterPeerState::IOpen);
    CHECK(std::string(s_io.getString()) == "IOpen", "IOpen as string");

    DiameterPeerState s_closed(DiameterPeerState::Closed);
    CHECK(std::string(s_closed.getString()) == "Closed", "Closed as string");

    DiameterPeerState s_closing(DiameterPeerState::Closing);
    CHECK(std::string(s_closing.getString()) == "Closing", "Closing as string");

    DiameterPeerState s_invalid((DiameterPeerState::DiameterPeerStateType)99);
    CHECK(std::string(s_invalid.getString()) == "INVALID_PEER_STATE",
          "Invalid state returns fallback string");
}

void test_event_strings() {
    std::cout << "\n--- Event Strings ---" << std::endl;

    DiameterPeerEvent ev_start(DiameterPeerEvent::Start);
    CHECK(std::string(ev_start.getString()) == "Start", "Start event");

    DiameterPeerEvent ev_stop(DiameterPeerEvent::Stop);
    CHECK(std::string(ev_stop.getString()) == "Stop", "Stop event");

    DiameterPeerEvent ev_timeout(DiameterPeerEvent::Timeout);
    CHECK(std::string(ev_timeout.getString()) == "Timeout", "Timeout event");

    DiameterPeerEvent ev_win(DiameterPeerEvent::WinElection);
    CHECK(std::string(ev_win.getString()) == "WinElection", "WinElection event");

    DiameterPeerEvent ev_inv((DiameterPeerEvent::DiameterPeerEventType)99);
    CHECK(std::string(ev_inv.getString()) == "INVALID_PEER_EVENT",
          "Invalid event returns fallback string");
}

void test_state_singletons() {
    std::cout << "\n--- State Singletons ---" << std::endl;

    DPFSMBaseState* c1 = DPFSMClosedState::getInstance();
    DPFSMBaseState* c2 = DPFSMClosedState::getInstance();
    CHECK(c1 == c2, "DPFSMClosedState is singleton");

    DPFSMBaseState* i1 = DPFSMIOpenState::getInstance();
    DPFSMBaseState* i2 = DPFSMIOpenState::getInstance();
    CHECK(i1 == i2, "DPFSMIOpenState is singleton");

    DPFSMBaseState* r1 = DPFSMROpenState::getInstance();
    DPFSMBaseState* r2 = DPFSMROpenState::getInstance();
    CHECK(r1 == r2, "DPFSMROpenState is singleton");
}

void test_state_peer_values() {
    std::cout << "\n--- State Peer Values ---" << std::endl;

    DPFSMBaseState* closed = DPFSMClosedState::getInstance();
    DiameterPeerState pstate = closed->getState();
    CHECK(pstate.getState() == DiameterPeerState::Closed,
          "Closed state has Closed peer state");

    DPFSMBaseState* ropen = DPFSMROpenState::getInstance();
    pstate = ropen->getState();
    CHECK(pstate.getState() == DiameterPeerState::ROpen,
          "ROpen state has ROpen peer state");

    DPFSMBaseState* iopen = DPFSMIOpenState::getInstance();
    pstate = iopen->getState();
    CHECK(pstate.getState() == DiameterPeerState::IOpen,
          "IOpen state has IOpen peer state");

    DPFSMBaseState* wca = DPFSMWaitConnAckState::getInstance();
    pstate = wca->getState();
    CHECK(pstate.getState() == DiameterPeerState::WaitConnAck,
          "WaitConnAck state has WaitConnAck peer state");
}

int main() {
    std::cout << "\n=== Diameter Peer FSM State Test ===\n" << std::endl;

    test_state_names();
    test_peer_state_strings();
    test_event_strings();
    test_state_singletons();
    test_state_peer_values();

    std::cout << "\n--- Results: " << tests << " tests, "
              << failures << " failures ---" << std::endl;
    return failures > 0 ? 1 : 0;
}
