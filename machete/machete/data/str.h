//
//  str.h
//  machete
//
//  Created by Hilario Perez Corona on 5/7/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file str.h
//! \brief String and other utilities to handle strings.

#pragma once

#include "lists.h"

namespace machete { 
  namespace data {
    
		//! A string class implementation.
		/*!
     Caution: Don't use lots of strings in your program. They are pure evil.
     
     Try to use strings only on initialization or things like that.
     */
    class Str {
    public:
			//! Creates an empty string with a maximum length.
			/*!
			 \param cap Maximum length of this string.
       */
      Str(int cap) {
        capacity = cap;
        chars = new char[capacity];
        len = 0;
      }
      
			//! Creates an empty string with a maximum length of 50.
      Str() {
        capacity = 50;
        chars = new char[capacity];
        len = 0;
      }
      
			//! Creates a string using the contents of a char. The maximum length is equal to the length of the char* excluding the null termination character.
			/*!
			 \param v The character string.  Must be null terminated.
       */
      Str(const char *v) {
        int i = 0;
        while (v[i] != 0) { i++; }
        
        capacity = i;
        chars = new char[capacity];
        this->len = 0;
        
        Append(v, 0, i);
      }
      
			//! Creates a string using the contents of a char. The maximum length is specified with a parameter.
			/*!
			 \param v The character string.  Must be null terminated.
			 \param cap The maximum size of the string.
       */
      Str(const char *v, int cap) {
        capacity = cap;
        chars = new char[capacity];
        this->len = 0;
        
        Append(v);
      }
      
			//! Creates a string using the first 'len' chars of v. The maximum length is specified with a parameter.
			/*!
			 \param v The character string. Must be null terminated.
			 \param len Only the first characters will make the new string.
			 \param cap The maximum size of the string.
       */
      Str(const char *v, int len, int cap) {
        capacity = cap;
        chars = new char[capacity];
        this->len = 0;
        
        Append(v, 0, len);
      }
      
			//! Creates a string copying the contents of another string.
			/*!
			 \param s The other string to copy the data from.
       */
      Str(const Str &s) {
        capacity = s.len;
        chars = new char[capacity];
        this->len = 0;
        
        Append(s.chars, 0, capacity);
      }
      
			//! Destructor.
      ~Str() {
        delete chars;
        chars = NULL;
      }
      
			//! Append a section of a character string to the end of the current string.
			/*!
			 This method won't check for null characters.
       
			 Null characters are allowed in this string class implementation.
       
			 The data will be truncated if there is no enought space in the string.
       
			 \param v The character string.
			 \param start The initial index to copy data.
			 \param len The number of bytes to copy.
       */
      void Append(const char *v, int start, int len) {
        int idx = start;
        
        if (this->len + len > capacity) {
          len = capacity - this->len;
        }
        
        while (--len >= 0) {
          chars[this->len++] = v[idx++];
        }
      }
      
			//! Assign a section of a character string.
			/*!
			 The string will be cleared, and the character string will be assigned.
       
			 Null characters are allowed in this string class implementation.
       
			 The data will be truncated if there is no enought space in the string.
       
			 \param v The character string.
			 \param len The number of bytes to copy.
       */
      void Assign(const char *v, int len) {
        this->len = 0;
        Append(v, 0, len);
      }
      
			//! Assign a character string that is null terminated.
			/*!
			 The string will be cleared, and the character string will be assigned.
       
			 The data will be truncated if there is no enought space in the string.
       
			 \param v The character string. Must be null terminated.
			 \param len The number of bytes to copy.
       */
      void Assign(const char *v) {
        int i = 0;
        
        while (v[i] != 0) { i++; }
        
        Assign(v, i);
      }
      
			//! Append a character string that is null terminated to the end of the current string.
			/*!
			 The data will be truncated if there is no enought space in the string.
       
			 \param v The character string. Must be null terminated.
       */
      void Append(const char *v) {
        int i = 0;
        
        while (v[i] != 0) { i++; }
        
        Append(v, 0, i);
      }
      
			//! Append an int value at the end of the string.
			/*!
			 The value of the int will be converted to a character string and then
			 it will be appended to the string.
       
			 \param i The int value to append.
       */
      void AppendInt(int i) {
        if (i == 0) {
          Append("0");
          return;
        }
        
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
      
			//! Append a float value at the end of the string.
			/*!
			 The value of the float will be converted to a character string and then
			 it will be appended to the string.
       
			 \param f The float value to append.
       */
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
      
			//! Get an 8-bit char at an index.
			/*!
			 \param idx The index.
			 \return The character at that index.
       */
      char ByteAt(int idx) const {
        return chars[idx];
      }
      
			//! Get the UTF-8 character at an index.
			/*!
			 \param idx The index.
			 \return A 32-bits character that can represent any UTF-8 character.
       */
      wchar_t CharAt(int idx) const {
        wchar_t c;
        
        int s = CharSizeAt(idx);
        if (s == 3) {
          c = chars[idx] + chars[idx+1] >> 8 + chars[idx+2] >> 16;
        } else if (s == 2) {
          c = chars[idx] + chars[idx+1] >> 8;
        } else {
          c = chars[idx];
        }
        
        return c;
      }
      
			//! Fint a character starting at an index.
			/*!
			 \param c The character to find.
			 \param start The start index.
			 \return The next index position of c, or -1 if not found.
       */
      int Index(wchar_t c, int start) const {
        for (int i = start; i < len; i++) {
          if (c == chars[i]) {
            return i;
          }
        }
        
        return -1;
      }
      
			//! Fint the last character's index.
			/*!
			 \param c The character to find.
			 \return The last index position of c, or -1 if not found.
       */
      int LastIndex(wchar_t c) const {
        for (int i = len - 1; i >= 0; i--) {
          if (c == chars[i]) {
            return i;
          }
        }
        
        return -1;
      }
      
			//! Creates a new string using portion of this string.
			/*!
			 If idx0 or idx1 are above to the size of the string, it is most
			 likely that the new string will be made of garbage.
       
			 A more "secure" method to avoid garbage is Substr.
       
			 \param idx0 Start index of the slice.
			 \param idx1 End index of the slice (non-inclusive).
			 \return A new string containing the characters at that slice.
			 \sa Substr
       */
      const Str Slice(int idx0, int idx1) const {
        Str newStr(idx1 - idx0);
        
        newStr.Append(chars, idx0, idx1 - idx0);
        
        return newStr;
      }
      
			//! Creates a new string using a portion of this string.
			/*!
			 If idx0 is above the size of the string, then an empty string will
			 be returned.
       
			 The new string will be truncated to the bytes available.
       
			 \param idx0 Start index of the substring.
			 \param len Number of bytes to copy to the new string.
			 \return A new string containing the characters requested.
       */
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
      
      //! Ensure minimum capacity.
      void Ensure(int cap) {
        if (cap > capacity) {
          char *pv = chars;
          chars = new char[capacity];
          Assign(chars, len);
          delete pv;
          capacity = cap;
        }
      }
      
			//! Copy the content of a string into the current string.
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
      
			//! Copy the content of a character string into the current string. It seeks for a null-termination.
      Str & operator = (const char *v) {
        Assign(v);
        
        return *this;
      }
      
      //! Assign an integer value (string representation) to the current string.
      Str & operator = (int i) {
        len = 0;
        AppendInt(i);
        
        return *this;
      }
      
			//! Assign a float value (string representation) to the current string.
      Str & operator = (float f) {
        len = 0;
        AppendFloat(f);
        
        return *this;
      }
      
			//! Append another string to the current string.
      Str & operator += (const Str &ostr) {
        Append(ostr.chars, 0, ostr.len);
        
        return *this;
      }
      
			//! Append a character string to the current string.
      Str & operator += (const char* v) {
        Append(v);
        
        return *this;
      }
      
			//! Append the string representation of an integer value to the current string.
      Str & operator += (int i) {
        AppendInt(i);
        
        return *this;
      }
      
			//! Append the string representation of a float value to the current string.
      Str & operator += (float f) {
        AppendFloat(f);
        
        return *this;
      }
      
			//! Concatenates two strings and returns a new string.
      const Str operator + (const Str &ostr) {
        Str newStr(this->len + ostr.len);
        
        newStr += *this;
        newStr += ostr;
        
        return newStr;
      }
      
			//! Concatenates a string with a character string and returns a new string.
      const Str operator + (const char *v) {
        Str ostr(v);
        Str newStr(this->len + ostr.len);
        
        newStr += *this;
        
        newStr += ostr;
        
        return newStr;
      }
      
      //! Concatenates a string with a string representation of an integer and returns a new string.
      const Str operator + (int i) {
        Str newStr(this->len + 20);
        
        newStr += *this;
        newStr += i;
        
        return newStr;
      }
      
      //! Concatenates a string with a string representation of a float and returns a new string.
      const Str operator + (float f) {
        Str newStr(this->len + 26);
        
        newStr += *this;
        newStr += f;
        
        return newStr;
      }
      
			//! Compares this string to another string.
			/*!
			 \param ostr The other string to compare.
			 \return 0 if both strings are equal. Greater than zero if the current string is greater. Less than zero if the current string is lower.
       */
      int CompareTo(const Str &ostr) const {
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
      
			//! Fill a character string with the contents of the current string's chars.
			/*!
			 At the end, chars[len] will be assigned a null character.
       
			 \param chars Characters to fill.
			 \param len Maximum length to fill.
       */
      int GetChars(char *chars, int len) const {
        if (len > this->len) {
          len = this->len;
        }
        
        for (int i = 0; i < len; i++) {
          chars[i] = this->chars[i];
        }
        chars[len] = 0;
        
        return len;
      }
      
			//! Compares two strings for equality.
      bool operator == (const Str &ostr) const {
        return CompareTo(ostr) == 0;
      }
      
			//! Compares two strings for difference.
      bool operator != (const Str &ostr) const {
        return CompareTo(ostr) != 0;
      }
      
			//! Compares if the current string is greater than another one.
      bool operator > (const Str &ostr) const {
        return CompareTo(ostr) > 0;
      }
      
			//! Compares if the current string is greater than or equal to another one.
      bool operator >= (const Str &ostr) const {
        return CompareTo(ostr) >= 0;
      }
      
			//! Compares if the current string is lower than another one.
      bool operator < (const Str &ostr) const {
        return CompareTo(ostr) < 0;
      }
      
			//! Compares if the current string is lower than or equal to another one.
      bool operator <= (const Str &ostr) const {
        return CompareTo(ostr) <= 0;
      }
      
			//! Get the size of the string.
			/*!
			 \return The size of the string. This chould be different than the number of characters.
       */
      int Size() const {
        return len;
      }
      
			//! Converts the string to an integer.
			/*!
			 \return An int representation of the string.
       */
      int ToInt() const {
        return ToInt(0, len);
      }
      
			//! The size of a character at an index.
			/*!
			 This can be 1, 2 or 3, depending on the character type.
			 \param idx The index.
			 \return The size in bytes of the character in UTF-8 format.
       */
      int CharSizeAt(int idx) const {
        if (chars[idx] <= 127) {
          return 1;
        } else if (*((unsigned short *)(&chars[idx])) <= 2047) {
          return 2;
        } else {
          return 3;
        }
      }
      
			//! Converts a slice of the string to an integer.
			/*!
			 \param idx0 Initial index of the slice.
			 \param idx1 Final index of the slice (non inclusive).
			 \return An int representation of the string.
       */
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
      
			//! Converts the string to a float.
			/*!
			 \return A float representation of the string.
       */
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
			//! Characters that hold the string.
      char *chars;
      
			//! Maximum capacity for the string.
      int capacity;
      
			//! The length of the string.
      int len;
    };
    
		//! A string parameter.
		/*!
		 Contains all the parameters for a string templating class.
     
		 Internal use only.
     */
    typedef Iterator<Str> StrParam;
    
		//! A template section.
		/*!
		 This can be a string part, or a placeholder.
     
		 Internal use only.
     */
    class TplSection {
    public:
			//! Create a new empty section.
      TplSection() {
        value = NULL;
        idx = 0;
      }
      
			//! Create a new string part.
      TplSection(const Str &v) {
        value = new Str(v);
        idx = 0;
      }
      
			//! Create a new placeholder section with an index.
			/*!
       \param i The index corresponding to this placeholder.
       */
      TplSection(int i) {
        value = NULL;
        idx = i;
      }
      
			//! Destructor.
      ~TplSection() {
        idx = 0;
        
        delete value;
      }
      
			//! Get the value corresponding to this section.
			/*!
			 /param sp The iterator containing the information for all placeholders.
			 /return A string if this was a string part. Else, it will seek thru the parameter for the index related to this section.
       */
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
      
			//! Copy the information from another section.
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
			//! The index corresponding to this placeholder.
      int idx;
      
			//! The string corresponding to this string part.
      Str *value;
    };
    
		//! Different string parts
		/*!
		 Iterator containing several sections.
     
		 Internal use only.
     */
    typedef Iterator<TplSection> TplPart;
    
		//! Templating functionality.
		/*!
		 We can use %1, %2, ... %N to replace a string and create a new one.
     */
    class StrTpl : public TplPart {
    public:
      
			//! Create a new empty String Template
      StrTpl() : TplPart() {
				totalParams = 0;
      }
      
			//! Create a new String Template based on a Template String
			/*!
			 \param tpl Template String (containing %1, %2, etc... in any order)
       */
      StrTpl(const Str &tpl) : TplPart() {
        totalParams = 0;
        
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
          totalParams++;
          
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
      
			//! Append the result of the template to a string that you alreary own.
			/*!
			 \param base The result will be appended to this string.
			 \param params Parameters to build the String Template with. They must be in order.
       */
      void Build(Str & base, StrParam *params) {
        Reset();
        
        while (Next()) {
          base += GetCurrent()->GetValue().GetValue(params);
        }
      }
      
			//! Get the number of parameters on the string template
			/*!
			 \return The number of parameters.
       */
      int GetNumParams() {
        return totalParams;
      }
      
    private:
			//! The total number of parameters in this template.
      int totalParams;
    };
    
  } 
}
