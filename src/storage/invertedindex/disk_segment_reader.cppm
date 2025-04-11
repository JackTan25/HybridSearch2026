

module;

export module disk_index_segment_reader;
import stl;

import segment_posting;
import index_defines;
import index_segment_reader;
import dict_reader;
import file_reader;
import posting_list_format;
import internal_types;
import term_meta;

namespace hybridsearch {
export class DiskIndexSegmentReader : public IndexSegmentReader {
public:
    DiskIndexSegmentReader(SegmentID segment_id,
                           ChunkID chunk_id,
                           const String &index_dir,
                           const String &base_name,
                           RowID base_row_id,
                           optionflag_t flag);

    virtual ~DiskIndexSegmentReader();

    bool GetSegmentPosting(const String &term, SegmentPosting &seg_posting, bool fetch_position = true) const override;

private:
    RowID base_row_id_{INVALID_ROWID};
    SharedPtr<DictionaryReader> dict_reader_;
    String posting_file_{};
    String posting_file_obj_{};
    String dict_file_{};
    u8 *data_ptr_{};
    SizeT data_len_{};
};

} // namespace hybridsearch