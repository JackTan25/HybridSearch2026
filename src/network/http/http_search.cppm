

module;

export module http_search;

import stl;
import status;
import third_party;
import parsed_expr;
import knn_expr;
import match_expr;
import fusion_expr;
import match_tensor_expr;
import match_sparse_expr;
import hybridsearch;
import internal_types;
import constant_expr;
import search_expr;
import select_statement;

namespace hybridsearch {

export class HTTPSearch {
public:
    static void Process(hybridsearch *hybridsearch_ptr,
                        const String &db_name,
                        const String &table_name,
                        const String &input_json,
                        HTTPStatus &http_status,
                        nlohmann::json &response);
    static void Explain(hybridsearch *hybridsearch_ptr,
                        const String &db_name,
                        const String &table_name,
                        const String &input_json,
                        HTTPStatus &http_status,
                        nlohmann::json &response);

    static Vector<ParsedExpr *> *ParseOutput(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static Vector<OrderByExpr *> *ParseSort(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static UniquePtr<ParsedExpr> ParseFilter(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static UniquePtr<SearchExpr> ParseSearchExpr(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static UniquePtr<FusionExpr> ParseFusion(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static UniquePtr<KnnExpr> ParseMatchDense(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static UniquePtr<MatchExpr> ParseMatchText(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static UniquePtr<MatchTensorExpr> ParseMatchTensor(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static UniquePtr<MatchSparseExpr> ParseMatchSparse(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
    static Tuple<i64, void *>
    ParseVector(const nlohmann::json &json_object, EmbeddingDataType elem_type, HTTPStatus &http_status, nlohmann::json &response);
    static UniquePtr<ConstantExpr> ParseSparseVector(const nlohmann::json &json_object, HTTPStatus &http_status, nlohmann::json &response);
};

} // namespace hybridsearch
