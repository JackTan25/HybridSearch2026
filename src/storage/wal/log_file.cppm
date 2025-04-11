

module;

export module log_file;

import stl;

// responsible for parsing and generating filenames for catalog files and wal files

namespace hybridsearch {

export struct FullCatalogFileInfo {
    String path_;
    TxnTimeStamp max_commit_ts_;
};

export struct DeltaCatalogFileInfo {
    String path_;
    TxnTimeStamp max_commit_ts_;
};

export struct TempFullCatalogFileInfo {
    String path_;
    TxnTimeStamp max_commit_ts_;
};

export struct TempDeltaCatalogFileInfo {
    String path_;
    TxnTimeStamp max_commit_ts_;
};

export struct CatalogFilesInfo {
    Vector<FullCatalogFileInfo> full_infos_;
    Vector<DeltaCatalogFileInfo> delta_infos_;
    Vector<TempFullCatalogFileInfo> temp_full_infos_;
    Vector<TempDeltaCatalogFileInfo> temp_delta_infos_;
};

export struct TempWalFileInfo {
    String path_;
};

export struct WalFileInfo {
    String path_;
    TxnTimeStamp max_commit_ts_;
};

export class CatalogFile {
public:
    static Optional<Pair<FullCatalogFileInfo, Vector<DeltaCatalogFileInfo>>> ParseValidCheckpointFilenames(const String &catalog_dir,
                                                                                                           TxnTimeStamp max_checkpoint_ts);

    static String FullCheckpointFilename(TxnTimeStamp max_commit_ts);

    static String TempFullCheckpointFilename(TxnTimeStamp max_commit_ts);

    static String DeltaCheckpointFilename(TxnTimeStamp max_commit_ts);

    static String TempDeltaCheckpointFilename(TxnTimeStamp max_commit_ts);

    // max_commit_ts is the largest commit ts before the latest full checkpoint
    static void RecycleCatalogFile(TxnTimeStamp max_commit_ts, const String &catalog_dir);

    static CatalogFilesInfo ParseCheckpointFilenames(const String &catalog_dir);
};

export class WalFile {
public:
    static Pair<Optional<TempWalFileInfo>, Vector<WalFileInfo>> ParseWalFilenames(const String &wal_dir);

    static String WalFilename(TxnTimeStamp max_commit_ts);

    static String TempWalFilename();

    // max_commit_ts is the largest commit ts before the latest checkpoint
    static void RecycleWalFile(TxnTimeStamp max_commit_ts, const String &wal_dir);
};

} // namespace hybridsearch
