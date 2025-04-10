
%{
#include <stdint.h>
#include <stdio.h>
#include "expression_parser.h"
#include "expression_lexer.h"
#include "parser_helper.h"

void expressionerror(YYLTYPE * llocp, void* lexer, hybridsearch::ExpressionParserResult* result, const char* msg);
%}

%code requires {

#include "expression.h"
#include "expression_parser_result.h"
#include "definition/column_def.h"
#include "type/info/decimal_info.h"
#include "type/info/embedding_info.h"

#include <vector>

#define YYSTYPE EXPRESSIONSTYPE
#define YYLTYPE EXPRESSIONLTYPE

struct EXPRESSION_LTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;

    int total_column;
    int string_length;

    std::vector<void*> parameters;
};

#define EXPRESSIONLTYPE EXPRESSION_LTYPE
#define EXPRESSIONLTYPE_IS_DECLARED 1

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

%define api.prefix {expression}

%lex-param {void *scanner}
%parse-param {void *scanner} {hybridsearch::ExpressionParserResult* result}

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

    hybridsearch::ParsedExpr*             expr_t;
    hybridsearch::ConstantExpr*           const_expr_t;
    std::vector<hybridsearch::ParsedExpr*>*    expr_array_t;
}

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
    delete ($$);
} <expr_t>

%destructor {
    delete ($$);
} <const_expr_t>

%token <str_value>              IDENTIFIER STRING
%token <double_value>           DOUBLE_VALUE
%token <long_value>             LONG_VALUE

/* EXPRESSION keywords */

%token CREATE SELECT INSERT DROP UPDATE DELETE COPY SET EXPLAIN SHOW ALTER EXECUTE PREPARE UNION ALL INTERSECT COMPACT
%token EXCEPT FLUSH USE OPTIMIZE PROPERTIES
%token DATABASE TABLE COLLECTION TABLES INTO VALUES AST PIPELINE RAW LOGICAL PHYSICAL FRAGMENT VIEW INDEX ANALYZE VIEWS DATABASES SEGMENT SEGMENTS BLOCK BLOCKS COLUMNS INDEXES
%token GROUP BY HAVING AS NATURAL JOIN LEFT RIGHT OUTER FULL ON INNER CROSS DISTINCT WHERE ORDER LIMIT OFFSET ASC DESC
%token IF NOT EXISTS IN FROM TO WITH DELIMITER FORMAT HEADER CAST END CASE ELSE THEN WHEN
%token BOOLEAN INTEGER INT TINYINT SMALLINT BIGINT HUGEINT VARCHAR FLOAT DOUBLE REAL DECIMAL DATE TIME DATETIME
%token TIMESTAMP UUID POINT LINE LSEG BOX PATH POLYGON CIRCLE BLOB BITMAP EMBEDDING VECTOR BIT SPARSE
%token PRIMARY KEY UNIQUE NULLABLE IS
%token TRUE FALSE INTERVAL SECOND SECONDS MINUTE MINUTES HOUR HOURS DAY DAYS MONTH MONTHS YEAR YEARS
%token EQUAL NOT_EQ LESS_EQ GREATER_EQ BETWEEN AND OR EXTRACT LIKE
%token DATA LOG BUFFER
%token KNN USING SESSION GLOBAL OFF EXPORT PROFILE CONFIGS PROFILES STATUS VAR
%token SEARCH MATCH QUERY FUSION

%token NUMBER

/* nonterminal symbol */

%type <expr_t>                  expr expr_alias column_expr function_expr operand
%type <expr_t>                  in_expr between_expr
%type <expr_t>                  conjunction_expr
%type <expr_t>                  match_expr query_expr fusion_expr
%type <const_expr_t>            constant_expr interval_expr
%type <const_expr_t>            long_array_expr unclosed_long_array_expr double_array_expr unclosed_double_array_expr
%type <expr_array_t>            expr_array

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

%%

input_pattern : expr_array {
    result->exprs_ptr_ = $1;
};

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
| in_expr
| between_expr
| conjunction_expr;

operand: '(' expr ')' {
   $$ = $2;
}
| constant_expr {
    $$ = $1;
}
| column_expr
| function_expr
| match_expr
| query_expr
| fusion_expr

match_expr : MATCH '(' STRING ',' STRING ')' {
    hybridsearch::MatchExpr* match_expr = new hybridsearch::MatchExpr();
    match_expr->fields_ = std::string($3);
    match_expr->matching_text_ = std::string($5);
    free($3);
    free($5);
    $$ = match_expr;
}
| MATCH '(' STRING ',' STRING ',' STRING ')' {
    hybridsearch::MatchExpr* match_expr = new hybridsearch::MatchExpr();
    match_expr->fields_ = std::string($3);
    match_expr->matching_text_ = std::string($5);
    match_expr->options_text_ = std::string($7);
    free($3);
    free($5);
    free($7);
    $$ = match_expr;
}

query_expr : QUERY '(' STRING ')' {
    hybridsearch::MatchExpr* match_expr = new hybridsearch::MatchExpr();
    match_expr->matching_text_ = std::string($3);
    free($3);
    $$ = match_expr;
}
| QUERY '(' STRING ',' STRING ')' {
    hybridsearch::MatchExpr* match_expr = new hybridsearch::MatchExpr();
    match_expr->matching_text_ = std::string($3);
    match_expr->options_text_ = std::string($5);
    free($3);
    free($5);
    $$ = match_expr;
}

fusion_expr : FUSION '(' STRING ')' {
    hybridsearch::FusionExpr* fusion_expr = new hybridsearch::FusionExpr();
    fusion_expr->method_ = std::string($3);
    free($3);
    $$ = fusion_expr;
}
| FUSION '(' STRING ',' STRING ')' {
    hybridsearch::FusionExpr* fusion_expr = new hybridsearch::FusionExpr();
    fusion_expr->method_ = std::string($3);
    free($3);
    fusion_expr->SetOptions($5);
    free($5);
    $$ = fusion_expr;
}

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
        expressionerror(&yyloc, scanner, result, "Invalid column expression format");
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
        expressionerror(&yyloc, scanner, result, "Invalid column expression format");
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
| long_array_expr {
    $$ = $1;
}
| double_array_expr {
    $$ = $1;
};

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

%%

void
expressionerror(YYLTYPE * llocp, void* lexer, hybridsearch::ExpressionParserResult* result, const char* msg) {
    if(result->IsError()) return ;

    result->error_message_ = std::string(msg) + ", " + std::to_string(llocp->first_column);
	fprintf(stderr, "Error: %s, %d:%d\n", msg, llocp->first_line, llocp->first_column);
}
