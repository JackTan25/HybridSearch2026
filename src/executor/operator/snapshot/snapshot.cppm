

module;

export module snapshot;

import stl;
import status;
import snapshot_info;
import query_context;

namespace hybridsearch {

export class Snapshot {
public:
    static Status CreateTableSnapshot(QueryContext *query_context, const String &snapshot_name, const String& table_name);
    static Status RestoreTableSnapshot(QueryContext *query_context, const String &snapshot_name);
    static Status DropSnapshot(QueryContext *query_context, const String &snapshot_name);
};

} // namespace hybridsearch
