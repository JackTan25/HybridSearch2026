

#include "gtest/gtest.h"
import base_test;

import stl;
import hybridsearch;
import query_result;
import data_block;
import value;
import query_options;

import status;
import third_party;
import logical_type;
import internal_types;
import embedding_info;
import parsed_expr;
import column_expr;
import extra_ddl_info;
import knn_expr;
import column_def;
import explain_statement;
import data_type;

using namespace hybridsearch;
class hybridsearchTableTest : public BaseTest {};

TEST_F(hybridsearchTableTest, test1) {
    using namespace hybridsearch;

    String path = GetHomeDir();
    RemoveDbDirs();
    hybridsearch::LocalInit(path);

    SharedPtr<hybridsearch> hybridsearch = hybridsearch::LocalConnect();
    {
        CreateDatabaseOptions create_db_opts;
        hybridsearch->CreateDatabase("db1", create_db_opts, "");

        QueryResult result = hybridsearch->GetDatabase("db1");
        EXPECT_TRUE(result.IsOk());
    }

    {
        std::vector<ColumnDef *> column_defs;

        // init column defs
        std::shared_ptr<DataType> col1_type =
            std::make_shared<DataType>(LogicalType::kEmbedding, std::make_shared<EmbeddingInfo>(EmbeddingDataType::kElemFloat, 128));
        String col1_name = "col1";
        auto col1_def = std::make_unique<ColumnDef>(0, col1_type, col1_name, std::set<ConstraintType>());
        column_defs.emplace_back(col1_def.release());

        std::shared_ptr<DataType> col2_type = std::make_shared<DataType>(LogicalType::kBigInt);
        String col2_name = "col2";
        auto col2_def = std::make_unique<ColumnDef>(0, col2_type, col2_name, std::set<ConstraintType>());
        column_defs.emplace_back(col2_def.release());

        std::string db_name = "db1";
        std::string table_name = "knn_table";

        CreateTableOptions create_tb_options;
        create_tb_options.conflict_type_ = ConflictType::kIgnore;

        QueryResult create_result =
            hybridsearch->CreateTable(db_name, table_name, std::move(column_defs), std::vector<TableConstraint *>{}, std::move(create_tb_options));
        EXPECT_TRUE(create_result.IsOk());

        {
            Vector<ParsedExpr *> *output_columns = new Vector<ParsedExpr *>();
            ColumnExpr *col2 = new ColumnExpr();
            col2->names_.emplace_back(col2_name);
            output_columns->emplace_back(col2);

            QueryResult explain_ast =
                hybridsearch
                    ->Explain(db_name, table_name, ExplainType::kAst, nullptr, nullptr, nullptr, nullptr, output_columns, nullptr, nullptr, nullptr, nullptr);
            EXPECT_TRUE(explain_ast.IsOk());
            //            fmt::print("AST: {}\n", explain_ast.ToString());
        }

        {
            Vector<ParsedExpr *> *output_columns = new Vector<ParsedExpr *>();
            ColumnExpr *col2 = new ColumnExpr();
            col2->names_.emplace_back(col2_name);
            output_columns->emplace_back(col2);

            QueryResult explain_unopt = hybridsearch->Explain(db_name,
                                                          table_name,
                                                          ExplainType::kUnOpt,
                                                          nullptr,
                                                          nullptr,
                                                          nullptr,
                                                          nullptr,
                                                          output_columns,
                                                          nullptr,
                                                          nullptr,
                                                          nullptr,
                                                          nullptr);
            EXPECT_TRUE(explain_unopt.IsOk());
            //            fmt::print("Unoptimized logical plan: {}\n", explain_unopt.ToString());
        }

        {
            Vector<ParsedExpr *> *output_columns = new Vector<ParsedExpr *>();
            ColumnExpr *col2 = new ColumnExpr();
            col2->names_.emplace_back(col2_name);
            output_columns->emplace_back(col2);

            QueryResult explain_optimized_logical =
                hybridsearch
                    ->Explain(db_name, table_name, ExplainType::kOpt, nullptr, nullptr, nullptr, nullptr, output_columns, nullptr, nullptr, nullptr, nullptr);

            EXPECT_TRUE(explain_optimized_logical.IsOk());
            //            fmt::print("Optimized logical plan: {}\n", explain_opt.ToString());
        }

        {
            Vector<ParsedExpr *> *output_columns = new Vector<ParsedExpr *>();
            ColumnExpr *col2 = new ColumnExpr();
            col2->names_.emplace_back(col2_name);
            output_columns->emplace_back(col2);

            QueryResult explain_phy = hybridsearch->Explain(db_name,
                                                        table_name,
                                                        ExplainType::kPhysical,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        output_columns,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr);
            EXPECT_TRUE(explain_phy.IsOk());
            //            fmt::print("Physical plan: {}\n", explain_phy.ToString());
        }

        {
            Vector<ParsedExpr *> *output_columns = new Vector<ParsedExpr *>();
            ColumnExpr *col2 = new ColumnExpr();
            col2->names_.emplace_back(col2_name);
            output_columns->emplace_back(col2);

            QueryResult explain_fragment = hybridsearch->Explain(db_name,
                                                             table_name,
                                                             ExplainType::kFragment,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             output_columns,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr);
            EXPECT_TRUE(explain_fragment.IsOk());
            //            fmt::print("Fragment: {}\n", explain_fragment.ToString());
        }

        {
            Vector<ParsedExpr *> *output_columns = new Vector<ParsedExpr *>();
            ColumnExpr *col2 = new ColumnExpr();
            col2->names_.emplace_back(col2_name);
            output_columns->emplace_back(col2);

            QueryResult explain_pipeline = hybridsearch->Explain(db_name,
                                                             table_name,
                                                             ExplainType::kPipeline,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             output_columns,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr);
            EXPECT_TRUE(explain_pipeline.IsOk());
            //            fmt::print("Pipeline: {}\n", explain_pipeline.ToString());
        }
    }

    hybridsearch->LocalDisconnect();
    hybridsearch::LocalUnInit();
}
