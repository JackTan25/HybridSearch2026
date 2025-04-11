

module;

import stl;

export module logical_node_type;

namespace hybridsearch {

// Logical operator types used in logical plan
export enum class LogicalNodeType : u8 {
    kInvalid = 0,
    kAggregate,
    kExcept,
    kUnion,
    kIntersect,
    kJoin,
    kCrossProduct,
    kLimit,
    kFilter,
    kProjection,
    kSort,
    kTop,
    kUnnest,
    kUnnestAggregate,

    // DML
    kDelete,
    kUpdate,
    kInsert,
    kImport,
    kExport,

    // DDL
    kAlter,
    kCreateTable,
    kCreateIndex,
    kCreateCollection,
    kCreateSchema,
    kCreateView,
    kDropTable,
    kDropIndex,
    kDropCollection,
    kDropSchema,
    kDropView,

    // Search
    kFusion,
    kMatch,

    // Scan
    kTableScan,
    kIndexScan,
    kViewScan,
    kDummyScan,
    kKnnScan,
    kMatchTensorScan,
    kMatchSparseScan,

    // ?
    kShow,
    kExplain,
    kCommand,
    kPrepare,
    kFlush,
    kOptimize,

    // Compact
    kCompact,
    kCompactIndex,
    kCompactFinish,

    kReadCache,
    kMock,
};
} // namespace hybridsearch
