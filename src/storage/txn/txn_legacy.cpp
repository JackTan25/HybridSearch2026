

//module;
//
//module txn;
//
//namespace hybridsearch {
//
//Status Txn::CreateDatabaseInternalLegacy(const SharedPtr<String> &db_name, ConflictType conflict_type, const SharedPtr<String> &comment) {
//    return Status::OK();
//}
//Status DropDatabaseInternalLegacy(const String &db_name, ConflictType conflict_type) { return Status::OK(); }
//Tuple<DBEntry *, Status> GetDatabaseInternalLegacy(const String &db_name) { return {nullptr, Status::OK()}; }
//Tuple<SharedPtr<DatabaseInfo>, Status> GetDatabaseInfoInternalLegacy(const String &db_name) { return {nullptr, Status::OK()}; }
//Vector<DatabaseDetail> ListDatabasesInternalLegacy() { return {}; }
//
//} // namespace hybridsearch
