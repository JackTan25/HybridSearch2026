

module;

export module hybridsearch_context;

import stl;
import config;
import resource_manager;
import task_scheduler;
import storage;
import singleton;
import session_manager;
import persistence_manager;
import third_party;
import global_resource_usage;
import status;
import cluster_manager;
import admin_statement;

namespace hybridsearch {

export class hybridsearchContext : public Singleton<hybridsearchContext> {
public:
    [[nodiscard]] inline TaskScheduler *task_scheduler() noexcept { return task_scheduler_.get(); }

    [[nodiscard]] inline Config *config() noexcept { return config_.get(); }

    [[nodiscard]] inline Storage *storage() noexcept { return storage_.get(); }

    [[nodiscard]] inline PersistenceManager *persistence_manager() noexcept { return storage_->persistence_manager(); }

    [[nodiscard]] inline ResourceManager *resource_manager() noexcept { return resource_manager_.get(); }

    [[nodiscard]] inline SessionManager *session_manager() noexcept { return session_mgr_.get(); }

    [[nodiscard]] inline ClusterManager *cluster_manager() noexcept { return cluster_manager_.get(); }

    [[nodiscard]] inline ThreadPool &GetFulltextInvertingThreadPool() { return inverting_thread_pool_; }
    [[nodiscard]] inline ThreadPool &GetFulltextCommitingThreadPool() { return commiting_thread_pool_; }
    [[nodiscard]] inline ThreadPool &GetHnswBuildThreadPool() { return hnsw_build_thread_pool_; }

    NodeRole GetServerRole() const;

    void InitPhase1(const SharedPtr<String> &config_path, DefaultConfig *default_config = nullptr);
    void InitPhase2(bool admin_flag = false);
    //    void InitAdminMode(const SharedPtr<String> &config_path, bool m_flag = false, DefaultConfig *default_config = nullptr);
    Status
    ChangeServerRole(NodeRole target_role, bool from_leader = false, const String &node_name = {}, String leader_ip = {}, u16 leader_port = {});
    bool IsAdminRole() const { return GetServerRole() == NodeRole::kAdmin; }
    bool IsClusterRole() const;

    void UnInit();

    void SetIndexThreadPool();
    void RestoreIndexThreadPoolToDefault();

    void AddThriftServerFn(std::function<void()> start_func, std::function<void()> stop_func);
    void StartThriftServers();
    void StopThriftServers();

    bool hybridsearchContextStarted() const { return hybridsearch_context_started_; }
    bool hybridsearchContextInited() const { return hybridsearch_context_inited_; }

private:
    friend class Singleton;

    hybridsearchContext();
    ~hybridsearchContext();

    UniquePtr<Config> config_{};
    UniquePtr<ResourceManager> resource_manager_{};
    UniquePtr<TaskScheduler> task_scheduler_{};
    UniquePtr<Storage> storage_{};
    UniquePtr<SessionManager> session_mgr_{};
    UniquePtr<ClusterManager> cluster_manager_{};
    atomic_bool hybridsearch_context_started_{false};
    atomic_bool hybridsearch_context_inited_{false};

    // For fulltext index
    ThreadPool inverting_thread_pool_{2};
    ThreadPool commiting_thread_pool_{2};

    // For hnsw index
    ThreadPool hnsw_build_thread_pool_{2};

    mutable std::mutex mutex_;

    std::function<void()> start_servers_func_{};
    std::function<void()> stop_servers_func_{};
    atomic_bool start_server_{false};
};

} // namespace hybridsearch
