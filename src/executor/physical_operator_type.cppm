

module;

export module physical_operator_type;

import stl;

namespace hybridsearch {

export enum class PhysicalOperatorType : i8 {
    kInvalid = 0,

    kAggregate,
    kMergeAggregate,

    kParallelAggregate,
    kMergeParallelAggregate,

    kUnionAll,
    kIntersect,
    kExcept,

    kTableScan,
    kFilter,
    kUnnest,
    kUnnestAggregate,
    kIndexScan,
    kDummyScan,
    kKnnScan,
    kMergeKnn,
    kMatchTensorScan,
    kMergeMatchTensor,
    kMatchSparseScan,
    kMergeMatchSparse,
    kMatch,
    kFusion,

    kHash,
    kMergeHash,

    // Join
    kJoinHash,
    kJoinNestedLoop,
    kJoinMerge,
    kJoinIndex,
    kCrossProduct,

    // Result modifier
    kLimit,
    kMergeLimit,

    kTop,
    kMergeTop,

    kProjection,

    kSort,
    kMergeSort,

    // DML
    kDelete,
    kUpdate,
    kInsert,
    kImport,
    kExport,

    // DDL
    kAlter,
    kCreateTable,
    kCreateCollection,
    kCreateDatabase,
    kCreateView,
    kDropTable,
    kDropIndex,
    kDropCollection,
    kDropDatabase,
    kDropView,

    kCreateIndexPrepare,
    kCreateIndexDo,
    kCreateIndexFinish,

    // misc
    kExplain,
    kPreparedPlan,
    kShow,
    kFlush,
    kOptimize,
    kCommand,

    // Compact
    kCompact,
    kCompactIndexPrepare,
    kCompactIndexDo,
    kCompactFinish,

    kReadCache,

    kSink,
    kSource,
};

export String PhysicalOperatorToString(PhysicalOperatorType type);

} // namespace hybridsearch
