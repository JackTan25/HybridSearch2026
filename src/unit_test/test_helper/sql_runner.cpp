

module;

#include <memory>

module sql_runner;

import stl;
import data_table;
import logger;
import session;
import query_context;
import hybridsearch_context;
import third_party;
import sql_parser;
import logical_planner;
import optimizer;
import physical_planner;
import fragment_builder;
import bind_context;
import logical_node;
import physical_operator;
import task_scheduler;
import fragment_context;
import fragment_task;

import hybridsearch_exception;
import singleton;
import resource_manager;
import storage;
import query_result;
import session_manager;
import base_statement;
import parser_result;
import persistence_manager;

namespace hybridsearch {

/**
 * @brief For testing the new push based execution engine
 * @param sql_text
 * @param print
 * @return Table
 */
SharedPtr<DataTable> SQLRunner::Run(const String &sql_text, bool print) {
    //    if (print) {
    //        LOG_TRACE(fmt::format("{}", sql_text));
    //    }

    //    UniquePtr<SessionManager> session_manager = MakeUnique<SessionManager>();
    SharedPtr<RemoteSession> session_ptr = hybridsearchContext::instance().session_manager()->CreateRemoteSession();

    UniquePtr<QueryContext> query_context_ptr = MakeUnique<QueryContext>(session_ptr.get());
    query_context_ptr->Init(hybridsearchContext::instance().config(),
                            hybridsearchContext::instance().task_scheduler(),
                            hybridsearchContext::instance().storage(),
                            hybridsearchContext::instance().resource_manager(),
                            hybridsearchContext::instance().session_manager(),
                            hybridsearchContext::instance().persistence_manager());
    query_context_ptr->set_current_schema(session_ptr->current_database());

    SharedPtr<SQLParser> parser = MakeShared<SQLParser>();
    SharedPtr<ParserResult> parsed_result = MakeShared<ParserResult>();
    parser->Parse(sql_text, parsed_result.get());

    if (parsed_result->IsError()) {
        UnrecoverableError(parsed_result->error_message_);
    }

    //    LogicalPlanner logical_planner(query_context_ptr.get());
    //    Optimizer optimizer(query_context_ptr.get());
    //    PhysicalPlanner physical_planner(query_context_ptr.get());
    //    FragmentBuilder fragment_builder(query_context_ptr.get());
    BaseStatement *statement = (*parsed_result->statements_ptr_)[0];

    query_context_ptr->BeginTxn(statement);

    SharedPtr<BindContext> bind_context;
    query_context_ptr->logical_planner()->Build(statement, bind_context);
    query_context_ptr->set_max_node_id(bind_context->GetNewLogicalNodeId());

    SharedPtr<LogicalNode> logical_plan = query_context_ptr->logical_planner()->LogicalPlans()[0];

    // Apply optimized rule to the logical plan
    query_context_ptr->optimizer()->optimize(logical_plan, statement->Type());

    // Build physical plan
    SharedPtr<PhysicalOperator> physical_plan = query_context_ptr->physical_planner()->BuildPhysicalOperator(logical_plan);

    // Create execution pipeline
    // Fragment Builder, only for test now. plan fragment is same as pipeline.
    auto plan_fragment = query_context_ptr->fragment_builder()->BuildFragment({physical_plan.get()});

    auto notifier = MakeUnique<Notifier>();

    FragmentContext::BuildTask(query_context_ptr.get(), nullptr, plan_fragment.get(), notifier.get());

    // Schedule the query tasks
    query_context_ptr->scheduler()->Schedule(plan_fragment.get(), statement);

    // Initialize query result
    QueryResult query_result;
    query_result.result_table_ = plan_fragment->GetResult();
    query_result.root_operator_type_ = logical_plan->operator_type();

    parsed_result->Reset();
    query_context_ptr->CommitTxn();
    return query_result.result_table_;
}

} // namespace hybridsearch
