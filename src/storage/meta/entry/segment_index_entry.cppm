

module;

#include <cassert>

export module segment_index_entry;

import stl;
import internal_types;
import buffer_handle;
import third_party;
import buffer_obj;
import base_entry;
import index_file_worker;
import status;
import index_base;
import column_def;
import cleanup_scanner;
import chunk_index_entry;
import default_values;
import statement_common;
import txn;
import snapshot_info;
import meta_info;

namespace hybridsearch {

class TxnTableStore;
struct TableIndexEntry;
struct BlockEntry;
struct TableEntry;
class BufferManager;
struct SegmentEntry;
struct TableEntry;
class HnswIndexInMem;
class IVFIndexInMem;
class SecondaryIndexInMem;
class EMVBIndexInMem;
class BMPIndexInMem;
class BaseMemIndex;
class MemoryIndexer;
class TxnStore;

export struct PopulateEntireConfig {
    bool prepare_;
    bool check_ts_;
};

export struct SegmentIndexEntry final : public BaseEntry {
public:
    static Vector<std::string_view> DecodeIndex(std::string_view encode);

    static String EncodeIndex(const SegmentID segment_id, const TableIndexEntry *table_index_entry);

    static SharedPtr<SegmentIndexEntry> NewIndexEntry(TableIndexEntry *table_index_entry, SegmentID segment_id, Txn *txn);

    static SharedPtr<SegmentIndexEntry> NewReplaySegmentIndexEntry(TableIndexEntry *table_index_entry,
                                                                   TableEntry *table_entry,
                                                                   SegmentID segment_id,
                                                                   BufferManager *buffer_manager,
                                                                   TxnTimeStamp min_ts,
                                                                   TxnTimeStamp max_ts,
                                                                   u32 next_chunk_id,
                                                                   TransactionID txn_id,
                                                                   TxnTimeStamp begin_ts,
                                                                   TxnTimeStamp commit_ts,
                                                                   TxnTimeStamp deprecate_ts);

    void UpdateSegmentIndexReplay(SharedPtr<SegmentIndexEntry> new_entry);

    // static String IndexFileName(SegmentID segment_id);

    nlohmann::json Serialize(TxnTimeStamp max_commit_ts);

    void SaveIndexFile();

    static UniquePtr<SegmentIndexEntry>
    Deserialize(const nlohmann::json &index_entry_json, TableIndexEntry *table_index_entry, BufferManager *buffer_mgr, TableEntry *table_entry);

    void CommitSegmentIndex(TransactionID txn_id, TxnTimeStamp commit_ts);

    void CommitOptimize(ChunkIndexEntry *new_chunk, const Vector<ChunkIndexEntry *> &old_chunks, TxnTimeStamp commit_ts);

    void OptIndex(SharedPtr<IndexBase> index_base, TxnTableStore *txn_table_store, const Vector<UniquePtr<InitParameter>> &opt_params, bool replay);

    bool Flush(TxnTimeStamp checkpoint_ts);

    void CommitIndex(TxnTimeStamp commit_ts);

    void Cleanup(CleanupInfoTracer *info_tracer = nullptr, bool dropped = true) final;

    Vector<String> GetFilePath(Txn* txn) const final;

    void PickCleanup(CleanupScanner *scanner) final;

    TableIndexEntry *table_index_entry() const { return table_index_entry_; }

public:
    // Getter
    inline SegmentID segment_id() const { return segment_id_; }
    inline TxnTimeStamp min_ts() const { return min_ts_; }
    inline TxnTimeStamp max_ts() const { return max_ts_; }
    inline ChunkID next_chunk_id() const { return next_chunk_id_; }
    inline TxnTimeStamp deprecate_ts() const { return deprecate_ts_; }
    SharedPtr<String> index_dir() const { return index_dir_; }

    // MemIndexInsert is non-blocking. Caller must ensure there's no RowID gap between each call.
    void MemIndexInsert(SharedPtr<BlockEntry> block_entry, u32 row_offset, u32 row_count, TxnTimeStamp commit_ts, BufferManager *buffer_manager, TxnStore *txn_store);

    // User shall invoke this reguarly to populate recently inserted rows into the fulltext index. Noop for other types of index.
    void MemIndexCommit();

    // Wait MemIndex inflight tasks done.
    void MemIndexWaitInflightTasks();

    // Dump or spill the memory indexer
    SharedPtr<ChunkIndexEntry> MemIndexDump(bool spill = false, SizeT *dump_size = nullptr);

    void AddWalIndexDump(ChunkIndexEntry *dumped_index_entry, Txn *txn, Vector<ChunkID> chunk_ids = {});

    // // Init the mem index from previously spilled one.
    // void MemIndexLoad(const String &base_name, RowID base_row_id);

    // Populate index entirely for the segment
    void PopulateEntirely(const SegmentEntry *segment_entry, Txn *txn, const PopulateEntireConfig &config);

    u32 MemIndexRowCount();

    Status CreateIndexPrepare(const SegmentEntry *segment_entry, Txn *txn, bool prepare, bool check_ts);

    Status CreateIndexDo(atomic_u64 &create_index_idx);

    void GetChunkIndexEntries(Vector<SharedPtr<ChunkIndexEntry>> &chunk_index_entries, SharedPtr<MemoryIndexer> &memory_indexer, Txn *txn = nullptr);

    Vector<SharedPtr<ChunkIndexEntry>> GetChunks() const {
        std::shared_lock lock(rw_locker_);
        return chunk_index_entries_;
    }

    void RemoveChunkIndexEntry(ChunkIndexEntry *chunk_index_entry);

    void ReplaceChunkIndexEntries(TxnTableStore *txn_table_store,
                                  SharedPtr<ChunkIndexEntry> merged_chunk_index_entry,
                                  Vector<ChunkIndexEntry *> old_chunks);

    ChunkIndexEntry *RebuildChunkIndexEntries(TxnTableStore *txn_table_store, SegmentEntry *segment_entry);

    BaseMemIndex *GetMemIndex() const;

    Tuple<Vector<SharedPtr<ChunkIndexEntry>>, SharedPtr<HnswIndexInMem>> GetHnswIndexSnapshot() {
        std::shared_lock lock(rw_locker_);
        return {chunk_index_entries_, memory_hnsw_index_};
    }

    Tuple<Vector<SharedPtr<ChunkIndexEntry>>, SharedPtr<BMPIndexInMem>> GetBMPIndexSnapshot() {
        std::shared_lock lock(rw_locker_);
        return {chunk_index_entries_, memory_bmp_index_};
    }

    Tuple<Vector<SharedPtr<ChunkIndexEntry>>, SharedPtr<IVFIndexInMem>> GetIVFIndexSnapshot() {
        std::shared_lock lock(rw_locker_);
        return {chunk_index_entries_, memory_ivf_index_};
    }

    Tuple<Vector<SharedPtr<ChunkIndexEntry>>, SharedPtr<SecondaryIndexInMem>> GetSecondaryIndexSnapshot() {
        std::shared_lock lock(rw_locker_);
        return {chunk_index_entries_, memory_secondary_index_};
    }

    Tuple<Vector<SharedPtr<ChunkIndexEntry>>, SharedPtr<EMVBIndexInMem>> GetEMVBIndexSnapshot() {
        std::shared_lock lock(rw_locker_);
        return {chunk_index_entries_, memory_emvb_index_};
    }

    Pair<u64, u32> GetFulltextColumnLenInfo();

    void UpdateFulltextColumnLenInfo(u64 column_len_sum, u32 column_len_cnt) {
        std::unique_lock lock(rw_locker_);
        ft_column_len_sum_ += column_len_sum;
        ft_column_len_cnt_ += column_len_cnt;
    }

public:
    SharedPtr<ChunkIndexEntry> CreateHnswIndexChunkIndexEntry(RowID base_rowid, u32 row_count, BufferManager *buffer_mgr, SizeT index_size);

    SharedPtr<ChunkIndexEntry> CreateSecondaryIndexChunkIndexEntry(RowID base_rowid, u32 row_count, BufferManager *buffer_mgr);

    SharedPtr<ChunkIndexEntry> CreateIVFIndexChunkIndexEntry(RowID base_rowid, u32 row_count, BufferManager *buffer_mgr);

    SharedPtr<ChunkIndexEntry> CreateEMVBIndexChunkIndexEntry(RowID base_rowid, u32 row_count, BufferManager *buffer_mgr);

    SharedPtr<ChunkIndexEntry> CreateBMPIndexChunkIndexEntry(RowID base_rowid, u32 row_count, BufferManager *buffer_mgr, SizeT index_size);

    void AddChunkIndexEntry(SharedPtr<ChunkIndexEntry> chunk_index_entry);

    SharedPtr<ChunkIndexEntry> AddFtChunkIndexEntry(const String &base_name, RowID base_rowid, u32 row_count);

    SharedPtr<ChunkIndexEntry> AddChunkIndexEntryReplayWal(ChunkID chunk_id,
                                                           TableEntry *table_entry,
                                                           const String &base_name,
                                                           RowID base_rowid,
                                                           u32 row_count,
                                                           TxnTimeStamp commit_ts,
                                                           TxnTimeStamp deprecate_ts,
                                                           BufferManager *buffer_mgr);

    SharedPtr<ChunkIndexEntry> AddChunkIndexEntryReplay(ChunkID chunk_id,
                                                        TableEntry *table_entry,
                                                        const String &base_name,
                                                        RowID base_rowid,
                                                        u32 row_count,
                                                        TxnTimeStamp commit_ts,
                                                        TxnTimeStamp deprecate_ts,
                                                        BufferManager *buffer_mgr);

    ChunkIndexEntry *GetChunkIndexEntry(ChunkID chunk_id);

    // only for unittest
    MemoryIndexer *GetMemoryIndexer() { return memory_indexer_.get(); }
    void SetMemoryIndexer(UniquePtr<MemoryIndexer> &&memory_indexer);
    static SharedPtr<SegmentIndexEntry> CreateFakeEntry(const String &index_dir);

    ChunkID GetNextChunkID() { return next_chunk_id_++; }

    void SetDeprecated(TxnTimeStamp deprecate_ts);

    bool CheckDeprecate(TxnTimeStamp ts) {
        TxnTimeStamp deprecate_ts = deprecate_ts_.load();
        return ts >= deprecate_ts;
    }

    SharedPtr<SegmentIndexInfo> GetSegmentIndexInfo(Txn* txn_ptr) const;

    SharedPtr<SegmentIndexSnapshotInfo> GetSnapshotInfo(Txn *txn_ptr) const;
private:
    explicit SegmentIndexEntry(TableIndexEntry *table_index_entry, SegmentID segment_id);

    SegmentIndexEntry(const SegmentIndexEntry &other);

public:
    ~SegmentIndexEntry() override;

    UniquePtr<SegmentIndexEntry> Clone(TableIndexEntry *table_index_entry) const;

private:
    BufferManager *buffer_manager_{};
    TableIndexEntry *table_index_entry_;
    SharedPtr<String> index_dir_{};
    const SegmentID segment_id_{};

    mutable std::shared_mutex rw_locker_{};

    TxnTimeStamp min_ts_{0}; // Indicate the commit_ts which create this SegmentIndexEntry
    TxnTimeStamp max_ts_{0}; // Indicate the max commit_ts which update data inside this SegmentIndexEntry
    TxnTimeStamp checkpoint_ts_{0};
    Atomic<ChunkID> next_chunk_id_{0};

    Vector<SharedPtr<ChunkIndexEntry>> chunk_index_entries_{};

    std::mutex mem_index_locker_{};
    SharedPtr<HnswIndexInMem> memory_hnsw_index_{};
    SharedPtr<IVFIndexInMem> memory_ivf_index_{};
    SharedPtr<MemoryIndexer> memory_indexer_{};
    SharedPtr<SecondaryIndexInMem> memory_secondary_index_{};
    SharedPtr<EMVBIndexInMem> memory_emvb_index_{};
    SharedPtr<BMPIndexInMem> memory_bmp_index_{};

    u64 ft_column_len_sum_{}; // increase only
    u32 ft_column_len_cnt_{}; // increase only
    Atomic<TxnTimeStamp> deprecate_ts_ = UNCOMMIT_TS;

public:
    Pair<bool, std::function<void()>> TrySetOptimizing(Txn *txn);

    void ResetOptimizing();

private:
    Atomic<bool> optimizing_ = false;
};

} // namespace hybridsearch
