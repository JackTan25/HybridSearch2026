

module;

import stl;

export module fastpfor;

namespace hybridsearch {

export enum class FastPForCodec { SIMDFastPFor, SIMDNewPFor, StreamVByte, SIMDBitPacking };

export template <FastPForCodec Codec>
struct FastPForWrapper {
    FastPForWrapper();

    ~FastPForWrapper();

    u32 Compress(const u32 *src, u32 count, u32 *dest, SizeT &outsize) const;

    const u32 *Decompress(const u32 *src, u32 count, u32 *dest, SizeT &outsize) const;

    static void ApplyDelta(u32 *src, SizeT count);

    static void RevertDelta(u32 *src, SizeT count);

private:
    struct Impl;
    UniquePtr<Impl> impl_;
};

export using SIMDFastPFor = FastPForWrapper<FastPForCodec::SIMDFastPFor>;
export using SIMDNewPFor = FastPForWrapper<FastPForCodec::SIMDNewPFor>;
export using StreamVByte = FastPForWrapper<FastPForCodec::StreamVByte>;
export using SIMDBitPacking = FastPForWrapper<FastPForCodec::SIMDBitPacking>;

} // namespace hybridsearch