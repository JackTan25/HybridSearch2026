

module;

#include "gtest/gtest.h"

#include <filesystem>
#include <stdlib.h>
#include <sys/stat.h>
#include <type_traits>
#include <unistd.h>

export module base_test;

import stl;
import hybridsearch_context;
import global_resource_usage;

namespace fs = std::filesystem;

namespace hybridsearch {

export template <typename T>
class BaseTestWithParam : public std::conditional_t<std::is_same_v<T, void>, ::testing::Test, ::testing::TestWithParam<T>> {
public:
    BaseTestWithParam() {
        const char *hybridsearch_home_ = GetHomeDir();
        bool ok = ValidateDirPermission(hybridsearch_home_);
        if (!ok) {
            std::cerr << "Please create directory " << hybridsearch_home_ << " and ensure current user has RWX permission of it." << std::endl;
            abort();
        }
        CleanupTmpDir();
    }

    ~BaseTestWithParam() override = default;

    void SetUp() override {}
    void TearDown() override {}

public:
    static constexpr const char *NULL_CONFIG_PATH = "";

    static constexpr const char *CONFIG_PATH = "test/data/config/test.toml";

    static constexpr const char *VFS_OFF_CONFIG_PATH = "test/data/config/test_vfs_off.toml";

protected:
    const char *GetHomeDir() { return "/var/hybridsearch"; }

    const char *GetFullDataDir() { return "/var/hybridsearch/data"; }

    const char *GetFullWalDir() { return "/var/hybridsearch/wal"; }

    const char *GetFullLogDir() { return "/var/hybridsearch/log"; }

    const char *GetFullTmpDir() { return "/var/hybridsearch/tmp"; }

    const char *GetFullPersistDir() { return "/var/hybridsearch/persistence"; }

    const char *GetTmpDir() { return "tmp"; }

    void CleanupDbDirs() {
        const char *hybridsearch_db_dirs[] = {GetFullDataDir(), GetFullWalDir(), GetFullLogDir(), GetFullTmpDir(), GetFullPersistDir()};
        for (auto &dir : hybridsearch_db_dirs) {
            CleanupDirectory(dir);
        }
    }

    void CleanupTmpDir() { CleanupDirectory(GetFullTmpDir()); }

    void RemoveDbDirs() {
        const char *hybridsearch_db_dirs[] = {GetFullDataDir(), GetFullWalDir(), GetFullLogDir(), GetFullTmpDir(), GetFullPersistDir()};
        for (auto &dir : hybridsearch_db_dirs) {
            RemoveDirectory(dir);
        }
    }

private:
    // Validate if given path satisfy all of following:
    // - The path is a directory or symlink to a directory.
    // - Current user has read, write, and execute permission of the path.
    bool ValidateDirPermission(const char *path_str) {
        fs::path path(path_str);
        std::error_code ec;

        // Check if the path exists and is a directory or symlink to a directory
        if (!fs::exists(path, ec) || ec)
            return false;
        if (!fs::is_directory(path, ec) && !(fs::is_symlink(path, ec) && fs::is_directory(fs::read_symlink(path), ec)))
            return false;

        // Check read and execute permission
        fs::directory_iterator it(path, fs::directory_options::skip_permission_denied, ec);
        if (ec)
            return false;

        // Check write permission
        fs::path temp_file = path / "temp_file.txt";
        std::ofstream ofs(temp_file, std::ios::out | std::ios::app);
        if (!ofs)
            return false;
        ofs.close();
        fs::remove(temp_file, ec);
        if (ec)
            return false;

        return true;
    }

    void CleanupDirectory(const char *dir) {
        std::error_code error_code;
        fs::path p(dir);
        if (!fs::exists(dir)) {
            std::filesystem::create_directories(p, error_code);
            if (error_code.value() != 0) {
                std::cerr << "Failed to create directory " << dir << std::endl;
                abort();
            }
        }
        try {
            for (const auto &dir_entry : std::filesystem::directory_iterator{dir}) {
                std::filesystem::remove_all(dir_entry.path());
            };
        } catch (const std::filesystem::filesystem_error &e) {
            std::cerr << "Failed to cleanup " << dir << ", exception: " << e.what() << std::endl;
            abort();
        }
    }

    void RemoveDirectory(const char *dir) {
        std::error_code error_code;
        fs::path p(dir);
        try {
            std::filesystem::remove_all(p, error_code);
        } catch (const std::filesystem::filesystem_error &e) {
            std::cerr << "Failed to remove " << dir << ", exception: " << e.what() << std::endl;
            abort();
        }
    }
};

export using BaseTest = BaseTestWithParam<void>;

export class BaseTestParamStr : public BaseTestWithParam<std::string> {
public:
    void SetUp() override {
        CleanupDbDirs();
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::string config_path_str = GetParam();
        std::shared_ptr<std::string> config_path = nullptr;
        if (config_path_str != BaseTestParamStr::NULL_CONFIG_PATH) {
            config_path = std::make_shared<std::string>(std::filesystem::absolute(config_path_str));
        }
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();
    }

    void TearDown() override {
        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
};

} // namespace hybridsearch
