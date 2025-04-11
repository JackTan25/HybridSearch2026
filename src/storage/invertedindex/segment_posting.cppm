

module;

import stl;
import byte_slice;
import posting_list_format;
import posting_writer;
import term_meta;
import index_defines;
import internal_types;
import file_reader;

export module segment_posting;

namespace hybridsearch {

export class SegmentPosting {
public:
    SegmentPosting() {};
    ~SegmentPosting() {};

    // for on disk segment posting
    void Init(SharedPtr<ByteSliceList> slice_list, RowID base_row_id, u64 doc_count, TermMeta &term_meta);
    void Init(SharedPtr<ByteSliceList> doc_slice_list,
              SharedPtr<ByteSliceList> pos_slice_list,
              RowID base_row_id,
              u64 doc_count,
              TermMeta &term_meta,
              u64 pos_begin,
              u64 pos_size,
              const SharedPtr<FileReader> &posting_reader);
    // for in memory segment posting
    void Init(RowID base_row_id, const SharedPtr<PostingWriter> &posting_writer);

    RowID GetBaseRowId() const { return base_row_id_; }
    void SetBaseRowId(RowID base_row_id) { base_row_id_ = base_row_id; }

    u32 GetDocCount() const { return doc_count_; }
    void SetDocCount(const u32 doc_count) { doc_count_ = doc_count; }

    const SharedPtr<PostingWriter> &GetInMemPostingWriter() const { return posting_writer_; }
    bool IsInMemorySegment() const { return posting_writer_.get(); }

    void GetInMemTermMeta(TermMeta &tm) {
        df_t df = posting_writer_->GetDF();
        tf_t ttf = posting_writer_->GetTotalTF();
        tm.SetDocFreq(df);
        tm.SetTotalTermFreq(ttf);
    }

    const TermMeta &GetTermMeta() const { return term_meta_; }

    const SharedPtr<ByteSliceList> &GetSliceListPtr() const { return slice_list_; }

    const SharedPtr<ByteSliceList> &GetDocSliceListPtr() const { return doc_slice_list_; }

    const SharedPtr<ByteSliceList> &GetPosSliceListPtr();

private:
    SharedPtr<ByteSliceList> slice_list_{nullptr};
    SharedPtr<ByteSliceList> doc_slice_list_{nullptr};
    SharedPtr<ByteSliceList> pos_slice_list_{nullptr};
    RowID base_row_id_ = INVALID_ROWID;
    u32 doc_count_ = 0;
    TermMeta term_meta_;
    SharedPtr<PostingWriter> posting_writer_{nullptr};
    SharedPtr<FileReader> posting_reader_{nullptr};
    String *path_ = nullptr;
    u64 pos_begin_ = 0;
    u64 pos_size_ = 0;
    u64 doc_start_ = 0;
};
} // namespace hybridsearch