#ifndef DIAMETER_MESSAGE_TYPES_H
#define DIAMETER_MESSAGE_TYPES_H

enum DiameterMessageType
  {
    CER = 257,
    CEA = 257,
    RAR = 258,
    RAA = 258,
    ACR = 271,
    ACA = 271,
    ASR = 274,
    ASA = 274,
    STR = 275,
    STA = 275,
    DWR = 280,
    DWA = 280,
    DPR = 282,
    DPA = 282,
// RFC 4006
    CCR = 272,
    CCA = 272
  };

#endif
