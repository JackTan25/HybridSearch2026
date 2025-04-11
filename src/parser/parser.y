
%{
#include <stdint.h>
#include <stdio.h>
#include "parser.h"
#include "lexer.h"

void yyerror(YYLTYPE * llocp, void* lexer, hybridsearch::ParserResult* result, const char* msg);
%}

%code requires {

#include "expression.h"
#include "parser_result.h"
#include "defer_operation.h"
#include "statement/alter_statement.h"
#include "statement/copy_statement.h"
#include "statement/create_statement.h"
#include "statement/delete_statement.h"
#include "statement/drop_statement.h"
#include "statement/execute_statement.h"
#include "statement/explain_statement.h"
#include "statement/flush_statement.h"
#include "statement/optimize_statement.h"
#include "statement/insert_statement.h"
#include "statement/prepare_statement.h"
#include "statement/select_statement.h"
#include "statement/show_statement.h"
#include "statement/update_statement.h"
#include "statement/command_statement.h"
#include "statement/compact_statement.h"
#include "statement/admin_statement.h"
#include "table_reference/base_table_reference.h"
#include "table_reference/join_reference.h"
#include "table_reference/cross_product_reference.h"
#include "table_reference/table_reference.h"
#include "table_reference/subquery_reference.h"
#include "parser_helper.h"

#include <vector>

#define YYSTYPE SQLSTYPE
#define YYLTYPE SQLLTYPE

struct SQL_LTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;

    int total_column;
    int string_length;

    std::vector<void*> parameters;
};

#define SQLLTYPE SQL_LTYPE
#define SQLLTYPE_IS_DECLARED 1

#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line;      \
    yylloc->first_column = yylloc->last_column;  \
    for(int i = 0; yytext[i] != '\0'; ++ i) {   \
        yylloc->total_column++;                   \
        yylloc->string_length++;                  \
        if (yytext[i] == '\n') {                  \
            yylloc->last_line++;                    \
            yylloc->last_column = 0;                \
        } else {                                  \
            yylloc->last_column++;                  \
        }                                         \
    }
}


%define api.pure full

%define parse.trace
%define parse.error verbose

%define api.prefix {sql}

%lex-param {void *scanner}
%parse-param {void *scanner} {hybridsearch::ParserResult* result}

%locations
%initial-action {
  // Initialize
  @$.first_column = 0;
  @$.last_column = 0;
  @$.first_line = 0;
  @$.last_line = 0;
  @$.total_column = 0;
  @$.string_length = 0;
};

%code{
// Avoid warnings with the error counter.
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
}

%union {
    bool    bool_value;
    char*   str_value;
    double  double_value;
    int64_t long_value;

    hybridsearch::BaseStatement*   base_stmt;
    hybridsearch::SelectStatement* select_stmt;
    hybridsearch::CopyStatement*   copy_stmt;
    hybridsearch::InsertStatement* insert_stmt;
    hybridsearch::UpdateStatement* update_stmt;
    hybridsearch::DeleteStatement* delete_stmt;
    hybridsearch::CreateStatement* create_stmt;
    hybridsearch::DropStatement*   drop_stmt;
    hybridsearch::PrepareStatement* prepare_stmt;
    hybridsearch::ExecuteStatement* execute_stmt;
    hybridsearch::AlterStatement*   alter_stmt;
    hybridsearch::ShowStatement*    show_stmt;
    hybridsearch::ExplainStatement* explain_stmt;
    hybridsearch::FlushStatement*  flush_stmt;
    hybridsearch::OptimizeStatement*  optimize_stmt;
    hybridsearch::CommandStatement* command_stmt;
    hybridsearch::CompactStatement* compact_stmt;
    hybridsearch::AdminStatement* admin_stmt;

    std::vector<hybridsearch::BaseStatement*>* stmt_array;

    std::vector<hybridsearch::TableElement*>*  table_element_array_t;
    std::vector<hybridsearch::ColumnDef*>*     column_def_array_t;
    hybridsearch::TableElement*           table_element_t;
    hybridsearch::ColumnDef*              table_column_t;
    hybridsearch::ColumnType*              column_type_t;
    std::vector<std::unique_ptr<hybridsearch::ColumnType>>*     column_type_array_t;
    hybridsearch::ConstraintType          column_constraint_t;
    std::set<hybridsearch::ConstraintType>* column_constraints_t;
    std::vector<std::string>*         identifier_array_t;
    hybridsearch::TableConstraint*        table_constraint_t;

    hybridsearch::BaseTableReference*     table_reference_t;
    hybridsearch::TableAlias *            table_alias_t;
    hybridsearch::JoinType                join_type_t;

    hybridsearch::OrderByExpr*            order_by_expr_t;
    std::vector<hybridsearch::OrderByExpr*>*   order_by_expr_list_t;
    hybridsearch::OrderType               order_by_type_t;

    hybridsearch::WithExpr*               with_expr_t;
    std::vector<hybridsearch::WithExpr*>*      with_expr_list_t;

    hybridsearch::SetOperatorType         set_operator_t;

    hybridsearch::ExplainType             explain_type_t;

    hybridsearch::ParsedExpr*             expr_t;
    hybridsearch::ConstantExpr*           const_expr_t;
    std::vector<hybridsearch::ParsedExpr*>*    expr_array_t;
    std::vector<hybridsearch::InsertRowExpr*>*    insert_row_list_t;

    std::vector<hybridsearch::WhenThen*>*     case_check_array_t;

    hybridsearch::UpdateExpr*             update_expr_t;
    std::vector<hybridsearch::UpdateExpr*>*    update_expr_array_t;

    hybridsearch::TableName* table_name_t;
    hybridsearch::CopyOption* copy_option_t;
    std::vector<hybridsearch::CopyOption*>* copy_option_array;

    hybridsearch::InitParameter*        index_param_t;
    std::vector<hybridsearch::InitParameter*>* index_param_list_t;
    std::vector<hybridsearch::InitParameter*>* with_index_param_list_t;

    hybridsearch::IndexInfo* index_info_t;

    // hybridsearch::IfExistsInfo*        if_exists_info_t;
    hybridsearch::IfNotExistsInfo*     if_not_exists_info_t;

    std::pair<int64_t, int64_t>*    int_sparse_ele_t;
    std::pair<int64_t, double>*     float_sparse_ele_t;
}

%destructor {
    fprintf(stderr, "destroy column_type\n");
    delete ($$);
} <column_type_t>

%destructor {
    fprintf(stderr, "destroy column_type_array\n");
    delete ($$);
} <column_type_array_t>

%destructor {
    fprintf(stderr, "destroy index info\n");
    if (($$) != nullptr) {
        delete ($$);
    }
} <index_info_t>

%destructor {
    fprintf(stderr, "destroy create index param list\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <with_index_param_list_t>

/* %destructor {
    fprintf(stderr, "destroy if exists info\n");
    if (($$) != nullptr) {
        delete ($$);
    }
} <if_exists_info_t> */

%destructor {
    fprintf(stderr, "destroy if not exists info\n");
    if (($$) != nullptr) {
        delete ($$);
    }
} <if_not_exists_info_t>

%destructor {
    fprintf(stderr, "destroy table element array\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <table_element_array_t>

%destructor {
    fprintf(stderr, "destroy column def array\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <column_def_array_t>

%destructor {
    fprintf(stderr, "destroy statement array\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <stmt_array>

%destructor {
    fprintf(stderr, "destroy expression array\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <expr_array_t>

%destructor {
    fprintf(stderr, "destroy insert row list\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <insert_row_list_t>

%destructor {
    fprintf(stderr, "destroy order by expr list\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <order_by_expr_list_t>

%destructor {
    fprintf(stderr, "destroy update expr\n");
    if($$ != nullptr) {
        delete $$;
    }
} <update_expr_t>

%destructor {
    fprintf(stderr, "destroy update expr array\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <update_expr_array_t>

%destructor {
    fprintf(stderr, "destroy with expr list\n");
    if (($$) != nullptr) {
        for (auto ptr : *($$)) {
            delete ptr;
        }
        delete ($$);
    }
} <with_expr_list_t>

%destructor {
    fprintf(stderr, "destroy table table_name\n");
    if (($$) != nullptr) {
        free($$->schema_name_ptr_);
        free($$->table_name_ptr_);
        delete ($$);
    }
} <table_name_t>

%destructor {
    fprintf(stderr, "destroy constraints\n");
    if (($$) != nullptr) {
        delete ($$);
    }
} <column_constraints_t>

%destructor {
    free($$);
} <str_value>

%destructor {
    fprintf(stderr, "destroy identifier array\n");
    delete ($$);
} <identifier_array_t>

%destructor {
    delete ($$);
} <expr_t>

%destructor {
    delete ($$);
} <const_expr_t>

%destructor {
    fprintf(stderr, "destroy table alias\n");
    delete ($$);
} <table_alias_t>

%destructor {
    fprintf(stderr, "destroy table reference\n");
    delete ($$);
} <table_reference_t>

%destructor {
    fprintf(stderr, "destroy order by expr\n");
    delete $$->expr_;
    delete $$;
} <order_by_expr_t>

%destructor {
    fprintf(stderr, "destroy with expr\n");
    delete $$->select_;
    delete $$;
} <with_expr_t>

%destructor {
    if($$ != nullptr) {
        delete $$;
    }
} <select_stmt>

%destructor {
    fprintf(stderr, "destroy case check array\n");
    if($$ != nullptr) {
        for(auto ptr: *($$)) {
            delete ptr;
        }
    }
} <case_check_array_t>

%destructor {
    delete ($$);
} <int_sparse_ele_t>

%destructor {
    delete ($$);
} <float_sparse_ele_t>

%token <str_value>              IDENTIFIER STRING
%token <double_value>           DOUBLE_VALUE
%token <long_value>             LONG_VALUE

/* SQL keywords */

%token CREATE SELECT INSERT DROP UPDATE DELETE COPY SET EXPLAIN SHOW ALTER EXECUTE PREPARE UNION ALL INTERSECT COMPACT LOCK UNLOCK ADD RENAME
%token EXCEPT FLUSH USE OPTIMIZE PROPERTIES
%token DATABASE TABLE COLLECTION TABLES INTO VALUES VIEW INDEX VIEWS DATABASES SEGMENT SEGMENTS BLOCK BLOCKS COLUMN COLUMNS INDEXES CHUNK SYSTEM
%token GROUP BY HAVING AS NATURAL JOIN LEFT RIGHT OUTER FULL ON INNER CROSS DISTINCT WHERE ORDER LIMIT OFFSET ASC DESC
%token IF NOT EXISTS IN FROM TO WITH DELIMITER FORMAT HEADER HIGHLIGHT CAST END CASE ELSE THEN WHEN
%token BOOLEAN INTEGER INT TINYINT SMALLINT BIGINT HUGEINT VARCHAR FLOAT DOUBLE REAL DECIMAL DATE TIME DATETIME FLOAT16 BFLOAT16 UNSIGNED
%token TIMESTAMP UUID POINT LINE LSEG BOX PATH POLYGON CIRCLE BLOB BITMAP
%token ARRAY TUPLE EMBEDDING VECTOR MULTIVECTOR TENSOR SPARSE TENSORARRAY BIT TEXT
%token PRIMARY KEY UNIQUE NULLABLE IS DEFAULT COMMENT IGNORE
%token TRUE FALSE INTERVAL SECOND SECONDS MINUTE MINUTES HOUR HOURS DAY DAYS MONTH MONTHS YEAR YEARS
%token EQUAL NOT_EQ LESS_EQ GREATER_EQ BETWEEN AND OR EXTRACT LIKE
%token DATA LOG BUFFER TRANSACTIONS TRANSACTION MEMINDEX
%token USING SESSION GLOBAL OFF EXPORT CONFIGS CONFIG PROFILES VARIABLES VARIABLE DELTA LOGS CATALOGS CATALOG
%token SEARCH MATCH MAXSIM QUERY QUERIES FUSION ROWLIMIT
%token ADMIN LEADER FOLLOWER LEARNER CONNECT STANDALONE NODES NODE REMOVE SNAPSHOT SNAPSHOTS RECOVER RESTORE
%token PERSISTENCE OBJECT OBJECTS FILES MEMORY ALLOCATION HISTORY

%token NUMBER

/* nonterminal symbol */

%type <base_stmt>         statement explainable_statement
%type <create_stmt>       create_statement
%type <drop_stmt>         drop_statement
%type <copy_stmt>         copy_statement
%type <show_stmt>         show_statement
%type <select_stmt>       select_clause_without_modifier select_without_paren select_with_paren select_statement
%type <select_stmt>       select_clause_without_modifier_paren select_clause_with_modifier
%type <delete_stmt>       delete_statement
%type <update_stmt>       update_statement
%type <insert_stmt>       insert_statement
%type <explain_stmt>      explain_statement
%type <flush_stmt>        flush_statement
%type <optimize_stmt>     optimize_statement
%type <command_stmt>      command_statement
%type <compact_stmt>      compact_statement
%type <admin_stmt>        admin_statement
%type <alter_stmt>        alter_statement

%type <stmt_array>        statement_list

%type <table_element_t>         table_element
%type <table_column_t>          table_column
%type <column_type_t>           column_type
%type <column_type_array_t>     column_type_array
%type <identifier_array_t>      identifier_array optional_identifier_array
%type <table_constraint_t>      table_constraint
%type <column_constraint_t>     column_constraint
%type <column_constraints_t>    column_constraints

%type <table_reference_t>       table_reference table_reference_unit table_reference_name from_clause join_clause
%type <table_alias_t>           table_alias
%type <join_type_t>             join_type
%type <order_by_type_t>         order_by_type
%type <order_by_expr_t>         order_by_expr
%type <order_by_expr_list_t>    order_by_expr_list order_by_clause
%type <with_expr_t>             with_expr
%type <with_expr_list_t>        with_expr_list with_clause
%type <set_operator_t>          set_operator

%type <expr_t>                  expr expr_alias column_expr function_expr subquery_expr match_vector_expr match_tensor_expr match_sparse_expr sub_search
%type <expr_t>                  having_clause where_clause limit_expr offset_expr operand in_expr between_expr
%type <expr_t>                  conjunction_expr cast_expr case_expr
%type <expr_t>                  match_text_expr query_expr fusion_expr search_clause optional_search_filter_expr
%type <const_expr_t>            constant_expr interval_expr default_expr
%type <const_expr_t>            array_expr long_array_expr unclosed_long_array_expr double_array_expr unclosed_double_array_expr
%type <const_expr_t>            curly_brackets_expr unclosed_curly_brackets_expr
%type <const_expr_t>            common_array_expr subarray_array_expr unclosed_subarray_array_expr
%type <const_expr_t>            sparse_array_expr long_sparse_array_expr unclosed_long_sparse_array_expr double_sparse_array_expr unclosed_double_sparse_array_expr
%type <const_expr_t>            empty_array_expr common_sparse_array_expr
%type <int_sparse_ele_t>        int_sparse_ele
%type <float_sparse_ele_t>      float_sparse_ele
%type <expr_array_t>            expr_array group_by_clause sub_search_array highlight_clause
%type <insert_row_list_t>       insert_row_list
%type <update_expr_t>           update_expr;
%type <update_expr_array_t>     update_expr_array;
%type <case_check_array_t>      case_check_array;

%type <table_element_array_t>   table_element_array
%type <column_def_array_t>      column_def_array

%type <table_name_t>      table_name
%type <copy_option_array> copy_option_list
%type <copy_option_t>     copy_option

%type <str_value>         file_path

%type <bool_value>        if_not_exists if_exists distinct

%type <index_param_t> index_param
%type <index_param_list_t> index_param_list
%type <with_index_param_list_t> with_index_param_list optional_table_properties_list

%type <index_info_t> index_info

/* %type <if_exists_info_t> if_exists_info */
%type <if_not_exists_info_t> if_not_exists_info 

/*
 * Operator precedence, low to high
 */

%left       OR
%left       AND
%left       BETWEEN CASE WHEN THEN ELSE
%right      NOT

%nonassoc   '=' EQUAL NOT_EQ LIKE
%nonassoc   '<' '>' LESS_EQ GREATER_EQ

%nonassoc   IS
%left       '+' '-'
%left       '*' '/' '%'

%left       '[' ']'
%left       '(' ')'
%left       '.'
%left       JOIN

%%

input_pattern : statement_list semicolon {
    result->statements_ptr_ = $1;
};

statement_list : statement {
    $1->stmt_length_ = yylloc.string_length;
    yylloc.string_length = 0;
    $$ = new std::vector<hybridsearch::BaseStatement*>();
    $$->push_back($1);
}
| statement_list ';' statement {
    $3->stmt_length_ = yylloc.string_length;
    yylloc.string_length = 0;
    $1->push_back($3);
    $$ = $1;
};

statement : create_statement { $$ = $1; }
| drop_statement { $$ = $1; }
| copy_statement { $$ = $1; }
| show_statement { $$ = $1; }
| select_statement { $$ = $1; }
| delete_statement { $$ = $1; }
| update_statement { $$ = $1; }
| insert_statement { $$ = $1; }
| explain_statement { $$ = $1; }
| flush_statement { $$ = $1; }
| optimize_statement { $$ = $1; }
| command_statement { $$ = $1; }
| compact_statement { $$ = $1; }
| admin_statement { $$ = $1; }
| alter_statement { $$ = $1; }

explainable_statement : create_statement { $$ = $1; }
| drop_statement { $$ = $1; }
| copy_statement { $$ = $1; }
| show_statement { $$ = $1; }
| select_statement { $$ = $1; }
| delete_statement { $$ = $1; }
| update_statement { $$ = $1; }
| insert_statement { $$ = $1; }
| flush_statement { $$ = $1; }
| optimize_statement { $$ = $1; }
| command_statement { $$ = $1; }
| compact_statement { $$ = $1; }

/*
 * CREATE STATEMENT
 */

/* CREATE DATABASE schema_name; */
create_statement : CREATE DATABASE if_not_exists IDENTIFIER COMMENT STRING {
    $$ = new hybridsearch::CreateStatement();
    std::shared_ptr<hybridsearch::CreateSchemaInfo> create_schema_info = std::make_shared<hybridsearch::CreateSchemaInfo>();

    ParserHelper::ToLower($4);
    create_schema_info->schema_name_ = $4;
    free($4);
    if(create_schema_info->schema_name_.empty()) {
        yyerror(&yyloc, scanner, result, "Empty database name is given.");
        YYERROR;
    }

    $$->create_info_ = create_schema_info;
    $$->create_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    $$->create_info_->comment_ = $6;
    free($6);
}
| CREATE DATABASE if_not_exists IDENTIFIER {
    $$ = new hybridsearch::CreateStatement();
    std::shared_ptr<hybridsearch::CreateSchemaInfo> create_schema_info = std::make_shared<hybridsearch::CreateSchemaInfo>();

    ParserHelper::ToLower($4);
    create_schema_info->schema_name_ = $4;
    free($4);
    if(create_schema_info->schema_name_.empty()) {
        yyerror(&yyloc, scanner, result, "Empty database name is given.");
        YYERROR;
    }

    $$->create_info_ = create_schema_info;
    $$->create_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
}

/* CREATE COLLECTION collection_name; */
| CREATE COLLECTION if_not_exists table_name {
    $$ = new hybridsearch::CreateStatement();
    std::shared_ptr<hybridsearch::CreateCollectionInfo> create_collection_info = std::make_shared<hybridsearch::CreateCollectionInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        create_collection_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    create_collection_info->collection_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    $$->create_info_ = create_collection_info;
    $$->create_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    delete $4;
}

/* CREATE TABLE table_name ( column list ); */
/* CREATE TABLE table_name ( column list ) PROPERTIES ("p1"="111", "p2"="xxx", ...); */
| CREATE TABLE if_not_exists table_name '(' table_element_array ')' optional_table_properties_list {
    $$ = new hybridsearch::CreateStatement();
    std::shared_ptr<hybridsearch::CreateTableInfo> create_table_info = std::make_shared<hybridsearch::CreateTableInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        create_table_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    create_table_info->table_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    delete $4;

    for (hybridsearch::TableElement*& element : *$6) {
        if(element->type_ == hybridsearch::TableElementType::kColumn) {
            create_table_info->column_defs_.emplace_back((hybridsearch::ColumnDef*)element);
        } else {
            create_table_info->constraints_.emplace_back((hybridsearch::TableConstraint*)element);
        }
    }
    delete $6;

    if ($8 != nullptr) {
        create_table_info->properties_ = std::move(*$8);
        delete $8;
    }

    $$->create_info_ = create_table_info;
    $$->create_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
}
/* CREATE TABLE table_name AS SELECT .... ; */
| CREATE TABLE if_not_exists table_name AS select_statement {
    $$ = new hybridsearch::CreateStatement();
    std::shared_ptr<hybridsearch::CreateTableInfo> create_table_info = std::make_shared<hybridsearch::CreateTableInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        create_table_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    create_table_info->table_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    delete $4;

    create_table_info->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    create_table_info->select_ = $6;
    $$->create_info_ = create_table_info;
}
| CREATE TABLE if_not_exists table_name '(' table_element_array ')' optional_table_properties_list COMMENT STRING {
    $$ = new hybridsearch::CreateStatement();
    std::shared_ptr<hybridsearch::CreateTableInfo> create_table_info = std::make_shared<hybridsearch::CreateTableInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        create_table_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    create_table_info->table_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    delete $4;

    for (hybridsearch::TableElement*& element : *$6) {
        if(element->type_ == hybridsearch::TableElementType::kColumn) {
            create_table_info->column_defs_.emplace_back((hybridsearch::ColumnDef*)element);
        } else {
            create_table_info->constraints_.emplace_back((hybridsearch::TableConstraint*)element);
        }
    }
    delete $6;

    if ($8 != nullptr) {
        create_table_info->properties_ = std::move(*$8);
        delete $8;
    }

    create_table_info->comment_ = $10;
    free($10);

    $$->create_info_ = create_table_info;
    $$->create_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
}
/* CREATE TABLE table_name AS SELECT .... ; */
| CREATE TABLE if_not_exists table_name AS select_statement COMMENT STRING {
    $$ = new hybridsearch::CreateStatement();
    std::shared_ptr<hybridsearch::CreateTableInfo> create_table_info = std::make_shared<hybridsearch::CreateTableInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        create_table_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    create_table_info->table_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    delete $4;

    create_table_info->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    create_table_info->select_ = $6;
    create_table_info->comment_ = $8;
    free($8);
    $$->create_info_ = create_table_info;
}
/* CREATE VIEW table_name AS SELECT .... ; */
| CREATE VIEW if_not_exists table_name optional_identifier_array AS select_statement {
    $$ = new hybridsearch::CreateStatement();
    std::shared_ptr<hybridsearch::CreateViewInfo> create_view_info = std::make_shared<hybridsearch::CreateViewInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        create_view_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    create_view_info->view_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    delete $4;

    create_view_info->view_columns_ = $5;
    create_view_info->select_ = $7;
    create_view_info->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    $$->create_info_ = create_view_info;
}
// TODO shenyushi 4: should support default index name if the name does not exist
/* CREATE INDEX [[IF NOT EXISTS] index_name] ON table_name (column1[, ...column2]) USING method [WITH (param[, ...param])]; */
| CREATE INDEX if_not_exists_info ON table_name index_info {
    std::shared_ptr<hybridsearch::CreateIndexInfo> create_index_info = std::make_shared<hybridsearch::CreateIndexInfo>();
    if($5->schema_name_ptr_ != nullptr) {
        create_index_info->schema_name_ = $5->schema_name_ptr_;
        free($5->schema_name_ptr_);
    }
    create_index_info->table_name_ = $5->table_name_ptr_;
    free($5->table_name_ptr_);
    delete $5;

    create_index_info->index_name_ = $3->info_;
    if ($3->exists_) {
        create_index_info->conflict_type_ = $3->if_not_exists_ ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    } else {
        create_index_info->conflict_type_ = hybridsearch::ConflictType::kIgnore;
    }
    delete $3;

    create_index_info->index_info_ = $6;

    if(create_index_info->index_name_.empty()) {
        yyerror(&yyloc, scanner, result, "No index name");
        YYERROR;
    }

    $$ = new hybridsearch::CreateStatement();
    $$->create_info_ = create_index_info;
}
| CREATE INDEX if_not_exists_info ON table_name index_info COMMENT STRING {
    std::shared_ptr<hybridsearch::CreateIndexInfo> create_index_info = std::make_shared<hybridsearch::CreateIndexInfo>();
    if($5->schema_name_ptr_ != nullptr) {
        create_index_info->schema_name_ = $5->schema_name_ptr_;
        free($5->schema_name_ptr_);
    }
    create_index_info->table_name_ = $5->table_name_ptr_;
    free($5->table_name_ptr_);
    delete $5;

    create_index_info->index_name_ = $3->info_;
    if ($3->exists_) {
        create_index_info->conflict_type_ = $3->if_not_exists_ ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    } else {
        create_index_info->conflict_type_ = hybridsearch::ConflictType::kIgnore;
    }
    delete $3;

    create_index_info->index_info_ = $6;
    create_index_info->comment_ = $8;
    free($8);

    if(create_index_info->index_name_.empty()) {
        yyerror(&yyloc, scanner, result, "No index name");
        YYERROR;
    }

    $$ = new hybridsearch::CreateStatement();
    $$->create_info_ = create_index_info;
};

table_element_array : table_element {
    $$ = new std::vector<hybridsearch::TableElement*>();
    $$->push_back($1);
}
| table_element_array ',' table_element {
    $1->push_back($3);
    $$ = $1;
};

column_def_array : table_column {
    $$ = new std::vector<hybridsearch::ColumnDef*>();
    $$->push_back($1);
}
| column_def_array ',' table_column {
    $1->push_back($3);
    $$ = $1;
};


table_element : table_column {
    $$ = $1;
}
| table_constraint {
    $$ = $1;
};


table_column :
//   1          2              3                  4
IDENTIFIER column_type with_index_param_list default_expr {
    std::shared_ptr<hybridsearch::ParsedExpr> default_expr($4);
    std::vector<std::unique_ptr<hybridsearch::InitParameter>> index_param_list = hybridsearch::InitParameter::MakeInitParameterList($3);
    auto [data_type_result, fail_reason] = hybridsearch::ColumnType::GetDataTypeFromColumnType(*($2), index_param_list);
    delete $2;
    if (!data_type_result) {
        yyerror(&yyloc, scanner, result, fail_reason.c_str());
        free($1);
        YYERROR;
    }
    $$ = new hybridsearch::ColumnDef(std::move(data_type_result), "", std::move(default_expr));
    ParserHelper::ToLower($1);
    $$->name_ = $1;
    free($1);
    /*
    if (!$$->trySetNullableExplicit()) {
        yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{$1}).c_str());
    }
    */
}
| IDENTIFIER column_type column_constraints default_expr {
    std::shared_ptr<hybridsearch::ParsedExpr> default_expr($4);
    auto [data_type_result, fail_reason] = hybridsearch::ColumnType::GetDataTypeFromColumnType(*($2), std::vector<std::unique_ptr<hybridsearch::InitParameter>>{});
    delete $2;
    if (!data_type_result) {
        yyerror(&yyloc, scanner, result, fail_reason.c_str());
        free($1);
        delete $3;
        YYERROR;
    }
    $$ = new hybridsearch::ColumnDef(std::move(data_type_result), "", std::move(default_expr));
    ParserHelper::ToLower($1);
    $$->name_ = $1;
    free($1);
    $$->constraints_ = std::move(*$3);
    delete $3;
    /*
    if (!$$->trySetNullableExplicit()) {
        yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{$1}).c_str());
    }
    */
}
| IDENTIFIER column_type with_index_param_list default_expr COMMENT STRING {
    std::shared_ptr<hybridsearch::ParsedExpr> default_expr($4);
    std::vector<std::unique_ptr<hybridsearch::InitParameter>> index_param_list = hybridsearch::InitParameter::MakeInitParameterList($3);
    auto [data_type_result, fail_reason] = hybridsearch::ColumnType::GetDataTypeFromColumnType(*($2), index_param_list);
    delete $2;
    if (!data_type_result) {
        yyerror(&yyloc, scanner, result, fail_reason.c_str());
        free($1);
        free($6);
        YYERROR;
    }

    $$ = new hybridsearch::ColumnDef(std::move(data_type_result), $6, std::move(default_expr));
    free($6);

    ParserHelper::ToLower($1);
    $$->name_ = $1;
    free($1);
    /*
    if (!$$->trySetNullableExplicit()) {
        yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{$1}).c_str());
    }
    */
}
| IDENTIFIER column_type column_constraints default_expr COMMENT STRING {
    std::shared_ptr<hybridsearch::ParsedExpr> default_expr($4);
    auto [data_type_result, fail_reason] = hybridsearch::ColumnType::GetDataTypeFromColumnType(*($2), std::vector<std::unique_ptr<hybridsearch::InitParameter>>{});
    delete $2;
    if (!data_type_result) {
        yyerror(&yyloc, scanner, result, fail_reason.c_str());
        free($1);
        free($6);
        delete $3;
        YYERROR;
    }
    $$ = new hybridsearch::ColumnDef(std::move(data_type_result), $6, std::move(default_expr));
    free($6);

    ParserHelper::ToLower($1);
    $$->name_ = $1;
    $$->constraints_ = std::move(*$3);
    delete $3;
    free($1);
    /*
    if (!$$->trySetNullableExplicit()) {
        yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{$1}).c_str());
    }
    */
};

column_type_array : column_type {
    $$ = new std::vector<std::unique_ptr<hybridsearch::ColumnType>>();
    $$->emplace_back($1);
}
| column_type_array ',' column_type {
    $$ = $1;
    $$->emplace_back($3);
};

column_type :
BOOLEAN { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kBoolean, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| TINYINT { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTinyInt, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| SMALLINT { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSmallInt, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| INTEGER { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kInteger, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| INT { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kInteger, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| BIGINT { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kBigInt, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| HUGEINT { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kHugeInt, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| FLOAT { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kFloat, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| REAL  { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kFloat, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| DOUBLE { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kDouble, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| FLOAT16 { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kFloat16, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| BFLOAT16 { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kBFloat16, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| DATE { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kDate, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| TIME { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTime, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| DATETIME { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kDateTime, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| TIMESTAMP { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTimestamp, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| UUID { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kUuid, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| POINT { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kPoint, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| LINE { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kLine, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| LSEG { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kLineSeg, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| BOX { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kBox, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
// | PATH { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kPath, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
// | POLYGON { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kPolygon, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| CIRCLE { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kCircle, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
// Variable types
| VARCHAR { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kVarchar, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| DECIMAL '(' LONG_VALUE ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kDecimal, 0, $3, $5, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| DECIMAL '(' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kDecimal, 0, $3, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
| DECIMAL { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kDecimal, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid}; }
// | BLOB '(' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kBlob, $3}; }
// | BITMAP '(' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kBitmap, $3}; }
| EMBEDDING '(' BIT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBit}; }
| EMBEDDING '(' TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt8}; }
| EMBEDDING '(' SMALLINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt16}; }
| EMBEDDING '(' INTEGER ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| EMBEDDING '(' INT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| EMBEDDING '(' BIGINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt64}; }
| EMBEDDING '(' FLOAT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat}; }
| EMBEDDING '(' DOUBLE ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemDouble}; }
| EMBEDDING '(' FLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat16}; }
| EMBEDDING '(' BFLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBFloat16}; }
| EMBEDDING '(' UNSIGNED TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $6, 0, 0, hybridsearch::EmbeddingDataType::kElemUInt8}; }
| MULTIVECTOR '(' BIT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBit}; }
| MULTIVECTOR '(' TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt8}; }
| MULTIVECTOR '(' SMALLINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt16}; }
| MULTIVECTOR '(' INTEGER ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| MULTIVECTOR '(' INT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| MULTIVECTOR '(' BIGINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt64}; }
| MULTIVECTOR '(' FLOAT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat}; }
| MULTIVECTOR '(' DOUBLE ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemDouble}; }
| MULTIVECTOR '(' FLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat16}; }
| MULTIVECTOR '(' BFLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBFloat16}; }
| MULTIVECTOR '(' UNSIGNED TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kMultiVector, $6, 0, 0, hybridsearch::EmbeddingDataType::kElemUInt8}; }
| TENSOR '(' BIT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBit}; }
| TENSOR '(' TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt8}; }
| TENSOR '(' SMALLINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt16}; }
| TENSOR '(' INTEGER ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| TENSOR '(' INT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| TENSOR '(' BIGINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt64}; }
| TENSOR '(' FLOAT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat}; }
| TENSOR '(' DOUBLE ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemDouble}; }
| TENSOR '(' FLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat16}; }
| TENSOR '(' BFLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBFloat16}; }
| TENSOR '(' UNSIGNED TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensor, $6, 0, 0, hybridsearch::EmbeddingDataType::kElemUInt8}; }
| TENSORARRAY '(' BIT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBit}; }
| TENSORARRAY '(' TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt8}; }
| TENSORARRAY '(' SMALLINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt16}; }
| TENSORARRAY '(' INTEGER ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| TENSORARRAY '(' INT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| TENSORARRAY '(' BIGINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt64}; }
| TENSORARRAY '(' FLOAT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat}; }
| TENSORARRAY '(' DOUBLE ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemDouble}; }
| TENSORARRAY '(' FLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat16}; }
| TENSORARRAY '(' BFLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBFloat16}; }
| TENSORARRAY '(' UNSIGNED TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTensorArray, $6, 0, 0, hybridsearch::EmbeddingDataType::kElemUInt8}; }
| VECTOR '(' BIT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBit}; }
| VECTOR '(' TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt8}; }
| VECTOR '(' SMALLINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt16}; }
| VECTOR '(' INTEGER ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| VECTOR '(' INT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| VECTOR '(' BIGINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt64}; }
| VECTOR '(' FLOAT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat}; }
| VECTOR '(' DOUBLE ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemDouble}; }
| VECTOR '(' FLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat16}; }
| VECTOR '(' BFLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBFloat16}; }
| VECTOR '(' UNSIGNED TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kEmbedding, $6, 0, 0, hybridsearch::EmbeddingDataType::kElemUInt8}; }
| SPARSE '(' BIT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBit}; }
| SPARSE '(' TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt8}; }
| SPARSE '(' SMALLINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt16}; }
| SPARSE '(' INTEGER ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| SPARSE '(' INT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt32}; }
| SPARSE '(' BIGINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemInt64}; }
| SPARSE '(' FLOAT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat}; }
| SPARSE '(' DOUBLE ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemDouble}; }
| SPARSE '(' FLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemFloat16}; }
| SPARSE '(' BFLOAT16 ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $5, 0, 0, hybridsearch::EmbeddingDataType::kElemBFloat16}; }
| SPARSE '(' UNSIGNED TINYINT ',' LONG_VALUE ')' { $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kSparse, $6, 0, 0, hybridsearch::EmbeddingDataType::kElemUInt8}; }
| ARRAY '(' column_type ')' {
  $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kArray, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid};
  $$->element_types_.emplace_back($3);
}
| TUPLE '(' column_type_array ')' {
  $$ = new hybridsearch::ColumnType{hybridsearch::LogicalType::kTuple, 0, 0, 0, hybridsearch::EmbeddingDataType::kElemInvalid};
  $$->element_types_ = std::move(*($3));
  delete $3;
}
/*
| DECIMAL opt_decimal_specification {
  $$ = hybridsearch::ColumnType{DataType::DECIMAL, 0, $2->first, $2->second};
  delete $2;
}
| TEXT { $$ = hybridsearch::ColumnType{DataType::TEXT}; }
| TIME opt_time_precision { $$ = hybridsearch::ColumnType{DataType::TIME, 0, $2}; }
| TIMESTAMP { $$ = hybridsearch::ColumnType{DataType::DATETIME}; }
| VARCHAR '(' INTVAL ')' { $$ = hybridsearch::ColumnType{DataType::VARCHAR, $3}; }

opt_time_precision : '(' INTVAL ')' { $$ = $2; }
| { $$ = 0; };

opt_decimal_specification : '(' INTVAL ',' INTVAL ')' { $$ = new std::pair<int64_t, int64_t>{$2, $4}; }
| '(' INTVAL ')' { $$ = new std::pair<int64_t, int64_t>{$2, 0}; }
|  { $$ = new std::pair<int64_t, int64_t>{0, 0}; };
*/

column_constraints : column_constraint {
    $$ = new std::set<hybridsearch::ConstraintType>();
    $$->insert($1);
}
| column_constraints column_constraint {
    if($1->contains($2)) {
        yyerror(&yyloc, scanner, result, "Duplicate column constraint.");
        delete $1;
        YYERROR;
    }
    $1->insert($2);
    $$ = $1;
}

column_constraint : PRIMARY KEY {
    $$ = hybridsearch::ConstraintType::kPrimaryKey;
}
| UNIQUE {
    $$ = hybridsearch::ConstraintType::kUnique;
}
| NULLABLE {
    $$ = hybridsearch::ConstraintType::kNull;
}
| NOT NULLABLE {
    $$ = hybridsearch::ConstraintType::kNotNull;
};

default_expr : DEFAULT constant_expr {
    $$ = $2;
}
| /* empty default value */ {
    $$ = nullptr;
};


table_constraint : PRIMARY KEY '(' identifier_array ')' {
    $$ = new hybridsearch::TableConstraint();
    $$->names_ptr_ = $4;
    $$->constraint_ = hybridsearch::ConstraintType::kPrimaryKey;
}
| UNIQUE '(' identifier_array ')' {
    $$ = new hybridsearch::TableConstraint();
    $$->names_ptr_ = $3;
    $$->constraint_ = hybridsearch::ConstraintType::kUnique;
};


identifier_array : IDENTIFIER {
    $$ = new std::vector<std::string>();
    ParserHelper::ToLower($1);
    $$->emplace_back($1);
    free($1);
}
| identifier_array ',' IDENTIFIER {
    ParserHelper::ToLower($3);
    $1->emplace_back($3);
    free($3);
    $$ = $1;
};

/*
 * DELETE STATEMENT
 */
delete_statement : DELETE FROM table_name where_clause {
    $$ = new hybridsearch::DeleteStatement();

    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;
    $$->where_expr_ = $4;
};

/*
 * INSERT STATEMENT
 */
insert_statement: INSERT INTO table_name optional_identifier_array VALUES insert_row_list {
    bool is_error{false};
    for (auto expr_array : *$6) {
        for (const auto &expr : expr_array->values_) {
            if(expr->type_ != hybridsearch::ParsedExprType::kConstant) {
                yyerror(&yyloc, scanner, result, ("Value list has non-constant expression: " + expr->ToString()).c_str());
                is_error = true;
            }
        }
    }
    if(is_error) {
        for (auto expr_array : *$6) {
            delete (expr_array);
        }
        delete $6;
        delete $3;
        delete $4;
        YYERROR;
    }

    $$ = new hybridsearch::InsertStatement();
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;
    for (hybridsearch::InsertRowExpr* &expr_ptr : *$6) {
        if ($4) {
            expr_ptr->columns_ = *($4);
        }
        $$->insert_rows_.emplace_back(expr_ptr);
        expr_ptr = nullptr;
    }
    delete $4;
    delete $6;
}
| INSERT INTO table_name optional_identifier_array select_without_paren {
    $$ = new hybridsearch::InsertStatement();
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;
    if ($4) {
        $$->columns_for_select_ = std::move(*($4));
        delete $4;
    }
    $$->select_.reset($5);
}

optional_identifier_array: '(' identifier_array ')' {
    $$ = $2;
}
| {
    $$ = nullptr;
}

/*
 * EXPLAIN STATEMENT
 */
explain_statement : EXPLAIN IDENTIFIER explainable_statement {
    $$ = new hybridsearch::ExplainStatement();
    ParserHelper::ToLower($2);
    if(!strcmp($2, "analyze")) $$->type_ = hybridsearch::ExplainType::kAnalyze;
    else if(!strcmp($2, "ast")) $$->type_ =hybridsearch::ExplainType::kAst;
    else if(!strcmp($2, "raw")) $$->type_ =hybridsearch::ExplainType::kUnOpt;
    else if(!strcmp($2, "logical")) $$->type_ =hybridsearch::ExplainType::kOpt;
    else if(!strcmp($2, "physical")) $$->type_ =hybridsearch::ExplainType::kPhysical;
    else if(!strcmp($2, "pipeline")) $$->type_ =hybridsearch::ExplainType::kPipeline;
    else if(!strcmp($2, "fragment")) $$->type_ =hybridsearch::ExplainType::kFragment;
    free($2);
    $$->statement_ = $3;
} | EXPLAIN explainable_statement {
    $$ = new hybridsearch::ExplainStatement();
    $$->type_ =hybridsearch::ExplainType::kPhysical;
    $$->statement_ = $2;
}

/*
 * UPDATE STATEMENT
 */
update_statement: UPDATE table_name SET update_expr_array where_clause {
    $$ = new hybridsearch::UpdateStatement();
    if($2->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $2->schema_name_ptr_;
        free($2->schema_name_ptr_);
    }
    $$->table_name_ = $2->table_name_ptr_;
    free($2->table_name_ptr_);
    delete $2;
    $$->where_expr_ = $5;
    $$->update_expr_array_ = $4;
}

update_expr_array: update_expr {
    $$ = new std::vector<hybridsearch::UpdateExpr*>();
    $$->emplace_back($1);
}
| update_expr_array ',' update_expr {
    $1->emplace_back($3);
    $$ = $1;
}

update_expr : IDENTIFIER '=' expr {
    $$ = new hybridsearch::UpdateExpr();
    ParserHelper::ToLower($1);
    $$->column_name = $1;
    free($1);
    $$->value = $3;
};

/*
 * DROP STATEMENT
 */

/* DROP DATABASE schema_name; */
drop_statement: DROP DATABASE if_exists IDENTIFIER {
    $$ = new hybridsearch::DropStatement();
    std::shared_ptr<hybridsearch::DropSchemaInfo> drop_schema_info = std::make_shared<hybridsearch::DropSchemaInfo>();

    ParserHelper::ToLower($4);
    drop_schema_info->schema_name_ = $4;
    free($4);

    $$->drop_info_ = drop_schema_info;
    $$->drop_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
};

/* DROP COLLECTION collection_name; */
| DROP COLLECTION if_exists table_name {
    $$ = new hybridsearch::DropStatement();
    std::shared_ptr<hybridsearch::DropCollectionInfo> drop_collection_info = std::make_unique<hybridsearch::DropCollectionInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        drop_collection_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    drop_collection_info->collection_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    $$->drop_info_ = drop_collection_info;
    $$->drop_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    delete $4;
}

/* DROP TABLE table_name; */
| DROP TABLE if_exists table_name {
    $$ = new hybridsearch::DropStatement();
    std::shared_ptr<hybridsearch::DropTableInfo> drop_table_info = std::make_unique<hybridsearch::DropTableInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        drop_table_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    drop_table_info->table_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    $$->drop_info_ = drop_table_info;
    $$->drop_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    delete $4;
}

/* DROP VIEW view_name; */
| DROP VIEW if_exists table_name {
    $$ = new hybridsearch::DropStatement();
    std::shared_ptr<hybridsearch::DropViewInfo> drop_view_info = std::make_unique<hybridsearch::DropViewInfo>();
    if($4->schema_name_ptr_ != nullptr) {
        drop_view_info->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    drop_view_info->view_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    $$->drop_info_ = drop_view_info;
    $$->drop_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;
    delete $4;
}

/* DROP INDEX index_name ON table_name; */
| DROP INDEX if_exists IDENTIFIER ON table_name {
    $$ = new hybridsearch::DropStatement();
    std::shared_ptr<hybridsearch::DropIndexInfo> drop_index_info = std::make_shared<hybridsearch::DropIndexInfo>();

    $$->drop_info_ = drop_index_info;
    $$->drop_info_->conflict_type_ = $3 ? hybridsearch::ConflictType::kIgnore : hybridsearch::ConflictType::kError;

    drop_index_info->index_name_ = $4;
    free($4);

    if($6->schema_name_ptr_ != nullptr) {
        drop_index_info->schema_name_ = $6->schema_name_ptr_;
        free($6->schema_name_ptr_);
    }
    drop_index_info->table_name_ = $6->table_name_ptr_;
    free($6->table_name_ptr_);
    delete $6;
};

/*
 * COPY STATEMENT
 */
// COPY database.table TO file_path WITH (FORMAT csv, DELIMITER ',', HEADER)
copy_statement: COPY table_name TO file_path WITH '(' copy_option_list ')' {
    $$ = new hybridsearch::CopyStatement();

    // Copy To
    $$->copy_from_ = false;

    // table_name
    if($2->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $2->schema_name_ptr_;
        free($2->schema_name_ptr_);
    }
    $$->table_name_ = $2->table_name_ptr_;
    free($2->table_name_ptr_);
    delete $2;

    // file path
    $$->file_path_ = $4;
    free($4);

    // copy options
    size_t option_count = (*$7).size();
    for(size_t idx = 0; idx < option_count; ++ idx) {
        hybridsearch::CopyOption* option_ptr = (*$7)[idx];
        switch(option_ptr->option_type_) {
            case hybridsearch::CopyOptionType::kFormat: {
                $$->copy_file_type_ = option_ptr->file_type_;
                break;
            }
            case hybridsearch::CopyOptionType::kDelimiter: {
                $$->delimiter_ = option_ptr->delimiter_;
                break;
            }
            case hybridsearch::CopyOptionType::kHeader: {
                $$->header_ = option_ptr->header_;
                break;
            }
            case hybridsearch::CopyOptionType::kOffset: {
                $$->offset_ = option_ptr->offset_;
                break;
            }
            case hybridsearch::CopyOptionType::kLimit: {
                $$->limit_ = option_ptr->limit_;
                break;
            }
            case hybridsearch::CopyOptionType::kRowLimit: {
                $$->row_limit_ = option_ptr->row_limit_;
                break;
            }
        }
        delete option_ptr;
    }
    delete $7;
}
| COPY table_name '(' expr_array ')' TO file_path WITH '(' copy_option_list ')' {
    $$ = new hybridsearch::CopyStatement();

    // Copy To
    $$->copy_from_ = false;

    // table_name
    if($2->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $2->schema_name_ptr_;
        free($2->schema_name_ptr_);
    }
    $$->table_name_ = $2->table_name_ptr_;
    free($2->table_name_ptr_);
    delete $2;

    $$->expr_array_ = $4;

    // file path
    $$->file_path_ = $7;
    free($7);

    // copy options
    size_t option_count = (*$10).size();
    for(size_t idx = 0; idx < option_count; ++ idx) {
        hybridsearch::CopyOption* option_ptr = (*$10)[idx];
        switch(option_ptr->option_type_) {
            case hybridsearch::CopyOptionType::kFormat: {
                $$->copy_file_type_ = option_ptr->file_type_;
                break;
            }
            case hybridsearch::CopyOptionType::kDelimiter: {
                $$->delimiter_ = option_ptr->delimiter_;
                break;
            }
            case hybridsearch::CopyOptionType::kHeader: {
                $$->header_ = option_ptr->header_;
                break;
            }
            case hybridsearch::CopyOptionType::kOffset: {
                $$->offset_ = option_ptr->offset_;
                break;
            }
            case hybridsearch::CopyOptionType::kLimit: {
                $$->limit_ = option_ptr->limit_;
                break;
            }
            case hybridsearch::CopyOptionType::kRowLimit: {
                $$->row_limit_ = option_ptr->row_limit_;
                break;
            }
        }
        delete option_ptr;
    }
    delete $10;
}
| COPY table_name FROM file_path WITH '(' copy_option_list ')' {
    $$ = new hybridsearch::CopyStatement();

    // Copy From
    $$->copy_from_ = true;

    // table_name
    if($2->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $2->schema_name_ptr_;
        free($2->schema_name_ptr_);
    }
    $$->table_name_ = $2->table_name_ptr_;
    free($2->table_name_ptr_);
    delete $2;

    // file path
    $$->file_path_ = $4;
    free($4);

    // copy options
    size_t option_count = (*$7).size();
    for(size_t idx = 0; idx < option_count; ++ idx) {
        hybridsearch::CopyOption* option_ptr = (*$7)[idx];
        switch(option_ptr->option_type_) {
            case hybridsearch::CopyOptionType::kFormat: {
                $$->copy_file_type_ = option_ptr->file_type_;
                break;
            }
            case hybridsearch::CopyOptionType::kDelimiter: {
                $$->delimiter_ = option_ptr->delimiter_;
                break;
            }
            case hybridsearch::CopyOptionType::kHeader: {
                $$->header_ = option_ptr->header_;
                break;
            }
            default: {
                delete option_ptr;
                delete $7;
                yyerror(&yyloc, scanner, result, "Invalid import option");
                YYERROR;
            }
        }
        delete option_ptr;
    }
    delete $7;
};

/*
 * SELECT STATEMENT
 */
select_statement : select_without_paren {
    $$ = $1;
}
| select_with_paren {
    $$ = $1;
}
| select_statement set_operator select_clause_without_modifier_paren {
    hybridsearch::SelectStatement* node = $1;
    while(node->nested_select_ != nullptr) {
        node = node->nested_select_;
    }
    node->set_op_ = $2;
    node->nested_select_ = $3;
    $$ = $1;
}
| select_statement set_operator select_clause_without_modifier {
    hybridsearch::SelectStatement* node = $1;
    while(node->nested_select_ != nullptr) {
        node = node->nested_select_;
    }
    node->set_op_ = $2;
    node->nested_select_ = $3;
    $$ = $1;
}

select_with_paren : '(' select_without_paren ')' {
    $$ = $2;
}
| '(' select_with_paren ')' {
    $$ = $2;
};

select_without_paren: with_clause select_clause_with_modifier {
    $2->with_exprs_ = $1;
    $$ = $2;
};

select_clause_with_modifier: select_clause_without_modifier order_by_clause limit_expr offset_expr {
    if($3 == nullptr and $4 != nullptr) {
        delete $1;
        delete $2;
        delete $4;
        yyerror(&yyloc, scanner, result, "Offset expression isn't valid without Limit expression");
        YYERROR;
    }
    if($1->search_expr_ != nullptr and ($2 != nullptr /*or $3 != nullptr or $4 != nullptr*/)) {
        delete $1;
        if ($2) {
            for (auto ptr : *($2)) {
                delete ptr;
            }
            delete $2;
        }
        delete $3;
        delete $4;
        yyerror(&yyloc, scanner, result, "Result modifier(ORDER BY) is conflict with SEARCH expression.");
        YYERROR;
    }
    $1->order_by_list_ = $2;
    $1->limit_expr_ = $3;
    $1->offset_expr_ = $4;
    $$ = $1;
}

select_clause_without_modifier_paren: '(' select_clause_without_modifier ')' {
  $$ = $2;
}
| '(' select_clause_without_modifier_paren ')' {
    $$ = $2;
};

select_clause_without_modifier:
SELECT distinct expr_array highlight_clause from_clause search_clause where_clause group_by_clause having_clause {
    $$ = new hybridsearch::SelectStatement();
    $$->select_distinct_ = $2;
    $$->select_list_ = $3;
    $$->highlight_list_ = $4;
    $$->table_ref_ = $5;
    $$->search_expr_ = $6;
    $$->where_expr_ = $7;
    $$->group_by_list_ = $8;
    $$->having_expr_ = $9;

    if($$->group_by_list_ == nullptr && $$->having_expr_ != nullptr) {
        yyerror(&yyloc, scanner, result, "HAVING clause should follow after GROUP BY clause");
        YYERROR;
    }
};

order_by_clause : ORDER BY order_by_expr_list {
    $$ = $3;
}
| /* empty order by */ {
    $$ = nullptr;
};

order_by_expr_list: order_by_expr {
    $$ = new std::vector<hybridsearch::OrderByExpr*>();
    $$->emplace_back($1);
}
| order_by_expr_list ',' order_by_expr {
    $1->emplace_back($3);
    $$ = $1;
}

order_by_expr : expr order_by_type {
    $$ = new hybridsearch::OrderByExpr();
    $$->expr_ = $1;
    $$->type_ = $2;
};

order_by_type: ASC {
    $$ = hybridsearch::kAsc;
}
| DESC {
    $$ = hybridsearch::kDesc;
}
| {
    $$ = hybridsearch::kAsc;
}

limit_expr: LIMIT expr {
    $$ = $2;
}
| /* empty limit expression */
{   $$ = nullptr; };

offset_expr: OFFSET expr {
    $$ = $2;
}
| /* empty offset expression */
{   $$ = nullptr; };

distinct : DISTINCT {
    $$ = true;
}
| {
    $$ = false;
}

highlight_clause: HIGHLIGHT expr_array {
    $$ = $2;
}
| {
    $$ = nullptr;
}

from_clause: FROM table_reference {
    $$ = $2;
}
| /* no from clause */ {
    $$ = nullptr;
}

search_clause: SEARCH sub_search_array {
    hybridsearch::SearchExpr* search_expr = new hybridsearch::SearchExpr();
    search_expr->SetExprs($2);
    $$ = search_expr;
}
| /* no search clause */ {
    $$ = nullptr;
}

optional_search_filter_expr: ',' WHERE expr {
    $$ = $3;
}
| /* no where clause */ {
    $$ = nullptr;
}

where_clause: WHERE expr {
    $$ = $2;
}
| /* no where clause */ {
    $$ = nullptr;
}

having_clause: HAVING expr {
    $$ = $2;
}
| /* no where clause */ {
    $$ = nullptr;
}

group_by_clause: GROUP BY expr_array {
    $$ = $3;
}
| {
    $$ = nullptr;
}

set_operator : UNION {
    $$ = hybridsearch::SetOperatorType::kUnion;
}
| UNION ALL {
    $$ = hybridsearch::SetOperatorType::kUnionAll;
}
| INTERSECT {
    $$ = hybridsearch::SetOperatorType::kIntersect;
}
| EXCEPT {
    $$ = hybridsearch::SetOperatorType::kExcept;
};

/*
 * TABLE REFERENCE
 */

table_reference : table_reference_unit {
    $$ = $1;
}
| table_reference ',' table_reference_unit {
    hybridsearch::CrossProductReference* cross_product_ref = nullptr;
    if($1->type_ == hybridsearch::TableRefType::kCrossProduct) {
        cross_product_ref = (hybridsearch::CrossProductReference*)$1;
        cross_product_ref->tables_.emplace_back($3);
    } else {
        cross_product_ref = new hybridsearch::CrossProductReference();
        cross_product_ref->tables_.emplace_back($1);
        cross_product_ref->tables_.emplace_back($3);
    }

    $$ = cross_product_ref;
};


table_reference_unit : table_reference_name | join_clause;

table_reference_name : table_name table_alias {
    hybridsearch::TableReference* table_ref = new hybridsearch::TableReference();
    if($1->schema_name_ptr_ != nullptr) {
        table_ref->db_name_ = $1->schema_name_ptr_;
        free($1->schema_name_ptr_);
    }
    table_ref->table_name_ = $1->table_name_ptr_;
    free($1->table_name_ptr_);
    delete $1;

    table_ref->alias_ = $2;
    $$ = table_ref;
}
/* FROM (select * from t1) AS t2 */
| '(' select_statement ')' table_alias {
    hybridsearch::SubqueryReference* subquery_reference = new hybridsearch::SubqueryReference();
    subquery_reference->select_statement_ = $2;
    subquery_reference->alias_ = $4;
    $$ = subquery_reference;
}


/* 'table_name' or 'schema_name.table_name' */
table_name : IDENTIFIER {
    $$ = new hybridsearch::TableName();
    ParserHelper::ToLower($1);
    $$->table_name_ptr_ = $1;
}
| IDENTIFIER '.' IDENTIFIER {
    $$ = new hybridsearch::TableName();
    ParserHelper::ToLower($1);
    ParserHelper::ToLower($3);
    $$->schema_name_ptr_ = $1;
    $$->table_name_ptr_ = $3;
};

/* AS 'table_alias' or AS 'table_alias(col1_alias, col2_alias ... )' */
table_alias : AS IDENTIFIER {
    $$ = new hybridsearch::TableAlias();
    ParserHelper::ToLower($2);
    $$->alias_ = $2;
}
| IDENTIFIER {
    $$ = new hybridsearch::TableAlias();
    ParserHelper::ToLower($1);
    $$->alias_ = $1;
}
| AS IDENTIFIER '(' identifier_array ')' {
    $$ = new hybridsearch::TableAlias();
    ParserHelper::ToLower($2);
    $$->alias_ = $2;
    $$->column_alias_array_ = $4;
}
| {
    $$ = nullptr;
}

/*
 * WITH CLAUSE
 */
with_clause : WITH with_expr_list {
    $$ = $2;
}
| /* empty with clause */ {
    $$ = nullptr;
}

with_expr_list: with_expr {
    $$ = new std::vector<hybridsearch::WithExpr*>();
    $$->emplace_back($1);
} | with_expr_list ',' with_expr {
    $1->emplace_back($3);
    $$ = $1;
}

with_expr: IDENTIFIER AS '(' select_clause_with_modifier ')' {
    $$ = new hybridsearch::WithExpr();
    ParserHelper::ToLower($1);
    $$->alias_ = $1;
    free($1);
    $$->select_ = $4;
}

/*
 * JOIN CLAUSE
 */

join_clause: table_reference_unit NATURAL JOIN table_reference_name {
    hybridsearch::JoinReference* join_reference = new hybridsearch::JoinReference();
    join_reference->left_ = $1;
    join_reference->right_ = $4;
    join_reference->join_type_ = hybridsearch::JoinType::kNatural;
    $$ = join_reference;
}
| table_reference_unit join_type JOIN table_reference_name ON expr {
    hybridsearch::JoinReference* join_reference = new hybridsearch::JoinReference();
    join_reference->left_ = $1;
    join_reference->right_ = $4;
    join_reference->join_type_ = $2;
    join_reference->condition_ = $6;
    $$ = join_reference;
};
/* Using column name to JOIN
| table_reference_unit join_type JOIN table_reference_name USING '(' column_name ')' {
}
*/


join_type : INNER {
    $$ = hybridsearch::JoinType::kInner;
}
| LEFT {
    $$ = hybridsearch::JoinType::kLeft;
}
| RIGHT {
    $$ = hybridsearch::JoinType::kRight;
}
| OUTER {
    $$ = hybridsearch::JoinType::kFull;
}
| FULL {
    $$ = hybridsearch::JoinType::kFull;
}
| CROSS {
    $$ = hybridsearch::JoinType::kCross;
}
| /* default */ {
};

/*
 * SHOW STATEMENT
 */
show_statement: SHOW DATABASES {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kDatabases;
}
| SHOW TABLES {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kTables;
}
| SHOW VIEWS {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kViews;
}
| SHOW CONFIGS {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kConfigs;
}
| SHOW CONFIG IDENTIFIER {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kConfig;
    ParserHelper::ToLower($3);
    $$->var_name_ = std::string($3);
    free($3);
}
| SHOW PROFILES {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kProfiles;
}
| SHOW BUFFER {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kBuffer;
}
| SHOW MEMINDEX {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kMemIndex;
}
| SHOW QUERIES {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kQueries;
}
| SHOW QUERY LONG_VALUE {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kQuery;
    $$->session_id_ = $3;
}
| SHOW TRANSACTIONS {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kTransactions;
}
| SHOW TRANSACTION LONG_VALUE {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kTransaction;
    $$->txn_id_ = $3;
}
| SHOW TRANSACTION HISTORY {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kTransactionHistory;
}
| SHOW SESSION VARIABLES {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kSessionVariables;
}
| SHOW GLOBAL VARIABLES {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kGlobalVariables;
}
| SHOW SESSION VARIABLE IDENTIFIER {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kSessionVariable;
    $$->var_name_ = std::string($4);
    free($4);
}
| SHOW GLOBAL VARIABLE IDENTIFIER {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kGlobalVariable;
    $$->var_name_ = std::string($4);
    free($4);
}
| SHOW DATABASE IDENTIFIER {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kDatabase;
    $$->schema_name_ = $3;
    free($3);
}
| SHOW TABLE table_name {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kTable;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;
}
| SHOW TABLE table_name COLUMNS {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kColumns;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;
}
| SHOW TABLE table_name SEGMENTS {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kSegments;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;
}
| SHOW TABLE table_name SEGMENT LONG_VALUE {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kSegment;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    $$->segment_id_ = $5;
    delete $3;
}
| SHOW TABLE table_name SEGMENT LONG_VALUE BLOCKS {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kBlocks;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    $$->segment_id_ = $5;
    delete $3;
}
| SHOW TABLE table_name SEGMENT LONG_VALUE BLOCK LONG_VALUE {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kBlock;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    $$->segment_id_ = $5;
    $$->block_id_ = $7;
    delete $3;
}
| SHOW TABLE table_name SEGMENT LONG_VALUE BLOCK LONG_VALUE COLUMN LONG_VALUE {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kBlockColumn;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    $$->segment_id_ = $5;
    $$->block_id_ = $7;
    $$->column_id_ = $9;
    delete $3;
}
| SHOW TABLE table_name INDEXES {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kIndexes;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;
}
| SHOW TABLE table_name INDEX IDENTIFIER {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kIndex;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;

    $$->index_name_ = $5;
    free($5);
}
| SHOW TABLE table_name INDEX IDENTIFIER SEGMENT LONG_VALUE {
    $$ = new hybridsearch::ShowStatement();
    $$->show_type_ = hybridsearch::ShowStmtType::kIndexSegment;
    if($3->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $3->schema_name_ptr_;
        free($3->schema_name_ptr_);
    }
    $$->table_name_ = $3->table_name_ptr_;
    free($3->table_name_ptr_);
    delete $3;

    $$->index_name_ = $5;
    free($5);

    $$->segment_id_ = $7;
}
| SHOW TABLE table_name INDEX IDENTIFIER SEGMENT LONG_VALUE CHUNK LONG_VALUE {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kIndexChunk;
      if($3->schema_name_ptr_ != nullptr) {
          $$->schema_name_ = $3->schema_name_ptr_;
          free($3->schema_name_ptr_);
      }
      $$->table_name_ = $3->table_name_ptr_;
      free($3->table_name_ptr_);
      delete $3;

      $$->index_name_ = $5;
      free($5);

      $$->segment_id_ = $7;
      $$->chunk_id_ = $9;
}
| SHOW LOGS {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kLogs;
}
| SHOW DELTA LOGS {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kDeltaLogs;
}
| SHOW CATALOGS {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kCatalogs;
}
| SHOW PERSISTENCE FILES {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kPersistenceFiles;
}
| SHOW PERSISTENCE OBJECTS {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kPersistenceObjects;
}
| SHOW PERSISTENCE OBJECT STRING {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kPersistenceObject;
      $$->file_name_ = $4;
      free($4);
}
| SHOW MEMORY {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kMemory;
}
| SHOW MEMORY OBJECTS {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kMemoryObjects;
}
| SHOW MEMORY ALLOCATION {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kMemoryAllocation;
}
| SHOW IDENTIFIER '(' ')' {
      $$ = new hybridsearch::ShowStatement();
      $$->show_type_ = hybridsearch::ShowStmtType::kFunction;
      $$->function_name_ = $2;
      free($2);
}
| SHOW SNAPSHOTS {
     $$ = new hybridsearch::ShowStatement();
     $$->show_type_ = hybridsearch::ShowStmtType::kListSnapshots;
}
| SHOW SNAPSHOT IDENTIFIER {
     $$ = new hybridsearch::ShowStatement();
     $$->show_type_ = hybridsearch::ShowStmtType::kShowSnapshot;
     $$->snapshot_name_ = $3;
     free($3);
}

/*
 * FLUSH STATEMENT
 */
flush_statement: FLUSH DATA {
    $$ = new hybridsearch::FlushStatement();
    $$->type_ = hybridsearch::FlushType::kData;
}
| FLUSH LOG {
    $$ = new hybridsearch::FlushStatement();
    $$->type_ = hybridsearch::FlushType::kLog;
}
| FLUSH BUFFER {
    $$ = new hybridsearch::FlushStatement();
    $$->type_ = hybridsearch::FlushType::kBuffer;
};

/*
 * OPTIMIZE STATEMENT
 */
optimize_statement: OPTIMIZE table_name {
    $$ = new hybridsearch::OptimizeStatement();
    if($2->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $2->schema_name_ptr_;
        free($2->schema_name_ptr_);
    }
    $$->table_name_ = $2->table_name_ptr_;
    free($2->table_name_ptr_);
    delete $2;
}
//   1         2      3        4            5
| OPTIMIZE IDENTIFIER ON table_name with_index_param_list{
    $$ = new hybridsearch::OptimizeStatement();
    if($4->schema_name_ptr_ != nullptr) {
        $$->schema_name_ = $4->schema_name_ptr_;
        free($4->schema_name_ptr_);
    }
    $$->table_name_ = $4->table_name_ptr_;
    free($4->table_name_ptr_);
    delete $4;

    $$->index_name_ = $2;
    free($2);

    for (auto *&index_param : *$5) {
        $$->opt_params_.emplace_back(std::unique_ptr<hybridsearch::InitParameter>(index_param));
        index_param = nullptr;
    }
    delete $5;
};

/*
 * Command
 */
command_statement: USE IDENTIFIER {
    $$ = new hybridsearch::CommandStatement();
    ParserHelper::ToLower($2);
    $$->command_info_ = std::make_shared<hybridsearch::UseCmd>($2);
    free($2);
}
| EXPORT PROFILES LONG_VALUE file_path {
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::ExportCmd>($4, hybridsearch::ExportType::kProfileRecord, $3);
    free($4);
}
| SET SESSION IDENTIFIER ON {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kSession, hybridsearch::SetVarType::kBool, $3, true);
    free($3);
}
| SET SESSION IDENTIFIER OFF {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kSession, hybridsearch::SetVarType::kBool, $3, false);
    free($3);
}
| SET SESSION IDENTIFIER IDENTIFIER {
    ParserHelper::ToLower($3);
    ParserHelper::ToLower($4);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kSession, hybridsearch::SetVarType::kString, $3, $4);
    free($3);
    free($4);
}
| SET SESSION IDENTIFIER LONG_VALUE {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kSession, hybridsearch::SetVarType::kInteger, $3, $4);
    free($3);
}
| SET SESSION IDENTIFIER DOUBLE_VALUE {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kSession, hybridsearch::SetVarType::kDouble, $3, $4);
    free($3);
};
| SET GLOBAL IDENTIFIER ON {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kGlobal, hybridsearch::SetVarType::kBool, $3, true);
    free($3);
}
| SET GLOBAL IDENTIFIER OFF {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kGlobal, hybridsearch::SetVarType::kBool, $3, false);
    free($3);
}
| SET GLOBAL IDENTIFIER IDENTIFIER {
    ParserHelper::ToLower($3);
    ParserHelper::ToLower($4);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kGlobal, hybridsearch::SetVarType::kString, $3, $4);
    free($3);
    free($4);
}
| SET GLOBAL IDENTIFIER LONG_VALUE {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kGlobal, hybridsearch::SetVarType::kInteger, $3, $4);
    free($3);
}
| SET GLOBAL IDENTIFIER DOUBLE_VALUE {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kGlobal, hybridsearch::SetVarType::kDouble, $3, $4);
    free($3);
}
| SET CONFIG IDENTIFIER ON {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kConfig, hybridsearch::SetVarType::kBool, $3, true);
    free($3);
}
| SET CONFIG IDENTIFIER OFF {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kConfig, hybridsearch::SetVarType::kBool, $3, false);
    free($3);
}
| SET CONFIG IDENTIFIER IDENTIFIER {
    ParserHelper::ToLower($3);
    ParserHelper::ToLower($4);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kConfig, hybridsearch::SetVarType::kString, $3, $4);
    free($3);
    free($4);
}
| SET CONFIG IDENTIFIER LONG_VALUE {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kConfig, hybridsearch::SetVarType::kInteger, $3, $4);
    free($3);
}
| SET CONFIG IDENTIFIER DOUBLE_VALUE {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SetCmd>(hybridsearch::SetScope::kConfig, hybridsearch::SetVarType::kDouble, $3, $4);
    free($3);
}
| LOCK TABLE table_name {
    $$ = new hybridsearch::CommandStatement();
    ParserHelper::ToLower($3->schema_name_ptr_);
    ParserHelper::ToLower($3->table_name_ptr_);
    $$->command_info_ = std::make_shared<hybridsearch::LockCmd>($3->schema_name_ptr_, $3->table_name_ptr_);
    free($3->schema_name_ptr_);
    free($3->table_name_ptr_);
    delete $3;
}
| UNLOCK TABLE table_name {
    $$ = new hybridsearch::CommandStatement();
    ParserHelper::ToLower($3->schema_name_ptr_);
    ParserHelper::ToLower($3->table_name_ptr_);
    $$->command_info_ = std::make_shared<hybridsearch::UnlockCmd>($3->schema_name_ptr_, $3->table_name_ptr_);
    free($3->schema_name_ptr_);
    free($3->table_name_ptr_);
    delete $3;
}
| CREATE SNAPSHOT IDENTIFIER ON TABLE IDENTIFIER {
    ParserHelper::ToLower($3);
    ParserHelper::ToLower($6);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SnapshotCmd>($3, hybridsearch::SnapshotOp::kCreate, hybridsearch::SnapshotScope::kTable, $6);
    free($3);
    free($6);
}
| CREATE SNAPSHOT IDENTIFIER ON DATABASE IDENTIFIER {
    ParserHelper::ToLower($3);
    ParserHelper::ToLower($6);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SnapshotCmd>($3, hybridsearch::SnapshotOp::kCreate, hybridsearch::SnapshotScope::kDatabase, $6);
    free($3);
    free($6);
}
| CREATE SNAPSHOT IDENTIFIER ON SYSTEM {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SnapshotCmd>($3, hybridsearch::SnapshotOp::kCreate, hybridsearch::SnapshotScope::kSystem);
    free($3);
}
| DROP SNAPSHOT IDENTIFIER {
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SnapshotCmd>($3, hybridsearch::SnapshotOp::kDrop, hybridsearch::SnapshotScope::kIgnore);
    free($3);
}
| RESTORE DATABASE SNAPSHOT IDENTIFIER {
    ParserHelper::ToLower($4);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SnapshotCmd>($4, hybridsearch::SnapshotOp::kRestore, hybridsearch::SnapshotScope::kDatabase);
    free($4);
}
| RESTORE TABLE SNAPSHOT IDENTIFIER {
    ParserHelper::ToLower($4);
    $$ = new hybridsearch::CommandStatement();
    $$->command_info_ = std::make_shared<hybridsearch::SnapshotCmd>($4, hybridsearch::SnapshotOp::kRestore, hybridsearch::SnapshotScope::kTable);
    free($4);
}

compact_statement: COMPACT TABLE table_name {
    std::string schema_name;
    if ($3->schema_name_ptr_ != nullptr) {
        schema_name = std::string($3->schema_name_ptr_);
        free($3->schema_name_ptr_);
    }
    std::string table_name = std::string($3->table_name_ptr_);
    free($3->table_name_ptr_);

    $$ = new hybridsearch::ManualCompactStatement(std::move(schema_name), std::move(table_name));
    delete $3;
}

admin_statement: ADMIN SHOW CATALOGS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListCatalogs;
}
| ADMIN SHOW CATALOG LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowCatalog;
     $$->catalog_file_index_ = $4;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASES {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListDatabases;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowDatabase;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLES {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListTables;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowTable;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE COLUMNS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowColumn;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE SEGMENTS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListSegments;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE SEGMENT LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowSegment;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
     $$->segment_index_ = $13;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE SEGMENT LONG_VALUE BLOCKS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListBlocks;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
     $$->segment_index_ = $13;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE SEGMENT LONG_VALUE BLOCK LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowBlock;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
     $$->segment_index_ = $13;
     $$->block_index_ = $15;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE SEGMENT LONG_VALUE BLOCK LONG_VALUE COLUMNS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListColumns;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
     $$->segment_index_ = $13;
     $$->block_index_ = $15;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE INDEXES {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListIndexes;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE INDEX LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowIndex;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
     $$->index_meta_index_ = $13;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE INDEX LONG_VALUE LONG_VALUE SEGMENTS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListIndexSegments;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
     $$->index_meta_index_ = $13;
     $$->index_entry_index_ = $14;
}
| ADMIN SHOW CATALOG LONG_VALUE LONG_VALUE DATABASE LONG_VALUE LONG_VALUE TABLE LONG_VALUE LONG_VALUE INDEX LONG_VALUE LONG_VALUE SEGMENT LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowIndexSegment;
     $$->catalog_file_start_index_ = $4;
     $$->catalog_file_end_index_ = $5;
     $$->database_meta_index_ = $7;
     $$->database_entry_index_ = $8;
     $$->table_meta_index_ = $10;
     $$->table_entry_index_ = $11;
     $$->index_meta_index_ = $13;
     $$->index_entry_index_ = $14;
     $$->segment_index_ = $16;
}
| ADMIN SHOW LOGS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListLogFiles;
}
| ADMIN SHOW LOG LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowLogFile;
     $$->log_file_index_ = $4;
}
| ADMIN SHOW LOG LONG_VALUE INDEXES {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListLogIndexes;
     $$->log_file_index_ = $4;
}
| ADMIN SHOW LOG LONG_VALUE INDEX LONG_VALUE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowLogIndex;
     $$->log_file_index_ = $4;
     $$->log_index_in_file_ = $6;
}
| ADMIN SHOW CONFIGS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListConfigs;
}
| ADMIN SHOW VARIABLES {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListVariables;
}
| ADMIN SHOW VARIABLE IDENTIFIER {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowVariable;
     $$->variable_name_ = $4;
     free($4);
}
| ADMIN CREATE SNAPSHOT {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kCreateSnapshot;
}
| ADMIN SHOW SNAPSHOTS {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListSnapshots;
}
| ADMIN SHOW SNAPSHOT IDENTIFIER {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowSnapshot;
     $$->snapshot_name_ = $4;
     free($4);
}
| ADMIN DELETE SNAPSHOT STRING {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kDeleteSnapshot;
     $$->snapshot_name_ = $4;
     free($4);
}
| ADMIN EXPORT SNAPSHOT STRING TO STRING {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kExportSnapshot;
     $$->snapshot_name_ = $4;
     $$->export_path_ = $6;
     free($4);
     free($6);
}
| ADMIN RECOVER FROM SNAPSHOT STRING {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kRecoverFromSnapshot;
     $$->snapshot_name_ = $5;
     free($5);
}
| ADMIN SHOW NODES {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kListNodes;
}
| ADMIN SHOW NODE STRING {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowNode;
     $$->node_name_ = $4;
     free($4);
}
| ADMIN SHOW NODE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kShowCurrentNode;
}
| ADMIN REMOVE NODE STRING {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kRemoveNode;
     $$->node_name_ = $4;
     free($4);
}
| ADMIN SET ADMIN {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kSetRole;
     $$->node_role_ = hybridsearch::NodeRole::kAdmin;
}
| ADMIN SET STANDALONE {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kSetRole;
     $$->node_role_ = hybridsearch::NodeRole::kStandalone;
}
| ADMIN SET LEADER USING STRING {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kSetRole;
     $$->node_role_ = hybridsearch::NodeRole::kLeader;
     $$->node_name_ = $5;
     free($5);
}
| ADMIN CONNECT STRING AS FOLLOWER USING STRING {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kSetRole;
     $$->node_role_ = hybridsearch::NodeRole::kFollower;
     $$->leader_address_ = $3;
     $$->node_name_ = $7;
     free($3);
     free($7);
}
| ADMIN CONNECT STRING AS LEARNER USING STRING {
     $$ = new hybridsearch::AdminStatement();
     $$->admin_type_ = hybridsearch::AdminStmtType::kSetRole;
     $$->node_role_ = hybridsearch::NodeRole::kLearner;
     $$->leader_address_ = $3;
     $$->node_name_ = $7;
     free($3);
     free($7);
}

alter_statement : ALTER TABLE table_name RENAME TO IDENTIFIER {
    auto *ret = new hybridsearch::RenameTableStatement($3->schema_name_ptr_, $3->table_name_ptr_);
    $$ = ret;
    ret->new_table_name_ = $6;
    free($6);
    free($3->schema_name_ptr_);
    free($3->table_name_ptr_);
    delete $3;
}
| ALTER TABLE table_name ADD COLUMN '(' column_def_array ')' {
    auto *ret = new hybridsearch::AddColumnsStatement($3->schema_name_ptr_, $3->table_name_ptr_);
    $$ = ret;

    for (hybridsearch::ColumnDef*& column_def : *$7) {
        ret->column_defs_.emplace_back(column_def);
    }
    delete $7;
    free($3->schema_name_ptr_);
    free($3->table_name_ptr_);
    delete $3;
}
| ALTER TABLE table_name DROP COLUMN '(' identifier_array ')' {
    auto *ret = new hybridsearch::DropColumnsStatement($3->schema_name_ptr_, $3->table_name_ptr_);
    $$ = ret;
    for (std::string &column_name : *$7) {
        ret->column_names_.emplace_back(std::move(column_name));
    }
    delete $7;
    free($3->schema_name_ptr_);
    free($3->table_name_ptr_);
    delete $3;
}

/*
 * EXPRESSION
 */

expr_array : expr_alias {
    $$ = new std::vector<hybridsearch::ParsedExpr*>();
    $$->emplace_back($1);
}
| expr_array ',' expr_alias {
    $1->emplace_back($3);
    $$ = $1;
};

insert_row_list : '(' expr_array ')' {
    auto res = std::make_unique<hybridsearch::InsertRowExpr>();
    for (auto* &expr : *$2) {
        res->values_.emplace_back(expr);
        expr = nullptr;
    }
    delete $2;
    $$ = new std::vector<hybridsearch::InsertRowExpr*>();
    $$->emplace_back(res.release());
}
| insert_row_list ',' '(' expr_array ')' {
    $$ = $1;
    auto res = std::make_unique<hybridsearch::InsertRowExpr>();
    for (auto* &expr : *$4) {
        res->values_.emplace_back(expr);
        expr = nullptr;
    }
    delete $4;
    $$->emplace_back(res.release());
};

/*
constant_expr_array: constant_expr {
    $$ = new std::vector<hybridsearch::ParsedExpr*>();
    $$->emplace_back($1);
}
| constant_expr_array ',' constant_expr {
    $1->emplace_back($3);
    $$ = $1;
}
*/

expr_alias : expr AS IDENTIFIER {
    $$ = $1;
    ParserHelper::ToLower($3);
    $$->alias_ = $3;
    free($3);
}
| expr {
    $$ = $1;
};

expr : operand
| subquery_expr
| in_expr
| between_expr
| conjunction_expr;

operand: '(' expr ')' {
    $$ = $2;
}
| '(' select_without_paren ')' {
    hybridsearch::SubqueryExpr* subquery_expr = new hybridsearch::SubqueryExpr();
    subquery_expr->subquery_type_ = hybridsearch::SubqueryType::kScalar;
    subquery_expr->select_ = $2;
    $$ = subquery_expr;
}
| constant_expr {
    $$ = $1;
}
| column_expr
| function_expr
| case_expr
| cast_expr
| match_vector_expr
| match_text_expr
| match_tensor_expr
| match_sparse_expr
| query_expr
| fusion_expr

//                                       4                 6                 8          10         12                                          13
//                  MATCH TENSOR  (  column_name, search_tensor, tensor_data_type, search_method, extra_match_tensor_option(including topn) optional_filter)
match_tensor_expr : MATCH TENSOR '(' column_expr ',' common_array_expr ',' STRING ',' STRING ',' STRING optional_search_filter_expr ')' {
    auto match_tensor_expr = std::make_unique<hybridsearch::MatchTensorExpr>();
    // search column
    match_tensor_expr->SetSearchColumn($4);
    // search tensor
    ParserHelper::ToLower($8);
    match_tensor_expr->SetQueryTensor($8, $6);
    // search method
    ParserHelper::ToLower($10);
    match_tensor_expr->SetSearchMethod($10);
    // search options
    match_tensor_expr->SetExtraOptions($12);
    match_tensor_expr->SetOptionalFilter($13);
    $$ = match_tensor_expr.release();
}
| //1   2     3       4       5        6            7     8    9     10   11   12          13                   14    15   16   17     18
MATCH TENSOR '(' column_expr ',' common_array_expr ',' STRING ',' STRING ',' STRING optional_search_filter_expr ')' USING INDEX '(' IDENTIFIER ')' {
    auto match_tensor_expr = std::make_unique<hybridsearch::MatchTensorExpr>();
    // search column
    match_tensor_expr->SetSearchColumn($4);
    // search tensor
    ParserHelper::ToLower($8);
    match_tensor_expr->SetQueryTensor($8, $6);
    // search method
    ParserHelper::ToLower($10);
    match_tensor_expr->SetSearchMethod($10);
    // search options
    match_tensor_expr->SetExtraOptions($12);
    match_tensor_expr->SetOptionalFilter($13);
    match_tensor_expr->index_name_ = $18;
    $$ = match_tensor_expr.release();
}
| //1   2     3       4       5        6            7     8    9     10   11   12          13                   14
MATCH TENSOR '(' column_expr ',' common_array_expr ',' STRING ',' STRING ',' STRING optional_search_filter_expr ')' IGNORE INDEX {
    auto match_tensor_expr = std::make_unique<hybridsearch::MatchTensorExpr>();
    // search column
    match_tensor_expr->SetSearchColumn($4);
    // search tensor
    ParserHelper::ToLower($8);
    match_tensor_expr->SetQueryTensor($8, $6);
    // search method
    ParserHelper::ToLower($10);
    match_tensor_expr->SetSearchMethod($10);
    // search options
    match_tensor_expr->ignore_index_ = true;
    match_tensor_expr->SetExtraOptions($12);
    match_tensor_expr->SetOptionalFilter($13);
    $$ = match_tensor_expr.release();
}
//                  MATCH VECTOR (column_name, query_vec, data_type, metric_type, topn             optional_filter          ) USING INDEX ( index_name )  extra options
//                   1      2         4         6              8          10           12                13                                        18             20
match_vector_expr : MATCH VECTOR '(' expr ',' array_expr ',' STRING ',' STRING ',' LONG_VALUE optional_search_filter_expr ')' USING INDEX '(' IDENTIFIER ')' with_index_param_list {
    hybridsearch::KnnExpr* match_vector_expr = new hybridsearch::KnnExpr();
    $$ = match_vector_expr;

    // vector search column
    match_vector_expr->column_expr_ = $4;

    // vector distance type
    ParserHelper::ToLower($10);
    bool check = match_vector_expr->InitDistanceType($10);
    if (!check) {
        goto Error1;
    }

    // vector data type
    ParserHelper::ToLower($8);
    check = match_vector_expr->InitEmbedding($8, $6);
    if (!check) {
        goto Error1;
    }
    free($8);
    free($10);
    delete $6;

    match_vector_expr->index_name_ = $18;
    free($18);
    match_vector_expr->topn_ = $12;
    match_vector_expr->filter_expr_.reset($13);
    match_vector_expr->opt_params_ = $20;
    goto Return1;
Error1:
    for (auto* param_ptr: *$20) {
        delete param_ptr;
    }
    delete $20;
    free($8);
    free($10);
    free($18);
    delete $6;
    delete $$;
    yyerror(&yyloc, scanner, result, "Invalid vector search distance type");
    YYERROR;
Return1:
    ;
}
|
MATCH VECTOR '(' expr ',' array_expr ',' STRING ',' STRING ',' LONG_VALUE optional_search_filter_expr ')' IGNORE INDEX {
    hybridsearch::KnnExpr* match_vector_expr = new hybridsearch::KnnExpr();
    $$ = match_vector_expr;

    // vector search column
    match_vector_expr->column_expr_ = $4;

    // vector distance type
    ParserHelper::ToLower($10);
    bool check = match_vector_expr->InitDistanceType($10);
    if (!check) {
        goto Error2;
    }

    // vector data type
    ParserHelper::ToLower($8);
    check = match_vector_expr->InitEmbedding($8, $6);
    if (!check) {
        goto Error2;
    }
    free($8);
    free($10);
    delete $6;

    match_vector_expr->topn_ = $12;
    match_vector_expr->filter_expr_.reset($13);
    match_vector_expr->ignore_index_ = true;
    goto Return2;
Error2:
    free($8);
    free($10);
    delete $6;
    delete $$;
    yyerror(&yyloc, scanner, result, "Invalid vector search distance type");
    YYERROR;
Return2:
    ;
}
|
MATCH VECTOR '(' expr ',' array_expr ',' STRING ',' STRING ',' LONG_VALUE optional_search_filter_expr ')' with_index_param_list {
    hybridsearch::KnnExpr* match_vector_expr = new hybridsearch::KnnExpr();
    $$ = match_vector_expr;

    // vector search column
    match_vector_expr->column_expr_ = $4;

    // vector distance type
    ParserHelper::ToLower($10);
    bool check = match_vector_expr->InitDistanceType($10);
    if (!check) {
        goto Error3;
    }

    // vector data type
    ParserHelper::ToLower($8);
    check = match_vector_expr->InitEmbedding($8, $6);
    if (!check) {
        goto Error3;
    }
    free($8);
    free($10);
    delete $6;

    match_vector_expr->topn_ = $12;
    match_vector_expr->filter_expr_.reset($13);
    match_vector_expr->opt_params_ = $15;
    goto Return3;
Error3:
    for (auto* param_ptr: *$15) {
        delete param_ptr;
    }
    delete $15;
    free($8);
    free($10);
    delete $6;
    delete $$;
    yyerror(&yyloc, scanner, result, "Invalid vector search distance type");
    YYERROR;
Return3:
    ;
}
|
MATCH VECTOR '(' expr ',' array_expr ',' STRING ',' STRING optional_search_filter_expr ')' with_index_param_list {
    hybridsearch::KnnExpr* match_vector_expr = new hybridsearch::KnnExpr();
    $$ = match_vector_expr;

    // vector search search column
    match_vector_expr->column_expr_ = $4;

    // vector search distance type
    ParserHelper::ToLower($10);
    bool check = match_vector_expr->InitDistanceType($10);
    if (!check) {
        goto Error4;
    }

    // vector search data type
    ParserHelper::ToLower($8);
    check = match_vector_expr->InitEmbedding($8, $6);
    if (!check) {
        goto Error4;
    }
    free($8);
    free($10);
    delete $6;

    match_vector_expr->topn_ = hybridsearch::DEFAULT_MATCH_VECTOR_TOP_N;
    match_vector_expr->filter_expr_.reset($11);
    match_vector_expr->opt_params_ = $13;
    goto Return4;

Error4:
    for (auto* param_ptr: *$13) {
        delete param_ptr;
    }
    delete $13;
    free($8);
    free($10);
    delete $6;
    delete $$;
    yyerror(&yyloc, scanner, result, "Invalid vector search distance type");
    YYERROR;
Return4:
    ;
}


//                 MATCH SPARSE (column_name,       query_sparse,      metric_type,     topn,         optional_filter)                                             extra options
//                   1      2         4                  6                   8           10                  11                                     16                  18
match_sparse_expr: MATCH SPARSE '(' expr ',' common_sparse_array_expr ',' STRING ',' LONG_VALUE optional_search_filter_expr ')' USING INDEX '(' IDENTIFIER ')' with_index_param_list {
    auto match_sparse_expr = new hybridsearch::MatchSparseExpr();
    $$ = match_sparse_expr;

    // search column
    match_sparse_expr->SetSearchColumn($4);

    // search sparse and data type
    match_sparse_expr->SetQuerySparse($6);

    // metric type
    ParserHelper::ToLower($8);
    match_sparse_expr->SetMetricType($8);

    // topn and options
    match_sparse_expr->SetOptParams($10, $18);

    // optional filter
    match_sparse_expr->SetOptionalFilter($11);

    match_sparse_expr->index_name_ = $16;
    free($16);
}
|
MATCH SPARSE '(' expr ',' common_sparse_array_expr ',' STRING ',' LONG_VALUE optional_search_filter_expr ')' IGNORE INDEX {
    auto match_sparse_expr = new hybridsearch::MatchSparseExpr();
    $$ = match_sparse_expr;

    // search column
    match_sparse_expr->SetSearchColumn($4);

    // search sparse and data type
    match_sparse_expr->SetQuerySparse($6);

    // metric type
    ParserHelper::ToLower($8);
    match_sparse_expr->SetMetricType($8);

    // topn and options
    match_sparse_expr->topn_ = $10;

    // optional filter
    match_sparse_expr->SetOptionalFilter($11);

    match_sparse_expr->ignore_index_ = true;
}
|
MATCH SPARSE '(' expr ',' common_sparse_array_expr ',' STRING ',' LONG_VALUE optional_search_filter_expr ')' with_index_param_list {
    auto match_sparse_expr = new hybridsearch::MatchSparseExpr();
    $$ = match_sparse_expr;

    // search column
    match_sparse_expr->SetSearchColumn($4);

    // search sparse and data type
    match_sparse_expr->SetQuerySparse($6);

    // metric type
    ParserHelper::ToLower($8);
    match_sparse_expr->SetMetricType($8);

    // optional filter
    match_sparse_expr->SetOptionalFilter($11);

    // topn and options
    match_sparse_expr->SetOptParams($10, $13);
}
|
MATCH SPARSE '(' expr ',' common_sparse_array_expr ',' STRING optional_search_filter_expr ')' with_index_param_list {
    auto match_sparse_expr = new hybridsearch::MatchSparseExpr();
    $$ = match_sparse_expr;

    // search column
    match_sparse_expr->SetSearchColumn($4);

    // search sparse and data type
    match_sparse_expr->SetQuerySparse($6);

    // metric type
    ParserHelper::ToLower($8);
    match_sparse_expr->SetMetricType($8);

    // optional filter
    match_sparse_expr->SetOptionalFilter($9);

    // topn and options
    match_sparse_expr->SetOptParams(hybridsearch::DEFAULT_MATCH_SPARSE_TOP_N, $11);
}

match_text_expr : MATCH TEXT '(' STRING ',' STRING optional_search_filter_expr ')' {
    hybridsearch::MatchExpr* match_text_expr = new hybridsearch::MatchExpr();
    match_text_expr->fields_ = std::string($4);
    match_text_expr->matching_text_ = std::string($6);
    match_text_expr->filter_expr_.reset($7);
    free($4);
    free($6);
    $$ = match_text_expr;
}
| MATCH TEXT '(' STRING ',' STRING ',' STRING optional_search_filter_expr ')' {
    hybridsearch::MatchExpr* match_text_expr = new hybridsearch::MatchExpr();
    match_text_expr->fields_ = std::string($4);
    match_text_expr->matching_text_ = std::string($6);
    match_text_expr->options_text_ = std::string($8);
    match_text_expr->filter_expr_.reset($9);
    free($4);
    free($6);
    free($8);
    $$ = match_text_expr;
}
| MATCH TEXT '(' STRING ',' STRING optional_search_filter_expr ')' USING INDEXES '(' STRING ')' {
    hybridsearch::MatchExpr* match_text_expr = new hybridsearch::MatchExpr();
    match_text_expr->fields_ = std::string($4);
    match_text_expr->matching_text_ = std::string($6);
    match_text_expr->filter_expr_.reset($7);
    match_text_expr->index_names_ = std::string($12);
    free($4);
    free($6);
    free($12);
    $$ = match_text_expr;
}
| MATCH TEXT '(' STRING ',' STRING ',' STRING optional_search_filter_expr ')' USING INDEXES '(' STRING ')' {
    hybridsearch::MatchExpr* match_text_expr = new hybridsearch::MatchExpr();
    match_text_expr->fields_ = std::string($4);
    match_text_expr->matching_text_ = std::string($6);
    match_text_expr->options_text_ = std::string($8);
    match_text_expr->filter_expr_.reset($9);
    match_text_expr->index_names_ = std::string($14);
    free($4);
    free($6);
    free($8);
    free($14);
    $$ = match_text_expr;
}

query_expr : QUERY '(' STRING optional_search_filter_expr ')' {
    hybridsearch::MatchExpr* match_text_expr = new hybridsearch::MatchExpr();
    match_text_expr->matching_text_ = std::string($3);
    match_text_expr->filter_expr_.reset($4);
    free($3);
    $$ = match_text_expr;
}
| QUERY '(' STRING ',' STRING optional_search_filter_expr ')' {
    hybridsearch::MatchExpr* match_text_expr = new hybridsearch::MatchExpr();
    match_text_expr->matching_text_ = std::string($3);
    match_text_expr->options_text_ = std::string($5);
    match_text_expr->filter_expr_.reset($6);
    free($3);
    free($5);
    $$ = match_text_expr;
}

fusion_expr : FUSION '(' STRING ')' {
    hybridsearch::FusionExpr* fusion_expr = new hybridsearch::FusionExpr();
    fusion_expr->method_ = std::string($3);
    free($3);
    $$ = fusion_expr;
}
| FUSION '(' STRING ',' STRING ')' {
    auto fusion_expr = std::make_unique<hybridsearch::FusionExpr>();
    fusion_expr->method_ = std::string($3);
    free($3);
    $3 = nullptr;
    fusion_expr->SetOptions($5);
    free($5);
    $5 = nullptr;
    fusion_expr->JobAfterParser();
    $$ = fusion_expr.release();
}

sub_search : match_vector_expr {
    $$ = $1;
}
| match_text_expr {
    $$ = $1;
}
| match_tensor_expr {
    $$ = $1;
}
| match_sparse_expr {
    $$ = $1;
}
| query_expr {
    $$ = $1;
}
| fusion_expr {
    $$ = $1;
}

sub_search_array : sub_search {
    $$ = new std::vector<hybridsearch::ParsedExpr*>();
    $$->emplace_back($1);
}
| sub_search_array ',' sub_search {
    $1->emplace_back($3);
    $$ = $1;
};

function_expr : IDENTIFIER '(' ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    ParserHelper::ToLower($1);
    func_expr->func_name_ = $1;
    free($1);
    func_expr->arguments_ = nullptr;
    $$ = func_expr;
}
| IDENTIFIER '(' expr_array ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    ParserHelper::ToLower($1);
    func_expr->func_name_ = $1;
    free($1);
    func_expr->arguments_ = $3;
    $$ = func_expr;
}
| IDENTIFIER '(' DISTINCT expr_array ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    ParserHelper::ToLower($1);
    func_expr->func_name_ = $1;
    free($1);
    func_expr->arguments_ = $4;
    func_expr->distinct_ = true;
    $$ = func_expr;
}
| YEAR '(' expr ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "year";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| MONTH '(' expr ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "month";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| DAY '(' expr ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "day";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| HOUR '(' expr ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "hour";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| MINUTE '(' expr ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "minute";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| SECOND '(' expr ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "second";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand IS NOT NULLABLE {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "is_not_null";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    $$ = func_expr;
}
| operand IS NULLABLE {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "is_null";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    $$ = func_expr;
}
| NOT operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "not";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($2);
    $$ = func_expr;
}
| '-' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "-";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($2);
    $$ = func_expr;
}
| '+' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "+";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($2);
    $$ = func_expr;
}
| operand '-' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "-";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand '+' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "+";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand '*' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "*";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand '/' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "/";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand '%' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "%";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand '=' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "=";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand EQUAL operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "=";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand NOT_EQ operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "<>";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand '<' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "<";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand '>' operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = ">";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand LESS_EQ operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "<=";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand GREATER_EQ operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = ">=";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| EXTRACT '(' STRING FROM operand ')' {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    ParserHelper::ToLower($3);
    if(strcmp($3, "year") == 0) {
        func_expr->func_name_ = "extract_year";
        func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    } else if(strcmp($3, "month") == 0) {
        func_expr->func_name_ = "extract_month";
        func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    } else if(strcmp($3, "day") == 0) {
        func_expr->func_name_ = "extract_day";
        func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    } else if(strcmp($3, "hour") == 0) {
        func_expr->func_name_ = "extract_hour";
        func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    } else if(strcmp($3, "minute") == 0) {
        func_expr->func_name_ = "extract_minute";
        func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    } else if(strcmp($3, "second") == 0) {
        func_expr->func_name_ = "extract_second";
        func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    } else {
        delete func_expr;
        yyerror(&yyloc, scanner, result, "Invalid column expression format");
        YYERROR;
    }
    free($3);
    func_expr->arguments_->emplace_back($5);
    $$ = func_expr;
}
| operand LIKE operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "like";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| operand NOT LIKE operand {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "not_like";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($4);
    $$ = func_expr;
};

conjunction_expr: expr AND expr {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "and";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
}
| expr OR expr {
    hybridsearch::FunctionExpr* func_expr = new hybridsearch::FunctionExpr();
    func_expr->func_name_ = "or";
    func_expr->arguments_ = new std::vector<hybridsearch::ParsedExpr*>();
    func_expr->arguments_->emplace_back($1);
    func_expr->arguments_->emplace_back($3);
    $$ = func_expr;
};

between_expr: operand BETWEEN operand AND operand {
    hybridsearch::BetweenExpr* between_expr = new hybridsearch::BetweenExpr();
    between_expr->value_ = $1;
    between_expr->lower_bound_ = $3;
    between_expr->upper_bound_ = $5;
    $$ = between_expr;
}

in_expr: operand IN '(' expr_array ')' {
    hybridsearch::InExpr* in_expr = new hybridsearch::InExpr(true);
    in_expr->left_ = $1;
    in_expr->arguments_ = $4;
    $$ = in_expr;
}
| operand NOT IN '(' expr_array ')' {
    hybridsearch::InExpr* in_expr = new hybridsearch::InExpr(false);
    in_expr->left_ = $1;
    in_expr->arguments_ = $5;
    $$ = in_expr;
};

case_expr: CASE expr case_check_array END {
    hybridsearch::CaseExpr* case_expr = new hybridsearch::CaseExpr();
    case_expr->expr_ = $2;
    case_expr->case_check_array_ = $3;
    $$ = case_expr;
}
| CASE expr case_check_array ELSE expr END {
    hybridsearch::CaseExpr* case_expr = new hybridsearch::CaseExpr();
    case_expr->expr_ = $2;
    case_expr->case_check_array_ = $3;
    case_expr->else_expr_ = $5;
    $$ = case_expr;
}
| CASE case_check_array END {
    hybridsearch::CaseExpr* case_expr = new hybridsearch::CaseExpr();
    case_expr->case_check_array_ = $2;
    $$ = case_expr;
}
| CASE case_check_array ELSE expr END {
    hybridsearch::CaseExpr* case_expr = new hybridsearch::CaseExpr();
    case_expr->case_check_array_ = $2;
    case_expr->else_expr_ = $4;
    $$ = case_expr;
};

case_check_array: WHEN expr THEN expr {
    $$ = new std::vector<hybridsearch::WhenThen*>();
    hybridsearch::WhenThen* when_then_ptr = new hybridsearch::WhenThen();
    when_then_ptr->when_ = $2;
    when_then_ptr->then_ = $4;
    $$->emplace_back(when_then_ptr);
}
| case_check_array WHEN expr THEN expr {
    hybridsearch::WhenThen* when_then_ptr = new hybridsearch::WhenThen();
    when_then_ptr->when_ = $3;
    when_then_ptr->then_ = $5;
    $1->emplace_back(when_then_ptr);
    $$ = $1;
};

cast_expr: CAST '(' expr AS column_type ')' {
    auto [data_type_result, fail_reason] = hybridsearch::ColumnType::GetDataTypeFromColumnType(*($5), std::vector<std::unique_ptr<hybridsearch::InitParameter>>{});
    delete $5;
    if (!data_type_result) {
        yyerror(&yyloc, scanner, result, fail_reason.c_str());
        delete $3;
        YYERROR;
    }
    hybridsearch::CastExpr* cast_expr = new hybridsearch::CastExpr(std::move(*data_type_result));
    cast_expr->expr_ = $3;
    $$ = cast_expr;
};

subquery_expr: EXISTS '(' select_without_paren ')' {
    hybridsearch::SubqueryExpr* subquery_expr = new hybridsearch::SubqueryExpr();
    subquery_expr->subquery_type_ = hybridsearch::SubqueryType::kExists;
    subquery_expr->select_ = $3;
    $$ = subquery_expr;
}
| NOT EXISTS '(' select_without_paren ')' {
    hybridsearch::SubqueryExpr* subquery_expr = new hybridsearch::SubqueryExpr();
    subquery_expr->subquery_type_ = hybridsearch::SubqueryType::kNotExists;
    subquery_expr->select_ = $4;
    $$ = subquery_expr;
}
| operand IN '(' select_without_paren ')' {
    hybridsearch::SubqueryExpr* subquery_expr = new hybridsearch::SubqueryExpr();
    subquery_expr->subquery_type_ = hybridsearch::SubqueryType::kIn;
    subquery_expr->left_ = $1;
    subquery_expr->select_ = $4;
    $$ = subquery_expr;
}
| operand NOT IN '(' select_without_paren ')' {
    hybridsearch::SubqueryExpr* subquery_expr = new hybridsearch::SubqueryExpr();
    subquery_expr->subquery_type_ = hybridsearch::SubqueryType::kNotIn;
    subquery_expr->left_ = $1;
    subquery_expr->select_ = $5;
    $$ = subquery_expr;
};

column_expr : IDENTIFIER {
    hybridsearch::ColumnExpr* column_expr = new hybridsearch::ColumnExpr();
    ParserHelper::ToLower($1);
    column_expr->names_.emplace_back($1);
    free($1);
    $$ = column_expr;
}
| column_expr '.' IDENTIFIER {
    hybridsearch::ColumnExpr* column_expr = (hybridsearch::ColumnExpr*)$1;
    ParserHelper::ToLower($3);
    column_expr->names_.emplace_back($3);
    free($3);
    $$ = column_expr;
}
| '*' {
    hybridsearch::ColumnExpr* column_expr = new hybridsearch::ColumnExpr();
    column_expr->star_ = true;
    $$ = column_expr;
}
| column_expr '.' '*' {
    hybridsearch::ColumnExpr* column_expr = (hybridsearch::ColumnExpr*)$1;
    if(column_expr->star_) {
        yyerror(&yyloc, scanner, result, "Invalid column expression format");
        YYERROR;
    }
    column_expr->star_ = true;
    $$ = column_expr;
};

constant_expr: STRING {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kString);
    const_expr->str_value_ = $1;
    $$ = const_expr;
}
| TRUE {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kBoolean);
    const_expr->bool_value_ = true;
    $$ = const_expr;
}
| FALSE {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kBoolean);
    const_expr->bool_value_ = false;
    $$ = const_expr;
}
| DOUBLE_VALUE {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kDouble);
    const_expr->double_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInteger);
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| DATE STRING {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kDate);
    const_expr->date_value_ = $2;
    $$ = const_expr;
}
| TIME STRING {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kTime);
    const_expr->date_value_ = $2;
    $$ = const_expr;
}
| DATETIME STRING {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kDateTime);
    const_expr->date_value_ = $2;
    $$ = const_expr;
}
| TIMESTAMP STRING {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kTimestamp);
    const_expr->date_value_ = $2;
    $$ = const_expr;
}
| INTERVAL interval_expr {
    $$ = $2;
}
| interval_expr {
    $$ = $1;
}
| common_array_expr {
    $$ = $1;
}
| curly_brackets_expr {
    $$ = $1;
}

common_array_expr: array_expr {
    $$ = $1;
}
| subarray_array_expr {
    $$ = $1;
}
| sparse_array_expr {
    $$ = $1;
}
| empty_array_expr {
    $$ = $1;
}

common_sparse_array_expr: sparse_array_expr {
    $$ = $1;
}
| array_expr {
    $$ = $1;
}
| empty_array_expr {
    $$ = $1;
}

subarray_array_expr: unclosed_subarray_array_expr ']' {
    $$ = $1;
};

unclosed_subarray_array_expr: '[' common_array_expr {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kSubArrayArray);
    const_expr->sub_array_array_.emplace_back($2);
    $$ = const_expr;
}
| unclosed_subarray_array_expr ',' common_array_expr {
    $1->sub_array_array_.emplace_back($3);
    $$ = $1;
}

sparse_array_expr: long_sparse_array_expr {
    $$ = $1;
}
| double_sparse_array_expr {
    $$ = $1;
}

long_sparse_array_expr: unclosed_long_sparse_array_expr ']' {
    $$ = $1;
}

unclosed_long_sparse_array_expr: '[' int_sparse_ele {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kLongSparseArray);
    const_expr->long_sparse_array_.first.emplace_back($2->first);
    const_expr->long_sparse_array_.second.emplace_back($2->second);
    delete $2;
    $$ = const_expr;
}
| unclosed_long_sparse_array_expr ',' int_sparse_ele {
    $1->long_sparse_array_.first.emplace_back($3->first);
    $1->long_sparse_array_.second.emplace_back($3->second);
    delete $3;
    $$ = $1;
}

double_sparse_array_expr: unclosed_double_sparse_array_expr ']' {
    $$ = $1;
}

unclosed_double_sparse_array_expr: '[' float_sparse_ele {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kDoubleSparseArray);
    const_expr->double_sparse_array_.first.emplace_back($2->first);
    const_expr->double_sparse_array_.second.emplace_back($2->second);
    delete $2;
    $$ = const_expr;
}
| unclosed_double_sparse_array_expr ',' float_sparse_ele {
    $1->double_sparse_array_.first.emplace_back($3->first);
    $1->double_sparse_array_.second.emplace_back($3->second);
    delete $3;
    $$ = $1;
}

empty_array_expr: '[' ']' {
    $$ = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kEmptyArray);
}

curly_brackets_expr: unclosed_curly_brackets_expr '}' {
    $$ = $1;
}
| '{' '}' {
    $$ = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kCurlyBracketsArray);
}

unclosed_curly_brackets_expr: '{' constant_expr {
    $$ = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kCurlyBracketsArray);
    $$->curly_brackets_array_.emplace_back($2);
}
| unclosed_curly_brackets_expr ',' constant_expr {
    $1->curly_brackets_array_.emplace_back($3);
    $$ = $1;
}

int_sparse_ele: LONG_VALUE ':' LONG_VALUE {
    $$ = new std::pair<int64_t, int64_t>{$1, $3};
}

float_sparse_ele: LONG_VALUE ':' DOUBLE_VALUE {
    $$ = new std::pair<int64_t, double>{$1, $3};
}

array_expr: long_array_expr {
    $$ = $1;
}
| double_array_expr {
    $$ = $1;
}

long_array_expr: unclosed_long_array_expr ']' {
    $$ = $1;
};

unclosed_long_array_expr: '[' LONG_VALUE {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kIntegerArray);
    const_expr->long_array_.emplace_back($2);
    $$ = const_expr;
}
| unclosed_long_array_expr ',' LONG_VALUE {
    $1->long_array_.emplace_back($3);
    $$ = $1;
}

double_array_expr: unclosed_double_array_expr ']' {
    $$ = $1;
};

unclosed_double_array_expr: '[' DOUBLE_VALUE {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kDoubleArray);
    const_expr->double_array_.emplace_back($2);
    $$ = const_expr;
}
| unclosed_double_array_expr ',' DOUBLE_VALUE {
    $1->double_array_.emplace_back($3);
    $$ = $1;
}

interval_expr: LONG_VALUE SECONDS {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kSecond;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE SECOND {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kSecond;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE MINUTES {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kMinute;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE MINUTE {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kMinute;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE HOURS {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kHour;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE HOUR {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kHour;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE DAYS {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kDay;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE DAY {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kDay;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE MONTHS {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kMonth;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE MONTH {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kMonth;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE YEARS {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kYear;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
}
| LONG_VALUE YEAR {
    hybridsearch::ConstantExpr* const_expr = new hybridsearch::ConstantExpr(hybridsearch::LiteralType::kInterval);
    const_expr->interval_type_ = hybridsearch::TimeUnit::kYear;
    const_expr->integer_value_ = $1;
    $$ = const_expr;
};

/*
 * Misc.
 */

copy_option_list : copy_option {
    $$ = new std::vector<hybridsearch::CopyOption*>();
    $$->push_back($1);
}
| copy_option_list ',' copy_option {
    $1->push_back($3);
    $$ = $1;
};

copy_option : FORMAT IDENTIFIER {
    $$ = new hybridsearch::CopyOption();
    $$->option_type_ = hybridsearch::CopyOptionType::kFormat;
    if (strcasecmp($2, "csv") == 0) {
        $$->file_type_ = hybridsearch::CopyFileType::kCSV;
        free($2);
    } else if (strcasecmp($2, "json") == 0) {
        $$->file_type_ = hybridsearch::CopyFileType::kJSON;
        free($2);
    } else if (strcasecmp($2, "jsonl") == 0) {
        $$->file_type_ = hybridsearch::CopyFileType::kJSONL;
        free($2);
    } else if (strcasecmp($2, "fvecs") == 0) {
        $$->file_type_ = hybridsearch::CopyFileType::kFVECS;
        free($2);
    } else if (strcasecmp($2, "csr") == 0) {
        $$->file_type_ = hybridsearch::CopyFileType::kCSR;
        free($2);
    } else if (strcasecmp($2, "bvecs") == 0) {
        $$->file_type_ = hybridsearch::CopyFileType::kBVECS;
        free($2);
    } else if (strcasecmp($2, "parquet") == 0) {
        $$->file_type_ = hybridsearch::CopyFileType::kPARQUET;
        free($2);
    } else {
        free($2);
        delete $$;
        yyerror(&yyloc, scanner, result, "Unknown file format");
        YYERROR;
    }
}
| DELIMITER STRING {
    $$ = new hybridsearch::CopyOption();
    $$->option_type_ = hybridsearch::CopyOptionType::kDelimiter;
    if(strlen($2) > 1 && $2[0] == '\\') {
        if($2[1] == 't') $$->delimiter_ = '\t';
    }else {
        $$->delimiter_ = $2[0];
    }
    free($2);
}
| HEADER {
    $$ = new hybridsearch::CopyOption();
    $$->option_type_ = hybridsearch::CopyOptionType::kHeader;
    $$->header_ = true;
};
| OFFSET LONG_VALUE {
    $$ = new hybridsearch::CopyOption();
    $$->option_type_ = hybridsearch::CopyOptionType::kOffset;
    $$->offset_ = $2;
}
| LIMIT LONG_VALUE {
    $$ = new hybridsearch::CopyOption();
    $$->option_type_ = hybridsearch::CopyOptionType::kLimit;
    $$->limit_ = $2;
}
| ROWLIMIT LONG_VALUE {
    $$ = new hybridsearch::CopyOption();
    $$->option_type_ = hybridsearch::CopyOptionType::kRowLimit;
    $$->row_limit_ = $2;
}

file_path : STRING {
    $$ = $1;
};

if_exists: IF EXISTS { $$ = true; }
| { $$ = false; };

if_not_exists : IF NOT EXISTS { $$ = true; }
| { $$ = false; };

semicolon : ';'
| /* nothing */
;

/* if_exists_info : if_exists IDENTIFIER {
    $$ = new hybridsearch::IfExistsInfo();
    $$->exists_ = true;
    $$->if_exists_ = $1;
    ParserHelper::ToLower($2);
    $$->info_ = $2;
    free($2);
} */

if_not_exists_info : if_not_exists IDENTIFIER {
    $$ = new hybridsearch::IfNotExistsInfo();
    $$->exists_ = true;
    $$->if_not_exists_ = $1;
    ParserHelper::ToLower($2);
    $$->info_ = $2;
    free($2);
}
| {
    $$ = new hybridsearch::IfNotExistsInfo();
}

with_index_param_list : WITH '(' index_param_list ')' {
    $$ = $3;
}
| {
    $$ = new std::vector<hybridsearch::InitParameter*>();
}

optional_table_properties_list : PROPERTIES '(' index_param_list ')' {
    $$ = $3;
}
| {
    $$ = nullptr;
}

index_param_list : index_param {
    $$ = new std::vector<hybridsearch::InitParameter*>();
    $$->push_back($1);
}
| index_param_list ',' index_param {
    $1->push_back($3);
    $$ = $1;
};

index_param : IDENTIFIER {
    ParserHelper::ToLower($1);
    $$ = new hybridsearch::InitParameter();
    $$->param_name_ = $1;
    free($1);
}
| IDENTIFIER '=' IDENTIFIER {
    ParserHelper::ToLower($1);
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::InitParameter();
    $$->param_name_ = $1;
    free($1);

    $$->param_value_ = $3;
    free($3);
}
| IDENTIFIER '=' STRING {
    ParserHelper::ToLower($1);
    ParserHelper::ToLower($3);
    $$ = new hybridsearch::InitParameter();
    $$->param_name_ = $1;
    free($1);

    $$->param_value_ = $3;
    free($3);
}
| IDENTIFIER '=' LONG_VALUE {
    ParserHelper::ToLower($1);
    $$ = new hybridsearch::InitParameter();
    $$->param_name_ = $1;
    free($1);

    $$->param_value_ = std::to_string($3);
}
| IDENTIFIER '=' DOUBLE_VALUE {
    ParserHelper::ToLower($1);
    $$ = new hybridsearch::InitParameter();
    $$->param_name_ = $1;
    free($1);

    $$->param_value_ = std::to_string($3);
};

/* CREATE INDEX [[IF NOT EXISTS] index_name] ON table_name
(column1[, ...column2]) USING method [WITH (param[, ...param])] (column1[, ...column2]) USING method [WITH (param[, ...param])]; */
index_info : '(' IDENTIFIER ')' USING IDENTIFIER with_index_param_list {
    ParserHelper::ToLower($5);
    hybridsearch::IndexType index_type = hybridsearch::IndexType::kInvalid;
    if(strcmp($5, "fulltext") == 0) {
        index_type = hybridsearch::IndexType::kFullText;
    } else if (strcmp($5, "hnsw") == 0) {
        index_type = hybridsearch::IndexType::kHnsw;
    } else if (strcmp($5, "bmp") == 0) {
        index_type = hybridsearch::IndexType::kBMP;
    } else if (strcmp($5, "ivf") == 0) {
        index_type = hybridsearch::IndexType::kIVF;
    } else if (strcmp($5, "emvb") == 0) {
        index_type = hybridsearch::IndexType::kEMVB;
    } else if(strcmp($5, "diskann") == 0){
        index_type = hybridsearch::IndexType::kDiskAnn;
    } else {
        free($5);
        free($2);
        delete $6;
        yyerror(&yyloc, scanner, result, "Unknown index type");
        YYERROR;
    }
    free($5);

    $$ = new hybridsearch::IndexInfo();

    $$->index_type_ = index_type;
    $$->column_name_ = $2;
    $$->index_param_list_ = $6;
    free($2);
}
| '(' IDENTIFIER ')' {
    $$ = new hybridsearch::IndexInfo();
    $$->index_type_ = hybridsearch::IndexType::kSecondary;
    $$->column_name_ = $2;
    free($2);
}

%%

void
yyerror(YYLTYPE * llocp, void* lexer, hybridsearch::ParserResult* result, const char* msg) {
    if(result->IsError()) return ;

    result->error_message_ = std::string(msg) + ", " + std::to_string(llocp->first_column);
	fprintf(stderr, "Error: %s, %d:%d\n", msg, llocp->first_line, llocp->first_column);
}
