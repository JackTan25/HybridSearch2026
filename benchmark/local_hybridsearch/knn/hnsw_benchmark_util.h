

#pragma once

#include "CLI11.hpp"

import stl;
import virtual_store;
import local_file_handle;
import hybridsearch_exception;

using namespace hybridsearch;

namespace benchmark {

template <typename T>
Tuple<SizeT, i32, UniquePtr<T[]>> DecodeFvecsDataset(const Path &path) {
    auto [file_handle, status] = VirtualStore::Open(path.string(), FileAccessMode::kRead);
    if (!status.ok()) {
        UnrecoverableError(status.message());
    }
    i32 dim = 0;
    file_handle->Read(&dim, sizeof(dim));
    SizeT file_size = file_handle->FileSize();
    SizeT vec_num = file_size / (dim * sizeof(T) + sizeof(dim));
    auto data = MakeUniqueForOverwrite<T[]>(vec_num * dim);
    for (SizeT i = 0; i < vec_num - 1; ++i) {
        file_handle->Read(data.get() + i * dim, dim * sizeof(T));
        i32 dim1 = 0;
        file_handle->Read(&dim1, sizeof(dim1));
        if (dim1 != dim) {
            UnrecoverableError("dim not match");
        }
    }
    file_handle->Read(data.get() + (vec_num - 1) * dim, dim * sizeof(T));
    return {vec_num, dim, std::move(data)};
}

} // namespace benchmark
