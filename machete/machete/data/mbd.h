//
//  mbd.h
//  machete
//
//  Created by Hilario Perez Corona on 6/2/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file mbd.h
//! \brief This file includes the definition of Machete Binary Dictionary

#pragma once

#include "tree.h"
#include "str.h"
#include "../platform/platform.h"

namespace machete { 
  namespace data {
    
		//! Machete Binary Dictionary Class
		/*!
		Machete only reads this kind of files to load:
		Levels, Images, Configuration, Layouts, etc...
		
		It consumes slightly more memory than reading binary
		files directly, but it is a lot faster.
		
		This class uses a Red-Black Tree to load huge amount
		of entries and provide a fast lookup for values.
		
		This is definitely faster than loading traditional
		text files, XML, JSON, etc...
		
		The mbd utility can convert XML, lang and pos files
		into mbd files.
		 */
    class Mbd {
    public:
			//! Creates a Dictionary by reading a file.
			/*!
			 \param file The name of the file. If it's extension is xml, it will automatically be changed to mbd.
			 */
      Mbd(const char *file);

			//! Destructor.
      ~Mbd();
      
			//! Count the number of items in a structured Dictionary.
			/*!
			 \param qry Query of the string to use.
			 \param ... Parameters to build the query.
			 \return The total number of items for the requested query.
			 */
      int Count(const Str & qry, ...);

			//! Get the string value of a query.
			/*!
			 \param qry Query the string value of an item.
			 \param ... Parameters to build the query.
			 \return The string value of the requested query.
			 */
      const Str Value(const Str & qry, ...);

			//! Get the int value of a query.
			/*!
			 \param qry Query the int value of an item.
			 \param ... Parameters to build the query.
			 \return The int value of the requested query.
			 */
      int IntValue(const Str & qry, ...);

			//! Get the float value of a query.
			/*!
			 \param qry Query the float value of an item.
			 \param ... Parameters to build the query.
			 \return The float value of the requested query.
			 */
      float FloatValue(const Str & qry, ...);
      
    private:
			//! Parse from memory.
			/*!
			 \param d Contains all the contents of a file. (this is not the file name)
			 */
      void ParseFile(const char *d);

			//! Read a string from a buffer of data.
			/*!
			 \param data A string will be readed from this data, starting at position 0.
			 \param v A pointer to a string pointer. A new pointer will be created.
			 \return The number of bytes consumed from data.
			 */
      unsigned int ReadString(const char* data, Str **v) const;

			//! Values for the Count queries.
      Hash<Str, unsigned int> counts;

			//! Values for the Value queries.
      Hash<Str, Str> dict;
    };
    
  } 
}
