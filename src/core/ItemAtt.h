/*
* Copyright (c) 2003-2015 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/
// ItemAtt.h
//-----------------------------------------------------------------------------

#ifndef __ITEMATT_H
#define __ITEMATT_H

#include "Util.h"
#include "Match.h"
#include "Item.h"
#include "os/UUID.h"
#include "StringX.h"

#include <time.h> // for time_t

//-----------------------------------------------------------------------------

/*
* CItemAtt is a class that contains an attachment associated with a password entry
*
* What makes this class interesting is that all fields are kept encrypted
* from the moment of construction, and are decrypted by the appropriate
* accessor (Get* member function).
*
* All this is to protect the data in memory, and has nothing to do with
* how the records are written to disk.
*/

class BlowFish;
class PWSfile;

class CItemAtt : public CItem
{
public:
  typedef char key256T[32];
  typedef char contentHMACT[32];

  enum FieldType {
    ATTUUID = 0x61,
    TITLE = 0x03,
    CTIME = 0x07,
    MEDIATYPE = 0x62,
    FILENAME = 0x63,
    ATTEK = 0x64,
    ATTAK = 0x65,
    CONTENT = 0x66,
    CONTENTHMAC = 0x67,
    END = 0xff
};

  //Construction
  CItemAtt();
  CItemAtt(const CItemAtt& stuffhere);

  ~CItemAtt();

  int Read(PWSfile *in);
  int Write(PWSfile *out) const;

  int Import(const stringT &fname);
  int Export(const stringT &fname) const;

  // Convenience: Get the name associated with FieldType
  static stringT FieldName(FieldType ft);
  // Convenience: Get the untranslated (English) name of a FieldType
  static stringT EngFieldName(FieldType ft);

  //Data retrieval
  StringX GetTitle() const {return GetField(TITLE);}
  void GetUUID(uuid_array_t &) const;
  const pws_os::CUUID GetUUID() const;
  StringX GetFileName() const {return GetField(FILENAME);}
  StringX GetCTime() const {return GetTime(CTIME, PWSUtil::TMC_ASC_UNKNOWN);}  // V30
  time_t GetCTime(time_t &t) const {GetTime(CTIME, t); return t;}  // V30

  void CreateUUID(); // for new
  void SetUUID(const pws_os::CUUID &uuid);
  void SetTitle(const StringX &title);
  void SetCTime() {SetTime(CTIME);}  // V30
  void SetCTime(time_t t) {SetTime(CTIME, t);}  // V30
  bool SetCTime(const stringT &time_str) {return SetTime(CTIME, time_str);}  // V30

  CItemAtt& operator=(const CItemAtt& second);

  // For unit tests:
  bool operator==(const CItemAtt &that) const;


  bool HasUUID() const                     { return IsFieldSet(ATTUUID);   }
  bool IsTitleSet() const                  { return IsFieldSet(TITLE);     }
  bool IsCreationTimeSet() const           { return IsFieldSet(CTIME);     }


  EntryStatus GetStatus() const {return m_entrystatus;}
  void ClearStatus() {m_entrystatus = ES_CLEAN;}
  void SetStatus(const EntryStatus es) {m_entrystatus = es;}


private:
  EntryStatus m_entrystatus;
  StringX GetTime(int whichtime, PWSUtil::TMC result_format) const; // V30
  void GetTime(int whichtime, time_t &t) const; // V30
  void SetTime(const int whichtime); // V30
  void SetTime(const int whichtime, time_t t); // V30
  bool SetTime(const int whichtime, const stringT &time_str); // V30
};

#endif /* __ITEMATT_H */
//-----------------------------------------------------------------------------
// Local variables:
// mode: c++
// End: