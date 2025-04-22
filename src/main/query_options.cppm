

export module query_options;

import stl;
import extra_ddl_info;
import statement_common;

namespace hybridsearch {

export class CreateDatabaseOptions {
public:
    ConflictType conflict_type_{ConflictType::kError};
};

export class DropDatabaseOptions {
public:
    ConflictType conflict_type_{ConflictType::kError};
};

export class CreateTableOptions {
public:
    ConflictType conflict_type_{ConflictType::kError};
    Vector<InitParameter *> properties_;
};

export class DropTableOptions {
public:
    ConflictType conflict_type_{ConflictType::kError};
};

export class CreateIndexOptions {
public:
    ConflictType conflict_type_{ConflictType::kError};
};

export class DropIndexOptions {
public:
    ConflictType conflict_type_{ConflictType::kError};
};

export class ImportOptions {
public:
    char delimiter_{','};
    bool header_{false};
    CopyFileType copy_file_type_{CopyFileType::kInvalid};
};

export class ExportOptions {
public:
    char delimiter_{','};
    bool header_{false};
    SizeT offset_{0};
    SizeT limit_{0};
    SizeT row_limit_{0};
    CopyFileType copy_file_type_{CopyFileType::kInvalid};
};

export class OptimizeOptions {
public:
    String index_name_{};
    Vector<InitParameter *> opt_params_{};
};

} // namespace hybridsearch
