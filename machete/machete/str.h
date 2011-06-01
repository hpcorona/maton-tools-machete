//
//  str.h
//  machete
//
//  Created by Hilario Perez Corona on 5/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

namespace machete { namespace data {
    
    class str {
    public:
        str(int cap) {
            capacity = cap;
            chars = new char[capacity];
            len = 0;
        }
        
        str() {
            capacity = 50;
            chars = new char[capacity];
            len = 0;
        }
        
        str(const char *v) {
            int i = 0;
            while (v[i] != 0) { i++; }
            
            capacity = i;
            chars = new char[capacity];
            this->len = 0;
            
            append(v, 0, i);
        }
        
        str(const char *v, int cap) {
            capacity = cap;
            chars = new char[capacity];
            this->len = 0;
            
            append(v);
        }
        
        str(const char *v, int len, int cap) {
            capacity = cap;
            chars = new char[capacity];
            this->len = 0;

            append(v, 0, len);
        }
        
        virtual ~str() {
            delete chars;
        }
        
        void append(const char *v, int start, int len) {
            int idx = start;
            
            if (this->len + len > capacity) {
                len = capacity - this->len;
            }
            
            while (--len >= 0) {
                chars[this->len++] = v[idx++];
            }
        }
        
        void assign(const char *v, int len) {
            this->len = 0;
            append(v, 0, len);
        }
        
        void assign(const char *v) {
            int i = 0;
            
            while (v[i] != 0) { i++; }
            
            assign(v, i);
        }
        
        void append(const char *v) {
            int i = 0;
            
            while (v[i] != 0) { i++; }
            
            append(v, 0, i);
        }
        
        const str substr(int idx0, int len) {
            str newStr(len);
            
            if (idx0 > this->len) {
                idx0 = 0;
                len = 0;
            } else if (idx0 + len > this->len) {
                len = this->len - idx0;
            }
            
            newStr.append(chars, idx0, len);
            
            return newStr;
        }
        
        str & operator = (const str &ostr) {
            assign(ostr.chars, ostr.len);
            
            return *this;
        }
        
        str & operator = (const char *v) {
            assign(v);
            
            return *this;
        }
        
        str & operator += (const str &ostr) {
            append(ostr.chars, 0, ostr.len);
            
            return *this;
        }

        str & operator += (const char* v) {
            append(v);
            
            return *this;
        }

        const str operator + (const str &ostr) {
            str newStr(this->len + ostr.len);
            
            newStr += *this;
            newStr += ostr;
            
            return newStr;
        }

        const str operator + (const char *v) {
            str ostr(v);
            str newStr(this->len + ostr.len);
            
            newStr += *this;
            
            newStr += ostr;
            
            return newStr;
        }

        int compareTo(const str &ostr) const {
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
        
        bool operator == (const str &ostr) const {
            return compareTo(ostr) == 0;
        }
        
        bool operator != (const str &ostr) const {
            return compareTo(ostr) != 0;
        }
        
        bool operator > (const str &ostr) const {
            return compareTo(ostr) > 0;
        }

        bool operator >= (const str &ostr) const {
            return compareTo(ostr) >= 0;
        }

        bool operator < (const str &ostr) const {
            return compareTo(ostr) < 0;
        }

        bool operator <= (const str &ostr) const {
            return compareTo(ostr) <= 0;
        }
        
    private:
        char *chars;
        int capacity;
        int len;
    };
    
} }
