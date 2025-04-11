

module;

export module darts_trie;

import stl;
import darts;
import status;

namespace hybridsearch {

export class POSTable {
public:
    POSTable(const String &path);

    ~POSTable() = default;

    Status Load();

    const char *GetPOS(i32 index) const;

    i32 GetPOSIndex(const String &tag) const;

private:
    String file_;
    i32 table_size_{0};
    Vector<String> pos_vec_;
    Map<String, i32> pos_map_;
};

export using DartsCore = DoubleArrayImpl<void, void, int, void>;

export struct DartsTuple {
    DartsTuple(const String &k, const int &v) : key_(k), value_(v) {}
    String key_;
    int value_;
};

export class DartsTrie {
    UniquePtr<DartsCore> darts_;
    Vector<DartsTuple> buffer_;

public:
    DartsTrie();

    void Add(const String &key, const int &value);

    void Build();

    void Load(const String &file_name);

    void Save(const String &file_name);

    bool HasKeysWithPrefix(std::string_view key) const;

    int Traverse(const char *key, SizeT &node_pos, SizeT &key_pos, SizeT length) const;

    int Get(std::string_view key) const;
};

} // namespace hybridsearch