//
//  mbd.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/3/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "mbd.h"
#include <stdarg.h>

using namespace machete::platform;
using namespace machete::math;

namespace machete { 
  namespace data {
    
    Mbd::Mbd(const char *file) {
      char *data = NULL;
      
      ThePlatform->LoadFile(file, &data);
      
      ParseFile(data);
      
      delete data;
    }
    
    Mbd::~Mbd() {
      
    }
    
    int Mbd::Count(const Str & qry, ...) {
      StrTpl tpl(qry);
      
      int count = tpl.GetNumParams();
      
      StrParam p;
      
      va_list li;
      va_start(li, qry);
      Str xs(6);
      
      for (int i = 0; i < count; i++) {
        int x = va_arg(li, int);
        xs = x;
        p.Append(xs);
      }
      
      va_end(li);
      
      int newSize = qry.Size();
      newSize += count * 2;
      Str base(newSize);
      
      tpl.Build(base, &p);
      
      Tree<Str, unsigned int> *v = counts.Seek(base);
      
      if (v != NULL) {
        return v->GetValue();
      }
      
      return 0;
    }
    
    const Str Mbd::Value(const Str & qry, ...) {
      StrTpl tpl(qry);
      
      int count = tpl.GetNumParams();
      
      StrParam p;
      
      va_list li;
      va_start(li, qry);
      Str xs(6);
      
      for (int i = 0; i < count; i++) {
        int x = va_arg(li, int);
        xs = x;
        p.Append(xs);
      }
      
      va_end(li);
      
      Str base(qry.Size() + count * 2);
      
      tpl.Build(base, &p);
      
      
      Tree<Str, Str> *v = dict.Seek(base);
      
      if (v != NULL) {
        return v->GetValue();
      }
      
      return "";
    }

    int Mbd::IntValue(const Str & qry, ...) {
      StrTpl tpl(qry);
      
      int count = tpl.GetNumParams();
      
      StrParam p;
      
      va_list li;
      va_start(li, qry);
      Str xs(6);
      
      for (int i = 0; i < count; i++) {
        int x = va_arg(li, int);
        xs = x;
        p.Append(xs);
      }
      
      va_end(li);
      
      Str base(qry.Size() + count * 2);
      
      tpl.Build(base, &p);
      
      
      Tree<Str, Str> *v = dict.Seek(base);
      
      if (v != NULL) {
        return v->GetValue().ToInt();
      }
      
      return 0;
    }

    float Mbd::FloatValue(const Str & qry, ...) {
      StrTpl tpl(qry);
      
      int count = tpl.GetNumParams();
      
      StrParam p;
      
      va_list li;
      va_start(li, qry);
      Str xs(6);
      
      for (int i = 0; i < count; i++) {
        int x = va_arg(li, int);
        xs = x;
        p.Append(xs);
      }
      
      va_end(li);
      
      Str base(qry.Size() + count * 2);
      
      tpl.Build(base, &p);
      
      
      Tree<Str, Str> *v = dict.Seek(base);
      
      if (v != NULL) {
        return v->GetValue().ToFloat();
      }
      
      return 0;
    }

    void Mbd::ParseFile(const char *all) {
      char *addr;
      char *dictionary;
      char *data;
      
      if (all[0] != 'M' || all[1] != 'B' || all[2] != 'D') return;
      
      unsigned int headerSize = 19;
      
      unsigned int addrSize = *(unsigned int *)(all + headerSize);
      unsigned int dictionarySize = *(unsigned int *)(all + headerSize + 4);
      
      addr = (char *)(all + headerSize + 12);
      dictionary = (char *)(all + headerSize + 12 + addrSize);
      data = (char *)(all + headerSize + 12 + addrSize + dictionarySize);
      
      unsigned int addrCount = *(unsigned int *)(addr);
      addr += 4;
      
      unsigned int dictionaryCount = *(unsigned int*)(dictionary);
      dictionary += 4;
      
      for (unsigned int i = 0; i < addrCount; i++) {
        Str *v;
        
        unsigned int xoff = ReadString(addr, &v);
        unsigned int val = *(unsigned int *)(addr + xoff);
        
        addr += xoff + 4;
        
        counts.Add(*v, val);
        
        delete v;
      }
      
      for (unsigned int i = 0; i < dictionaryCount; i++) {
        Str *k, *v;
        
        unsigned int koff = ReadString(dictionary, &k);
        unsigned int address = *(unsigned int *)(dictionary + koff);
        
        dictionary += koff + 4;
        
        ReadString((char*)(data + address), &v);
        
        dict.Add(*k, *v);
        
        delete k;
        delete v;
      }
    }
    
    unsigned int Mbd::ReadString(const char* data, Str **v) const {
      unsigned int size = *(unsigned int *)(data);
      
      data += 4;
      
      *v = new Str(size);
      (*v)->Append(data, 0, size);
      
      return size + 4;
    }
    
    Layout::Layout(const char* file) : device(20) {
      dict = new Mbd(file);
    }
    
    Layout::~Layout() {
      delete dict;
    }
    
    void Layout::SetDevice(const char *_device) {
      device = _device;
    }
    
    float Layout::GetValue(const char *item) {
      Str all("", 50);
      all += item;
      all += "/@";
      all += device;
      
      return dict->FloatValue(all);
    }
    
    Vec2 Layout::GetVector(const char *item) {
      Str allX("", 50);
      allX += item;
      allX += "/";
      allX += device;
      allX += "/@x";

      Str allY("", 50);
      allY += item;
      allY += "/";
      allY += device;
      allY += "/@y";

      Vec2 val;
      
      val.x = dict->FloatValue(allX);
      val.y = dict->FloatValue(allY);
      
      return val;
    }
    
  } 
}
