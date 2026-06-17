#ifndef DIAMETER_AVP_LIST_H
#define DIAMETER_AVP_LIST_H

#include <vector>
#include "DiameterAvp.h"

class DiameterAvpList
{
 public:

  DiameterAvpList();
  ~DiameterAvpList();
  UINT32 getAvpListLength() { return totalAvpListLength; }
  UINT32 getNumberOfAvps () { return avpVector.size(); }
  void   addAvpToAvpList (const DiameterAvp*); // Add in the back

  void   serialize   (OutputStream& os);
  void   deserialize (InputStream& in);

 private:

  std::list<DiameterAvp*> deserializeGroupedAvp (UINT32& sizeOfData, InputStream& in, UINT32 depth);

  std::vector<DiameterAvp*> avpVector;
  UINT32 totalAvpListLength; // In bytes
};
#endif
