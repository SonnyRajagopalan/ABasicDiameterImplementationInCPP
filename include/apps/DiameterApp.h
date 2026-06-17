#ifndef DIAMETER_APP_H
#define DIAMETER_APP_H
/**
   Application ID assignments. See 
   http://www.iana.org/assignments/aaa-parameters/aaa-parameters.xml#aaa-parameters-45
   for details.
*/
typedef enum __DiameterApp
  {
    ThreeGPPGx = 16777238,
    ThreeGPPRx = 16777229
  } DiameterApp;

#endif
