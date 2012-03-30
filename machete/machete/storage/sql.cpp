//
//  sql.cpp
//  machete
//
//  Created by Hilario Perez Corona on 11/20/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "sql.h"

namespace machete {
  namespace storage {
    
    DbQuery::DbQuery() {
      database = NULL;
      query = NULL;
    }
    
    DbQuery::~DbQuery() {
      Cancel();
    }
    
    void DbQuery::SetDatabase(void *db) {
      database = static_cast<sqlite3*>(db);
    }
    
    bool DbQuery::Prepare(const char *qry, ...) {
      Cancel();
      
      va_list args;
      va_start(args, qry);
      char* pqry = sqlite3_vmprintf(qry, args);
      va_end(args);
      
      int result = sqlite3_prepare(database, pqry, -1, &query, NULL);
      
#ifdef MACHETE_STORAGE_DEBUG
      if (result != SQLITE_OK) {
        std::cout << "ERROR IN SQL:" << std::endl << "\t" << pqry << std::endl;
      } else {
        std::cout << "SUCCESS:" << std::endl << "\t" << pqry << std::endl;
      }
#endif
      
      return result == SQLITE_OK;
    }
    
    bool DbQuery::Execute(const char *qry, ...) {
      va_list args;
      va_start(args, qry);
      char* pqry = sqlite3_vmprintf(qry, args);
      va_end(args);
      
      int result = sqlite3_exec(database, pqry, NULL, NULL, NULL);
      
#ifdef MACHETE_STORAGE_DEBUG
      if (result != SQLITE_OK) {
        std::cout << "ERROR IN SQL:" << std::endl << "\t" << pqry << std::endl;
      } else {
        std::cout << "SUCCESS:" << std::endl << "\t" << pqry << std::endl;
      }
#endif
      
      return result == SQLITE_OK;
    }
    
    void DbQuery::Reset() {
      if (query == NULL) return;
      
      sqlite3_reset(query);
    }
    
    bool DbQuery::Next() {
      if (query == NULL) return false;
      
      return sqlite3_step(query) == SQLITE_ROW;
    }
    
    void DbQuery::Cancel() {
      if (query != NULL) {
        sqlite3_finalize(query);
        query = NULL;
      }
    }
    
    ColumnType DbQuery::GetType(int idx)  {
      return (ColumnType)sqlite3_column_type(query, idx);
    }
    
    int DbQuery::GetSize(int idx) {
      return sqlite3_column_bytes(query, idx);
    }
    
    int DbQuery::GetInt(int idx) {
      return sqlite3_column_int(query, idx);
    }
    
    double DbQuery::GetDouble(int idx) {
      return sqlite3_column_double(query, idx);
    }
    
    const unsigned char* DbQuery::GetRawText(int idx) {
      return sqlite3_column_text(query, idx);
    }
    
    const void* DbQuery::GetRawData(int idx) {
      return sqlite3_column_blob(query, idx);
    }
    
    void DbQuery::GetString(int idx, Str & str) {
      const unsigned char *txt = sqlite3_column_text(query, idx);
      int size = sqlite3_column_bytes(query, idx);
      
      str.Ensure(size);
      str.Assign((const char*)txt, size);
    }
    
    Database::Database(const char* name) {
      db = NULL;
      
      char *full = ThePlatform->WritableFile(name);
      sqlite3_open(full, &db);
      
      delete full;
    }
    
    Database::~Database() {
      sqlite3_close(db);
    }
    
    DbQuery* Database::NewQuery() {
      DbQuery* q = new DbQuery();
      q->SetDatabase(db);
      
      return q;
    }
    
  }
}
