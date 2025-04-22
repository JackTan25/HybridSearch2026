

module;

#include "query_node.h"

export module query_node;

namespace hybridsearch {

export using hybridsearch::QueryNodeType;
export using hybridsearch::QueryNodeTypeToString;
export using hybridsearch::CreateSearchParams;
export using hybridsearch::QueryNode;
export using hybridsearch::TermQueryNode;
export using hybridsearch::MultiQueryNode;
export using hybridsearch::AndQueryNode;
export using hybridsearch::AndNotQueryNode;
export using hybridsearch::OrQueryNode;
export using hybridsearch::NotQueryNode;
export using hybridsearch::PhraseQueryNode;

// unimplemented
// export using hybridsearch::WandQueryNode;
// export using hybridsearch::PrefixTermQueryNode;
// export using hybridsearch::SuffixTermQueryNode;
// export using hybridsearch::SubstringTermQueryNode;

} // namespace hybridsearch
