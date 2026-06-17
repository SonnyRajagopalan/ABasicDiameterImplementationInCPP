#ifndef DIAMETER_DICTIONARY_H
#define DIAMETER_DICTIONARY_H

#include <map>

//#include "DiameterAvpTypes.h"
#include "DiameterAvp.h"

class DiameterDictionary
{
 public:

  static DiameterDictionary* getInstance ();
  ~DiameterDictionary ();
  inline DiameterAvp* findAvp (UINT32& _code);
  inline DiameterAvp::AvpType lookupAvpType (UINT32& _code);
  inline UINT32 getDictionarySize () { return entries.size (); }

 private:

  DiameterDictionary ();
  static DiameterDictionary* instance;
  void debugging (UINT32);
  std::map<UINT32, DiameterAvp*> entries;
  void init ();
  void addAvp (char* _name, UINT32 _code, DiameterAvp::AvpType _type, 
	       UINT32 _must, 
	       UINT32 _may, 
	       UINT32 _shouldNot, 
	       UINT32 _mustNot, 
	       bool _encr);
};

DiameterAvp* DiameterDictionary::findAvp (UINT32& _code)
{
  std::map<UINT32, DiameterAvp*>::iterator it;
  it = entries.find (_code);

  if (it == entries.end()) return NULL;
  return (*it).second;
}

DiameterAvp::AvpType DiameterDictionary::lookupAvpType (UINT32& _code)
{
  DiameterAvp* avp = findAvp (_code);

  if (avp == NULL) return DiameterAvp::OctetString;
  return avp->getType ();
}

#endif
