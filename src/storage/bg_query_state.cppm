

module;

export module bg_query_state;

import stl;
import logical_node;
import physical_operator;
import plan_fragment;
import fragment_context;
import query_context;
import hybridsearch_context;
import session_manager;
import session;
import txn;

namespace hybridsearch {

export struct BGQueryState {
    Vector<SharedPtr<LogicalNode>> logical_plans{};
    Vector<UniquePtr<PhysicalOperator>> physical_plans{};
    SharedPtr<PlanFragment> plan_fragment{};
    UniquePtr<Notifier> notifier{};
};

export struct BGQueryContextWrapper {
    UniquePtr<QueryContext> query_context_;

    SessionManager *session_mgr_;
    SharedPtr<BaseSession> session_;

    BGQueryContextWrapper(BGQueryContextWrapper &&other)
        : query_context_(std::move(other.query_context_)), session_mgr_(other.session_mgr_), session_(std::move(other.session_)) {
        other.session_mgr_ = nullptr;
    }

    BGQueryContextWrapper(Txn *txn) : session_mgr_(hybridsearchContext::instance().session_manager()) {
        session_ = session_mgr_->CreateLocalSession();
        query_context_ = MakeUnique<QueryContext>(session_.get());
        query_context_->Init(hybridsearchContext::instance().config(),
                             hybridsearchContext::instance().task_scheduler(),
                             hybridsearchContext::instance().storage(),
                             hybridsearchContext::instance().resource_manager(),
                             hybridsearchContext::instance().session_manager(),
                             hybridsearchContext::instance().persistence_manager());
        query_context_->SetTxn(txn);
    }

    ~BGQueryContextWrapper() {
        if (session_mgr_ != nullptr) {
            auto *session = query_context_->current_session();
            session_mgr_->RemoveSessionByID(session->session_id());
        }
    }
};

} // namespace hybridsearch
