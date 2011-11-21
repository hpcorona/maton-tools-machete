//
//  sql.h
//  machete
//
//  Created by Hilario Perez Corona on 11/20/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//
//! \file sql.h
//! \brief SQLite3 tools.

#pragma once

#include <sqlite3.h>
#include "../data/str.h"
#include "../platform/platform.h"

using namespace machete::data;
using namespace machete::platform;

namespace machete {
  
  //! Database tools.
  namespace storage {
    
    //! Column type.
    enum ColumnType {
      TypeInteger = SQLITE_INTEGER,   //! Integer
      TypeFloat = SQLITE_FLOAT,       //! Float
      TypeText = SQLITE_TEXT,         //! Text
      TypeBlob = SQLITE_BLOB,         //! Blob
      TypeNull = SQLITE_NULL          //! Null
    };
    
    //! A database Query.
    class DbQuery {
    public:
      
      //! Creates a new uninitialized query object.
      DbQuery();
      
      //! Destructor.
      ~DbQuery();
      
      //! Set the database
      /*!
       \param db The database object.
       */
      void SetDatabase(void* db);
      
      //! Prepare a Query.
      /*!
       \param query The query to execute.
       \param ... The parameters to substitute on the query.
       \return True if there were no errors.
       */
      bool Prepare(const char* qry, ...);
      
      //! Executes a query, and then frees up any memory used.
      /*!
       \param query The query to execute.
       \param ... The parameters to substitute on the query.
       \return True if there were no errors.
       */
      bool Execute(const char* qry, ...);
      
      //! Reset the current query.
      void Reset();
      
      //! Reads the next value.
      /*!
       \return True if a new record was readed.
       */
      bool Next();
      
      //! Cancel the declaration of a query.
      void Cancel();
      
      //! Get the column type.
      /*!
       \param idx The column index, starting from 0.
       \return The column type.
       */
      ColumnType GetType(int idx);
      
      //! Get the size in bytes of a column.
      /*!
       \brief If the data is a string, the size would not cover the zero terminated character.
       \param idx The column index, starting from 0.
       \return The column size in bytes.
       */
      int GetSize(int idx);
      
      //! Get the integer data of a column.
      /*!
       \param idx The column index, starting from 0.
       \return The integer value.
       */       
      int GetInt(int idx);
      
      //! Get the double data of a column.
      /*!
       \param idx The column index, starting from 0.
       \return The double value.
       */       
      double GetDouble(int idx);
      
      //! Get the raw text data data of a column.
      /*!
       \brief This does not includes the zero termination character. For that you must use GetSize. Please, do NOT free the memory returned, it will be freed up automatically.
       \param idx The column index, starting from 0.
       \return The text chars value.
       \seealso GetSize.
       */       
      const unsigned char* GetRawText(int idx);
      
      //! Get the raw byte data data of a column.
      /*!
       \brief To determine the data size you must use GetSize. Please, do NOT free the memory returned, it will be freed up automatically.
       \param idx The column index, starting from 0.
       \return The raw bytes value.
       \seealso GetSize.
       */       
      const void* GetRawData(int idx);
      
      
      //! Get the string data of a column.
      /*!
       \param idx The column index, starting from 0.
       \param The string to output the data.
       */       
      void GetString(int idx, Str & str);
      
      
    private:
      
      //! The statement.
      sqlite3_stmt *query;
      
      //! The sqlite3 database.
      sqlite3 *database;
      
    };
    
    //! A sqlite3 database.
    class Database {
    public:
      
      //! Constructs a new Database.
      /*!
       \param name File name of the database.
       */
      Database(const char* name);
      
      //! Destructor.
      ~Database();
      
      //! Creates a new query.
      DbQuery* NewQuery();
      
    private:
      
      //! Sqlite3 database.
      sqlite3* db;
      
    };
    
  }
  
}
