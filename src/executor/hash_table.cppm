

module;

export module hash_table;

import stl;
import column_vector;
import internal_types;
import data_type;

namespace hybridsearch {

class HashTableBase {
public:
    bool Initialized() const { return !types_.empty(); }

    void Init(Vector<SharedPtr<DataType>> types);

    void GetHashKey(const Vector<SharedPtr<ColumnVector>> &columns, SizeT row_id, String &hash_key) const;

public:
    Vector<SharedPtr<DataType>> types_{};
    SizeT key_size_{};
};

export class HashTable : public HashTableBase {
public:
    void Append(const Vector<SharedPtr<ColumnVector>> &columns, SizeT block_id, SizeT row_count);

public:
    // Key -> (block id -> row array)
    HashMap<String, HashMap<SizeT, Vector<SizeT>>> hash_table_{};
};

export class MergeHashTable : public HashTableBase {
public:
    void Append(const Vector<SharedPtr<ColumnVector>> &columns, SizeT block_id, SizeT row_count);

    bool GetOrInsert(const Vector<SharedPtr<ColumnVector>> &columns, SizeT row_id, Pair<SizeT, SizeT> &block_row_id);

public:
    // Key -> (block id, row id)
    HashMap<String, Pair<SizeT, SizeT>> hash_table_{};
};

} // namespace hybridsearch
