#ifndef DIAMETER_CER_H
#define DIAMETER_CER_H

#include "DiameterMessage.h"
#include "avps/DiameterDictionary.h"

class DiameterCER : public DiameterMessage
{
 public:
  DiameterCER ();
  ~DiameterCER ();
  DiameterCER (UINT8 _version,
	       UINT8 _flags,
	       UINT32 _applicationId,
	       UINT32 _hopByHopId,
	       UINT32 _endToEndId);

  void serialize (OutputStream& os);
  void deserialize (InputStream& in);
  
 private:

  DiameterDictionary* dict;
  void initialize (); 
  void updateMessageLength (DiameterAvp*);
};
#endif
