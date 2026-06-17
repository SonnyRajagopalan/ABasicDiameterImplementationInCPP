#include <iterator>
#include <iostream>
#include <typeinfo>
#include "avps/DiameterAvpList.h"
#include "avps/DiameterDictionary.h"
#include "common/Debug.h"
// AVPs:
#include "avps/DiameterInteger32Avp.h"
#include "avps/DiameterInteger64Avp.h"
#include "avps/DiameterUnsigned32Avp.h"
#include "avps/DiameterUnsigned64Avp.h"
#include "avps/DiameterFloat32Avp.h"
#include "avps/DiameterFloat64Avp.h"
#include "avps/DiameterEnumeratedAvp.h"
#include "avps/DiameterGroupedAvp.h"
#include "avps/DiameterOctetStringAvp.h"
#include "avps/DiameterUTF8StringAvp.h"
#include "avps/DiameterTimeAvp.h"
#include "avps/DiameterAddressAvp.h"
#include "avps/DiameterDiamIdentAvp.h"
#include "avps/DiameterDiamURIAvp.h"
#include "avps/DiameterIPFiltrRuleAvp.h"
#include "avps/DiameterQoSFilterRuleAvp.h"

DiameterAvpList::DiameterAvpList ()
{
  totalAvpListLength = 0;
}

DiameterAvpList::~DiameterAvpList ()
{
  for (std::vector<DiameterAvp*>::iterator it = avpVector.begin(); it != avpVector.end(); ++it)
    delete *it;
}

void DiameterAvpList::addAvpToAvpList (const DiameterAvp* avp)
{
  UINT32 avpLen = avp->getLength ();

  /**
     Notes: 04/22/2012: This attribute totalAvpListLength is used by
     methods in the DiameterMessage family of objects. The total
     message length which we place in the header does not
     contain padding bytes length, so this totalAvpListLength 
     computed here should also not contain the padding length.
  */
  totalAvpListLength += avpLen;

  /*
  if ((avpLen % 4) != 0)
    {
      avpLen = avpLen + (4 - (avpLen % 4));
    }
  cout << "    (DiameterAvpList::addAvpToAvpList) Len is currently = " << totalAvpListLength;
  //totalAvpListLength += avpLen; (see note from 04/22/2012 above).
  cout << ", total len is now " << totalAvpListLength << endl;
  */

  avpVector.push_back (const_cast<DiameterAvp*>(avp));
}

void DiameterAvpList::serialize (OutputStream& os)
{
  vector<DiameterAvp*>::iterator item;

  for (item = avpVector.begin (); item != avpVector.end (); item++)
    {
      //cout << "(DiameterAvpList::serialize) Serializing item with code = " << (*item)->getCode () << ", and type = " << typeid (*item).name () << endl;
      (*item)->serialize (os);
    }
}

void DiameterAvpList::deserialize (InputStream& in)
{
  DiameterDictionary* dict = DiameterDictionary::getInstance ();

  while (!in.eof ())
    {      
      UINT32 _code = 0;
      UINT8  _flags = 0;
      UINT8  _length[3];
      UINT32 _vendorId = 0;
      UINT32 avpLenUINT32 = 0;
      UINT32 readInSoFar = 0; // Bytes read in so far
      UINT32 sizeOfData = 0;  // Size (in bytes) of the "data" field
      UINT32 padding = 0;     // Used for OctetString and derived avps
      DiameterAvp::AvpType _avpType;
      // Deserialize code, flags, and length to get the other information

      deserializeUINT32 (_code, in); readInSoFar += 4;
      deserializeUINT8  (_flags, in); readInSoFar += 1;
      deserializeTo3ByteUnsignedChar (_length, in); readInSoFar += 3;

      if (_flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
	{
	  deserializeUINT32 (_vendorId, in); readInSoFar += 4;
	}
      unsplit3ByteUINT8ArrayToUINT32 (avpLenUINT32, _length);

      // Compute size of the data filed and the padding (if present)
      sizeOfData = avpLenUINT32 - readInSoFar;

      if ((sizeOfData % 4) != 0)
	{
	  padding = 4 - (sizeOfData % 4);
	  sizeOfData += padding;
	}

      // Figure out what type of AVP the data contains
      _avpType = dict->lookupAvpType (_code);

      cout << "Deserializing ... code = " << dec << _code << ", avptype = " << getAvpTypeString (_avpType);
      cout << ", flags = " << hex << static_cast<int>(_flags);
      cout << dec << ", len = " << avpLenUINT32;
      cout << " > will read " << sizeOfData << " bytes into the current AVP" << endl; 
      if (padding)
	{
	  //cout << ", and skip (" << padding << ") padding bytes";
	}
      //cout << endl;

      //cout << "Dictionary size = " << dict->getDictionarySize () << endl;
      //in.read (junkBytes, sizeOfData); Not needed after switch case DiameterAvp::buildout
      switch (_avpType)
	{
	case DiameterAvp::Integer32:
	  {
	    SINT32 _value;
	    deserializeSINT32 (_value, in);
	    DiameterInteger32Avp* avp = new DiameterInteger32Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Integer32 avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Enumerated:
	  {
	    SINT32 _value;
	    deserializeSINT32 (_value, in);
	    DiameterEnumeratedAvp* avp = new DiameterEnumeratedAvp (_code, _flags, _vendorId, _value);

	    //cout << "..Enumerated avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Integer64:
	  {
	    SINT64 _value;
	    deserializeSINT64 (_value, in);
	    DiameterInteger64Avp* avp = new DiameterInteger64Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Integer64 avp received [" << _value.int64 << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Unsigned32:
	  {
	    UINT32 _value;
	    deserializeUINT32 (_value, in);
	    DiameterUnsigned32Avp* avp = new DiameterUnsigned32Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Unsigned32 avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Unsigned64:
	  {
	    UINT64 _value;
	    deserializeUINT64 (_value, in);
	    DiameterUnsigned64Avp* avp = new DiameterUnsigned64Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Unsigned64 avp received [" << _value.uint64 << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Float32:
	  {
	    FLT32 _value;
	    deserializeFLT32 (_value, in);
	    DiameterFloat32Avp* avp = new DiameterFloat32Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Float32 avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Float64:
	  {
	    FLT64 _value;
	    deserializeFLT64 (_value, in);
	    DiameterFloat64Avp* avp = new DiameterFloat64Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Float64 avp received [" << _value.flt64 << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Grouped: // Size of Grouped AVP is always % 4
	  {
	    list<DiameterAvp*> _value = deserializeGroupedAvp (sizeOfData, in, 0);
	    DiameterGroupedAvp* avp = new DiameterGroupedAvp (_code, _flags, _vendorId, _value);	    
	    UINT8 _length[3];
	    UINT32 _avpLenUINT32 = sizeOfData + padding + readInSoFar;
	    splitUINT32To3ByteUINT8Array (_avpLenUINT32, _length);

	    avp->setLength (_length);
	    //cout << "..Grouped avp received [<not printed (top level), see above>]";
	    //cout << ", total length from created AVP = " << avp->getLength () << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::OctetString:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!
	    DiameterOctetStringAvp* avp = new DiameterOctetStringAvp (_code, _flags, _vendorId, _value);	    

	    //cout << "..OctetString avp [" << _value << "] received" << endl;
	    
	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Address:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!
	    DiameterAddressAvp* avp = new DiameterAddressAvp (_code, _flags, _vendorId, _value);	    

	    //cout << "..AddressAvp avp [" << _value << "] received" << endl;
	    
	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::Time:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!
	    DiameterTimeAvp* avp = new DiameterTimeAvp (_code, _flags, _vendorId, _value);	    

	    //cout << "..OctetString avp [" << _value << "] received" << endl;
	    
	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::UTF8String:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterUTF8StringAvp* avp = new DiameterUTF8StringAvp (_code, _flags, _vendorId, _value);

	    //cout << "..UTF8String avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::DiamIdent:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterDiamIdentAvp* avp = new DiameterDiamIdentAvp (_code, _flags, _vendorId, _value);
// 	    cout << "Sizeofdata = " << sizeOfData << " and padding = " << padding << endl;
// 	    cout << "..DiamIdent avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::DiamURI:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterDiamURIAvp* avp = new DiameterDiamURIAvp (_code, _flags, _vendorId, _value);

	    //cout << "..DiamURI avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::IPFiltrRule:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterIPFiltrRuleAvp* avp = new DiameterIPFiltrRuleAvp (_code, _flags, _vendorId, _value);

	    //cout << "..IPFiltrRule avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	case DiameterAvp::QoSFilterRule:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterQoSFilterRuleAvp* avp = new DiameterQoSFilterRuleAvp (_code, _flags, _vendorId, _value);

	    //cout << "..QoSFilterRule avp received [" << _value << "]" << endl;

	    addAvpToAvpList (avp);
	    break;
	  }
	default:
	  {
	    // Unknown AVP type - skip the data
	    vector<char> skipBuf(sizeOfData);
	    in.read (&skipBuf[0], sizeOfData);
	  }
	} // End of switch statement
      in.peek (); // if EOF, will result in the flag being set
    } // End of while loop reading in from InputStream&

}

list<DiameterAvp*> DiameterAvpList::deserializeGroupedAvp (UINT32& sizeOfData, InputStream& in, UINT32 depth)
{
  UINT32 dataBytesLeft = sizeOfData;
  list<DiameterAvp*> componentAvps;
  DiameterDictionary* dict = DiameterDictionary::getInstance ();

  while (dataBytesLeft)
    {      
      UINT32 _code = 0;
      UINT8  _flags = 0;
      UINT8  _length[3];
      UINT32 _vendorId = 0;
      UINT32 avpLenUINT32 = 0;
      UINT32 readInSoFar = 0; // Bytes read in so far
      UINT32 sizeOfData = 0;  // Size (in bytes) of the "data" field
      UINT32 padding = 0;     // Used for OctetString and derived avps
      DiameterAvp::AvpType _avpType;

      // Deserialize code, flags, and length to get the other information
      deserializeUINT32 (_code, in); readInSoFar += 4; dataBytesLeft -= 4;
      deserializeUINT8  (_flags, in); readInSoFar += 1; dataBytesLeft -= 1;
      deserializeTo3ByteUnsignedChar (_length, in); readInSoFar += 3; dataBytesLeft -= 3;
      if (_flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
	{
	  deserializeUINT32 (_vendorId, in); readInSoFar += 4; dataBytesLeft -= 4;
	}
      unsplit3ByteUINT8ArrayToUINT32 (avpLenUINT32, _length);

      // Compute size of the data filed and the padding (if present)
      sizeOfData = avpLenUINT32 - readInSoFar;
      if ((sizeOfData % 4) != 0)
	{
	  padding = 4 - (sizeOfData % 4);
	  sizeOfData += padding;
	}

      // Figure out what type of AVP the data contains
      _avpType = dict->lookupAvpType (_code);

      for (int i = 0; i < (int) depth; i++) cout << "\t";
      cout << "Deserializing grouped avp ... code = " << dec << _code << ", avptype = " << _avpType;
      cout << ", flags = " << hex << static_cast<int>(_flags);
      cout << dec << ", len = " << avpLenUINT32;
      cout << " > will read " << sizeOfData << " bytes into the current AVP" << endl;
      if (padding)
	{
	  //cout << ", and skip (" << padding << ") padding bytes";
	}
      //cout << endl;

      //in.read (junkBytes, sizeOfData); Not needed after switch case DiameterAvp::buildout
      switch (_avpType)
	{
	case DiameterAvp::Integer32:
	  {
	    SINT32 _value;
	    deserializeSINT32 (_value, in);
	    DiameterInteger32Avp* avp = new DiameterInteger32Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Integer32 avp received [" << _value << "]" << endl;

	    dataBytesLeft -= 4;
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Enumerated:
	  {
	    SINT32 _value;
	    deserializeSINT32 (_value, in);
	    DiameterEnumeratedAvp* avp = new DiameterEnumeratedAvp (_code, _flags, _vendorId, _value);

	    //cout << "..Enumerated avp received [" << _value << "]" << endl;

	    dataBytesLeft -= 4;
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Integer64:
	  {
	    SINT64 _value;
	    deserializeSINT64 (_value, in);
	    DiameterInteger64Avp* avp = new DiameterInteger64Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Integer64 avp received [" << _value.int64 << "]" << endl;

	    dataBytesLeft -= 8;
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Unsigned32:
	  {
	    UINT32 _value;
	    deserializeUINT32 (_value, in);
	    DiameterUnsigned32Avp* avp = new DiameterUnsigned32Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Unsigned32 avp received [" << _value << "]" << endl;

	    dataBytesLeft -= 4;
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Unsigned64:
	  {
	    UINT64 _value;
	    deserializeUINT64 (_value, in);
	    DiameterUnsigned64Avp* avp = new DiameterUnsigned64Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Unsigned64 avp received [" << _value.uint64 << "]" << endl;

	    dataBytesLeft -= 8;
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Float32:
	  {
	    FLT32 _value;
	    deserializeFLT32 (_value, in);
	    DiameterFloat32Avp* avp = new DiameterFloat32Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Float32 avp received [" << _value << "]" << endl;

	    dataBytesLeft -= 4;
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Float64:
	  {
	    FLT64 _value;
	    deserializeFLT64 (_value, in);
	    DiameterFloat64Avp* avp = new DiameterFloat64Avp (_code, _flags, _vendorId, _value);

	    //cout << "..Float64 avp received [" << _value.flt64 << "]" << endl;

	    dataBytesLeft -= 8;
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Grouped: // Size of Grouped AVP is always % 4
	  {
	    list<DiameterAvp*> _value = deserializeGroupedAvp (sizeOfData, in, depth+1);
	    DiameterGroupedAvp* avp = new DiameterGroupedAvp (_code, _flags, _vendorId, _value);	    
	    UINT8 _length[3];
	    UINT32 _avpLenUINT32 = sizeOfData + padding + readInSoFar;
	    splitUINT32To3ByteUINT8Array (_avpLenUINT32, _length);

	    avp->setLength (_length);
	    //cout << "..Grouped avp received [<not printed (top level), see above>]";
	    //cout << ", total length from created AVP = " << avp->getLength () << endl;

	    dataBytesLeft -= sizeOfData;
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::OctetString:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!
	    DiameterOctetStringAvp* avp = new DiameterOctetStringAvp (_code, _flags, _vendorId, _value);	    

	    //cout << "..OctetString avp [" << _value << "] received" << endl;
	    
	    dataBytesLeft -= (_value.size () + padding);
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Address:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!
	    DiameterAddressAvp* avp = new DiameterAddressAvp (_code, _flags, _vendorId, _value);	    

	    //cout << "..AddressAvp avp [" << _value << "] received" << endl;
	    
	    dataBytesLeft -= (_value.size () + padding);
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::Time:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!
	    DiameterTimeAvp* avp = new DiameterTimeAvp (_code, _flags, _vendorId, _value);	    

	    //cout << "..OctetString avp [" << _value << "] received" << endl;
	    
	    dataBytesLeft -= (_value.size () + padding);
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::UTF8String:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterUTF8StringAvp* avp = new DiameterUTF8StringAvp (_code, _flags, _vendorId, _value);

	    //cout << "..UTF8String avp received [" << _value << "]" << endl;

	    dataBytesLeft -= (_value.size () + padding);
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::DiamIdent:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterDiamIdentAvp* avp = new DiameterDiamIdentAvp (_code, _flags, _vendorId, _value);

	    //cout << "..DiamIdent avp received [" << _value << "]" << endl;

	    dataBytesLeft -= (_value.size () + padding);
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::DiamURI:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterDiamURIAvp* avp = new DiameterDiamURIAvp (_code, _flags, _vendorId, _value);

	    //cout << "..DiamURI avp received [" << _value << "]" << endl;

	    dataBytesLeft -= (_value.size () + padding);
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::IPFiltrRule:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterIPFiltrRuleAvp* avp = new DiameterIPFiltrRuleAvp (_code, _flags, _vendorId, _value);

	    //cout << "..IPFiltrRule avp received [" << _value << "]" << endl;

	    dataBytesLeft -= (_value.size () + padding);
	    componentAvps.push_back (avp);
	    break;
	  }
	case DiameterAvp::QoSFilterRule:
	  {
	    BasicString _value;
	    deserializeString (_value, in, (sizeOfData - padding));
	    deserializePadding (padding, in); // Discard these bytes!

	    DiameterQoSFilterRuleAvp* avp = new DiameterQoSFilterRuleAvp (_code, _flags, _vendorId, _value);

	    //cout << "..QoSFilterRule avp received [" << _value << "]" << endl;

	    dataBytesLeft -= (_value.size () + padding);
	    componentAvps.push_back (avp);
	    break;
	  }
	default:
	  {
	    // Unknown grouped AVP type - skip the data
	    vector<char> skipBuf(sizeOfData);
	    in.read (&skipBuf[0], sizeOfData);
	    dataBytesLeft -= sizeOfData;
	  }
	} // End of switch statement
      in.peek (); // if EOF, will result in the flag being set
    } // End of while loop reading in from InputStream&
  return componentAvps;
}
