

module;

export module chunk_index_entry;

import stl;
import third_party;
import internal_types;
import base_entry;
import cleanup_scanner;
import index_file_worker;
import index_base;
import buffer_handle;
import default_values;
import column_def;
import txn;
import buffer_obj;
import snapshot_info;

namespace hybridsearch {

struct SegmentIndexEntry;
struct BlockEntry;
class BufferManager;
struct SegmentEntry;

// ChunkIndexEntry is an immutable chunk of SegmentIndexEntry. MemIndexer(for fulltext) is the mutable chunk of SegmentIndexEntry.
export struct ChunkIndexEntry final : public BaseEntry {
public:
    static Vector<std::string_view> DecodeIndex(std::string_view encode);

    static String EncodeIndex(const ChunkID chunk_id, const String &base_name, const SegmentIndexEntry *segment_index_entry);

    ChunkIndexEntry(ChunkID chunk_id, SegmentIndexEntry *segment_index_entry, const String &base_name, RowID base_rowid, u32 row_count);

    ~ChunkIndexEntry() override;

private:
    ChunkIndexEntry(const ChunkIndexEntry &other);

public:
    UniquePtr<ChunkIndexEntry> Clone(SegmentIndexEntry *segment_index_entry) const;

public:
    static String IndexFileName(SegmentID segment_id, ChunkID chunk_id);

    static SharedPtr<ChunkIndexEntry> NewHnswIndexChunkIndexEntry(ChunkID chunk_id,
                                                                  SegmentIndexEntry *segment_index_entry,
                                                                  const String &base_name,
                                                                  RowID base_rowid,
                                                                  u32 row_count,
                                                                  BufferManager *buffer_mgr,
                                                                  SizeT index_size);

    static SharedPtr<ChunkIndexEntry> NewFtChunkIndexEntry(SegmentIndexEntry *segment_index_entry,
                                                           ChunkID chunk_id,
                                                           const String &base_name,
                                                           RowID base_rowid,
                                                           u32 row_count,
                                                           BufferManager *buffer_mgr);

    static SharedPtr<ChunkIndexEntry> NewSecondaryIndexChunkIndexEntry(ChunkID chunk_id,
                                                                       SegmentIndexEntry *segment_index_entry,
                                                                       const String &base_name,
                                                                       RowID base_rowid,
                                                                       u32 row_count,
                                                                       BufferManager *buffer_mgr);

    static SharedPtr<ChunkIndexEntry> NewIVFIndexChunkIndexEntry(ChunkID chunk_id,
                                                                 SegmentIndexEntry *segment_index_entry,
                                                                 const String &base_name,
                                                                 RowID base_rowid,
                                                                 u32 row_count,
                                                                 BufferManager *buffer_mgr);

    static SharedPtr<ChunkIndexEntry> NewEMVBIndexChunkIndexEntry(ChunkID chunk_id,
                                                                  SegmentIndexEntry *segment_index_entry,
                                                                  const String &base_name,
                                                                  RowID base_rowid,
                                                                  u32 row_count,
                                                                  BufferManager *buffer_mgr);

    static SharedPtr<ChunkIndexEntry> NewBMPIndexChunkIndexEntry(ChunkID chunk_id,
                                                                 SegmentIndexEntry *segment_index_entry,
                                                                 const String &base_name,
                                                                 RowID base_rowid,
                                                                 u32 row_count,
                                                                 BufferManager *buffer_mgr,
                                                                 SizeT index_size);

    static SharedPtr<ChunkIndexEntry> NewReplayChunkIndexEntry(ChunkID chunk_id,
                                                               SegmentIndexEntry *segment_index_entry,
                                                               const String &base_name,
                                                               RowID base_rowid,
                                                               u32 row_count,
                                                               TxnTimeStamp commit_ts,
                                                               TxnTimeStamp deprecate_ts,
                                                               BufferManager *buffer_mgr);

    void SetRowCount(u32 row_count) { row_count_ = row_count; }

    void AddRowCount(u32 row_count) { row_count_ += row_count; }

    u32 GetRowCount() const { return row_count_; }

    // Only for fulltext
    u64 GetColumnLengthSum() const;

    BufferHandle GetIndex();

    nlohmann::json Serialize();

    static SharedPtr<ChunkIndexEntry>
    Deserialize(const nlohmann::json &index_entry_json, SegmentIndexEntry *segment_index_entry, BufferManager *buffer_mgr);

    virtual void Cleanup(CleanupInfoTracer *info_tracer = nullptr, bool dropped = true) override;

    virtual void PickCleanup(CleanupScanner *scanner) override {}

    Vector<String> GetFilePath(Txn* txn) const override;

    void SaveIndexFile();

    BufferObj *GetBufferObj() { return buffer_obj_; }

    void DeprecateChunk(TxnTimeStamp commit_ts);

    bool CheckVisible(Txn *txn) const override;

    bool CheckDeprecate(TxnTimeStamp ts) {
        TxnTimeStamp deprecate_ts = deprecate_ts_.load();
        return ts >= deprecate_ts;
    }

    SharedPtr<ChunkIndexSnapshotInfo> GetSnapshotInfo(Txn *txn_ptr) const;

public:
    ChunkID chunk_id_;

    SegmentIndexEntry *segment_index_entry_;
    String base_name_;
    RowID base_rowid_;
    u32 row_count_;
    Atomic<TxnTimeStamp> deprecate_ts_{UNCOMMIT_TS};

private:
    BufferObj *buffer_obj_{};
};

} // namespace hybridsearch
