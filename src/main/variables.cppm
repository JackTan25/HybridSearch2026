

module;

export module variables;

import stl;
import command_statement;

namespace hybridsearch {

// Variable has two scopes: global and session
export enum class GlobalVariable {
    kQueryCount,              // global
    kSessionCount,            // global
    kBufferPoolUsage,         // global
    kSchedulePolicy,          // global
    kDeltaLogCount,           // global
    kNextTxnID,               // global
    kBufferedObjectCount,     // global
    kUnusedBufferObjectCount, // global
    kActiveTxnCount,          // global
    kCurrentTs,               // global
    kTotalCommitCount,        // global
    kTotalRollbackCount,      // global
    kActiveWALFilename,       // global
    kProfileRecordCapacity,   // global
    kBackgroundTaskCount,     // global
    kRunningBGTask,           // global
    kRunningCompactTask,      // global
    kSystemMemoryUsage,       // global
    kOpenFileCount,           // global
    kCPUUsage,                // global
    kJeProf,                  // global
    kCleanupTrace,            // global
    kFollowerNum,             // global
    kResultCache,             // global
    kCacheResultCapacity,     // global
    kCacheResultNum,          // global
    kMemoryCacheMiss,         // global
    kDiskCacheMiss,           // global
    kEnableProfile,           // global
    kInvalid,
};

export enum class SessionVariable {
    kQueryCount,         // session
    kTotalCommitCount,   // session
    kTotalRollbackCount, // session
    kConnectedTime,      // session

    kInvalid,
};

export struct VarUtil {
    static HashMap<String, GlobalVariable> global_name_map_;
    static HashMap<String, SessionVariable> session_name_map_;

    static void InitVariablesMap();
    static GlobalVariable GetGlobalVarByName(const String &variable_name);
    static SessionVariable GetSessionVarByName(const String &variable_name);
};

} // namespace hybridsearch
