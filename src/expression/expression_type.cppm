

module;

export module expression_type;

namespace hybridsearch {

export enum class ExpressionType {
    // Aggregate function
    kAggregate,
    kGroupingFunction, // ??

    // Arithmetic operator
    kArithmetic,

    // Cast operator
    kCast,

    // Case operator
    kCase,

    // Conjunction operator
    kAnd,
    kConjunction,
    kOr,
    kNot,

    // Column
    kColumn,
    kReference,

    // Correlated column expression
    kCorrelatedColumn,

    // Exists
    kExists,

    // Extract
    kExtract,

    // Interval
    kInterval,

    // Function,
    kFunction,

    // List,
    kList,

    // Logical,
    kLogical,
    kEqual,
    kNotEqual, // kNot + kEqual ?
    kLessThan,
    kGreaterThan,
    kLessThanEqual,
    kGreaterThanEqual,
    kBetween,
    kNotBetween, // kNot + kBetween ?

    // Unbound subquery
    kSubQuery,

    // Unary
    kUnaryMinus,
    kIsNull,
    kIsNotNull, // kNot + kIsNull ?

    // Value
    kValue,
    kDefault,   // ??
    kParameter, // ??

    // IN
    kIn,
    kNotIn, // kNot + kIn

    // WINDOW Function
    kWindowRank,
    kWindowRowNumber,

    // Search
    kSearch,
    kMatch,
    kFusion,
    kMatchTensor,
    kMatchSparse,

    // Misc.
    kKnn,
    kDistinctFrom,    // ??
    kNotDistinctFrom, // ??
    kPlaceholder,
    kPredicate, // ?
    kRaw,

    // Fulltext filter
    kFilterFullText,

    // Unnest
    kUnnest,
};
}