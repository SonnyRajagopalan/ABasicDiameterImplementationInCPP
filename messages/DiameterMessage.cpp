#include "messages/DiameterMessage.h"

DiameterMessage::DiameterMessage (UINT8 _version, 
				  UINT32 _messageLength,
				  UINT8 _flags,
				  UINT32 _commandCode,
				  UINT32 _applicationId,
				  UINT32 _hopByHopId,
				  UINT32 _endToEndId)
{
  setVersion (_version);
  setMessageLength (_messageLength);
  setFlags (_flags);
  setCommandCode (_commandCode);
  setApplicationId (_applicationId);
  setHopByHopId (_hopByHopId);
  setEndToEndId (_endToEndId);
}
