//
//  str.h
//  machete
//
//  Created by Hilario Perez Corona on 5/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "lists.h"

namespace machete { namespace data {
    
    class Str {
    public:
        Str(int cap) {
            capacity = cap;
            chars = new char[capacity];
            len = 0;
        }
        
        Str() {
            capacity = 50;
            chars = new char[capacity];
            len = 0;
        }
        
        Str(const char *v) {
            int i = 0;
            while (v[i] != 0) { i++; }
            
            capacity = i;
            chars = new char[capacity];
            this->len = 0;
            
            Append(v, 0, i);
        }
        
        Str(const char *v, int cap) {
            capacity = cap;
            chars = new char[capacity];
            this->len = 0;
            
            Append(v);
        }
        
        Str(const char *v, int len, int cap) {
            capacity = cap;
            chars = new char[capacity];
            this->len = 0;

            Append(v, 0, len);
        }
      
        Str(const Str &s) {
          capacity = s.len;
          chars = new char[capacity];
          this->len = 0;
          
          Append(s.chars, 0, capacity);
        }
      
        ~Str() {
          delete chars;
          chars = NULL;
        }
        
        void Append(const char *v, int start, int len) {
            int idx = start;
            
            if (this->len + len > capacity) {
                len = capacity - this->len;
            }
            
            while (--len >= 0) {
                chars[this->len++] = v[idx++];
            }
        }
        
        void Assign(const char *v, int len) {
            this->len = 0;
            Append(v, 0, len);
        }
        
        void Assign(const char *v) {
            int i = 0;
            
            while (v[i] != 0) { i++; }
            
            Assign(v, i);
        }
        
        void Append(const char *v) {
            int i = 0;
            
            while (v[i] != 0) { i++; }
            
            Append(v, 0, i);
        }
      
        void AppendInt(int i) {
          char str[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
          
          bool sign = false;
          if (i < 0) {
            sign = true;
            i = i * -1;
          }
          
          int pos = 18;
          while (i != 0) {
            int v = i % 10;
            i = i / 10;
            str[pos--] = v + 48;
          }
          
          if (sign) {
            str[pos] = '-';
          } else {
            pos++;
          }
          
          Append(&str[pos]);
        }
      
        void AppendFloat(float f) {
          AppendInt((int)f);
          
          float dp = f - (int)f;
          if (dp < 0) {
            dp = dp * -1;
          }
          
          if (dp != 0) {
            *this += ".";
            
            for (int i = 0; i < 5; i++) {
              dp = dp * 10.0f;
              if (dp - (int)dp == 0) {
                break;
              }
            }
            
            AppendInt((int)dp);
          }
        }
      
        char ByteAt(int idx) const {
          return chars[idx];
        }
      
        int Index(wchar_t c, int start) const {
          for (int i = start; i < len; i++) {
            if (c == chars[i]) {
              return i;
            }
          }
          
          return -1;
        }
      
        int LastIndex(wchar_t c) const {
          for (int i = len - 1; i >= 0; i--) {
            if (c == chars[i]) {
              return i;
            }
          }
          
          return -1;
        }
      
        const Str Slice(int idx0, int idx1) const {
          Str newStr(idx1 - idx0);
          
          newStr.Append(chars, idx0, idx1 - idx0);
          
          return newStr;
        }
        
        const Str Substr(int idx0, int len) const {
            Str newStr(len);
            
            if (idx0 > this->len) {
                idx0 = 0;
                len = 0;
            } else if (idx0 + len > this->len) {
                len = this->len - idx0;
            }
            
            newStr.Append(chars, idx0, len);
            
            return newStr;
        }
        
        Str & operator = (const Str &ostr) {
            if (ostr.len > capacity) {
              delete chars;
              
              capacity = ostr.len;
              chars = new char[capacity];
              len = 0;
            }
            Assign(ostr.chars, ostr.len);
            
            return *this;
        }
        
        Str & operator = (const char *v) {
            Assign(v);
            
            return *this;
        }
      
        Str & operator = (int i) {
          len = 0;
          AppendInt(i);
          
          return *this;
        }

        Str & operator = (float f) {
          len = 0;
          AppendFloat(f);
        
          return *this;
        }

        Str & operator += (const Str &ostr) {
            Append(ostr.chars, 0, ostr.len);
            
            return *this;
        }

        Str & operator += (const char* v) {
            Append(v);
            
            return *this;
        }
      
        Str & operator += (int i) {
          AppendInt(i);
          
          return *this;
        }

        Str & operator += (float f) {
          AppendFloat(f);
        
          return *this;
        }

        const Str operator + (const Str &ostr) {
            Str newStr(this->len + ostr.len);
            
            newStr += *this;
            newStr += ostr;
            
            return newStr;
        }

        const Str operator + (const char *v) {
            Str ostr(v);
            Str newStr(this->len + ostr.len);
            
            newStr += *this;
            
            newStr += ostr;
            
            return newStr;
        }

        const Str operator + (int i) {
          Str newStr(this->len + 20);
        
          newStr += *this;
          newStr += i;
        
          return newStr;
        }

        const Str operator + (float f) {
          Str newStr(this->len + 26);
        
          newStr += *this;
          newStr += f;
        
          return newStr;
        }

        int compareTo(const Str &ostr) const {
            int toComp = this->len;
            if (ostr.len < toComp) {
                toComp = ostr.len;
            }
            
            int diff;
            for (int i = 0; i < toComp; i++) {
                diff = chars[i] - ostr.chars[i];
                
                if (diff != 0) {
                    return diff;
                }
            }
            
            diff = this->len - ostr.len;
            
            return diff;
        }
        
        bool operator == (const Str &ostr) const {
            return compareTo(ostr) == 0;
        }
        
        bool operator != (const Str &ostr) const {
            return compareTo(ostr) != 0;
        }
        
        bool operator > (const Str &ostr) const {
            return compareTo(ostr) > 0;
        }

        bool operator >= (const Str &ostr) const {
            return compareTo(ostr) >= 0;
        }

        bool operator < (const Str &ostr) const {
            return compareTo(ostr) < 0;
        }

        bool operator <= (const Str &ostr) const {
            return compareTo(ostr) <= 0;
        }
      
        int Size() const {
          return len;
        }
      
        int ToInt() const {
          return ToInt(0, len);
        }
      
        int ToInt(int idx0, int idx1) const {
          int n = 0;
          int s = idx0;
          
          if (chars[idx0] == '-') {
            s++;
          }
        
          char c;
          for (int i = s; i < idx1; i++) {
            c = chars[i];
            if (c < '0' || c > '9') {
              break;
            }
            
            c -= 48;
            n = n * 10 + c;
          }
          
          if (chars[idx0] == '-') {
            n = n * -1;
          }
          
          return n;
        }
      
        float ToFloat() const {
          int pp = Index('.', 0);
          
          if (pp < 1) {
            return (float)ToInt();
          } else {
            float ent = ToInt(0, pp);
            float dec = ToInt(pp + 1, len);
            
            for (int i = pp + 1; i < len; i++) {
              if (chars[i] < '0' || chars[i] > '9') break;
              
              dec /= 10.0f;
            }
            
            if (ent < 0) {
              return ent - dec;
            } else {
              return ent + dec;
            }
          }
        }

      
    private:
        char *chars;
        int capacity;
        int len;
    };

    typedef Iterator<Str> StrParam;

    class TplSection {
    public:
      TplSection() {
        value = NULL;
        idx = 0;
      }
      
      TplSection(const Str &v) {
        value = new Str(v);
        idx = 0;
      }
      
      TplSection(int i) {
        value = NULL;
        idx = i;
      }
      
      ~TplSection() {
        idx = 0;
        
        delete value;
      }
      
      Str & GetValue(StrParam *sp) {
        if (idx == 0) {
          return *value;
        }
        
        sp->Reset();
        int i = idx;
        
        while (--i >= 0) {
          if (sp->Next() == false) {
            break;
          }
          
          if (i == 0) {
            return sp->GetCurrent()->GetValue();
          }
        }
        
        return sp->GetCurrent()->GetValue();
      }
      
      TplSection & operator = (const TplSection &s) {
        if (s.value != NULL) {
          value = new Str(*s.value);
        } else {
          delete value;
          
          value = NULL;
        }
        idx = s.idx;
        
        return *this;
      }
      
    private:
      int idx;
      Str *value;
    };
  
    typedef Iterator<TplSection> TplPart;
    
    class StrTpl : public TplPart {
    public:
      StrTpl() : TplPart() {
        
      }
      
      StrTpl(const Str &tpl) : TplPart() {
        int prev = 0;
        int idx = tpl.Index('%', prev);
        while (idx >= 0) {
          if (idx > 0 && tpl.ByteAt(idx - 1) == '\\') {
            idx = tpl.Index('%', idx + 1);
            continue;
          }
          
          char c = tpl.ByteAt(idx + 1);
          if (c < '0' || c > '9') {
            idx = tpl.Index('%', idx + 1);
            continue;
          }
          
          Append(TplSection(tpl.Slice(prev, idx)));
          prev = idx + 1;
          idx = prev;
          
          while (c >= '0' && c <= '9') {
            idx++;
            c = tpl.ByteAt(idx);
          }
          
          Append(TplSection(tpl.Slice(prev, idx).ToInt()));
          
          if (c == '_') {
            idx++;
          }
          
          prev = idx;
          idx = tpl.Index('%', prev);
        }
        
        if (tpl.Size() > prev) {
          Append(TplSection(tpl.Slice(prev, tpl.Size())));
        }
      }
      
      Str Build(int cap, StrParam *params) {
        Str base(cap);
        
        Reset();
        
        while (Next()) {
          base = base + GetCurrent()->GetValue().GetValue(params);
        }

        return base;
      }
    };
    
} }
