

//module;
//
//module txn;
//
//namespace hybridsearch {
//
//Status Txn::CreateDatabaseInternal(const SharedPtr<String> &db_name, ConflictType conflict_type, const SharedPtr<String> &comment) {
//    return Status::OK();
//}
//Status DropDatabaseInternal(const String &db_name, ConflictType conflict_type) { return Status::OK(); }
//Tuple<DBEntry *, Status> GetDatabaseInternal(const String &db_name) { return {nullptr, Status::OK()}; }
//Tuple<SharedPtr<DatabaseInfo>, Status> GetDatabaseInfoInternal(const String &db_name) { return {nullptr, Status::OK()}; }
//Vector<DatabaseDetail> ListDatabasesInternal() { return {}; }
//
//}