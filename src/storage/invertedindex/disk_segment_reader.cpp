
module;

#include <cassert>
#include <sys/mman.h>

module disk_index_segment_reader;

import stl;

import segment_posting;
import index_defines;
import index_segment_reader;
import file_reader;
import dict_reader;
import term_meta;
import byte_slice;
import posting_list_format;
import internal_types;
import third_party;
import byte_slice_reader;
import hybridsearch_exception;
import status;
import logger;
import persistence_manager;
import hybridsearch_context;
import persist_result_handler;
import virtual_store;

namespace hybridsearch {

DiskIndexSegmentReader::DiskIndexSegmentReader(SegmentID segment_id,
                                               ChunkID chunk_id,
                                               const String &index_dir,
                                               const String &base_name,
                                               RowID base_row_id,
                                               optionflag_t flag)
    : IndexSegmentReader(segment_id, chunk_id), base_row_id_(base_row_id) {
    Path path = Path(hybridsearchContext::instance().config()->DataDir()) / index_dir / base_name;
    String path_str = path.string();
    PersistenceManager *pm = hybridsearchContext::instance().persistence_manager();

    posting_file_ = path_str;
    posting_file_.append(POSTING_SUFFIX);
    String posting_file = posting_file_;
    if (nullptr != pm) {
        PersistResultHandler handler(pm);
        PersistReadResult result = pm->GetObjCache(posting_file);
        const ObjAddr &obj_addr = handler.HandleReadResult(result);
        if (!obj_addr.Valid()) {
            // Empty posting
            return;
        }
        posting_file_obj_ = pm->GetObjPath(obj_addr.obj_key_);
        posting_file = posting_file_obj_;
    }
    if (posting_file.empty() || std::filesystem::file_size(posting_file) == 0) {
        // Empty posting
        return;
    }
    i32 rc = VirtualStore::MmapFile(posting_file, data_ptr_, data_len_);
    assert(rc == 0);
    if (rc != 0) {
        Status status = Status::MmapFileError(posting_file);
        RecoverableError(status);
    }

    dict_file_ = path_str;
    dict_file_.append(DICT_SUFFIX);
    String dict_file = dict_file_;
    if (nullptr != pm) {
        PersistResultHandler handler(pm);
        PersistReadResult result = pm->GetObjCache(dict_file);
        const ObjAddr &obj_addr = handler.HandleReadResult(result);
        dict_file = pm->GetObjPath(obj_addr.obj_key_);
    }
    dict_reader_ = MakeShared<DictionaryReader>(dict_file, PostingFormatOption(flag));
}

DiskIndexSegmentReader::~DiskIndexSegmentReader() {
    if (data_len_ == 0)
        return;
    PersistenceManager *pm = hybridsearchContext::instance().persistence_manager();
    String posting_file = posting_file_;
    if (nullptr != pm) {
        posting_file = posting_file_obj_;
    }
    i32 rc = VirtualStore::MunmapFile(posting_file);
    assert(rc == 0);
    if (rc != 0) {
        Status status = Status::MunmapFileError(posting_file);
        RecoverableError(status);
    }
    if (nullptr != pm) {
        PersistResultHandler handler(pm);
        PersistWriteResult res1 = pm->PutObjCache(dict_file_);
        PersistWriteResult res2 = pm->PutObjCache(posting_file_);
        handler.HandleWriteResult(res1);
        handler.HandleWriteResult(res2);
    }
}

bool DiskIndexSegmentReader::GetSegmentPosting(const String &term, SegmentPosting &seg_posting, bool fetch_position) const {
    TermMeta term_meta;
    if (!dict_reader_.get() || !dict_reader_->Lookup(term, term_meta)) {
        return false;
    }
    u64 file_length = fetch_position ? (term_meta.pos_end_ - term_meta.doc_start_) : (term_meta.pos_start_ - term_meta.doc_start_);
    ByteSlice *slice = ByteSlice::NewSlice(data_ptr_ + term_meta.doc_start_, file_length);
    SharedPtr<ByteSliceList> byte_slice_list = MakeShared<ByteSliceList>(slice);
    seg_posting.Init(std::move(byte_slice_list), base_row_id_, term_meta.doc_freq_, term_meta);
    return true;
}

} // namespace hybridsearch
