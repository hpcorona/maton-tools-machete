//
//  mbd.h
//  machete
//
//  Created by Hilario Perez Corona on 6/2/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "tree.h"
#include "str.h"

namespace machete { namespace data {

  class Mbd {
  public:
    Mbd(const char *data);
    ~Mbd();
    
    int Count(const Str & qry, ...);
    const Str Value(const Str & qry, ...);
    
    void ParseFile(const char *d);
    unsigned int ReadString(const char* data, Str **v) const;
    
  private:
    Hash<Str, unsigned int> counts;
    Hash<Str, Str> dict;
  };

} }
