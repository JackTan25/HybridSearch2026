

module;

import boost;
import pg_message;
import ring_buffer_iterator;
import default_values;
import stl;
import global_resource_usage;

export module buffer_writer;

namespace hybridsearch {

export class BufferWriter {
public:
    explicit BufferWriter(const SharedPtr<boost::asio::ip::tcp::socket> &socket) : socket_(socket) {}
    ~BufferWriter() = default;
    [[nodiscard]] SizeT size() const;

    inline static SizeT max_capacity() { return PG_MSG_BUFFER_SIZE - 1; }

    inline bool full() const { return size() == max_capacity(); }

    void send_value_i8(i8 host_value);

    void send_value_u8(u8 host_value);

    void send_value_i16(i16 host_value);

    void send_value_u16(u16 host_value);

    void send_value_i32(i32 host_value);

    void send_value_u32(u32 host_value);

    void send_string(const String &value, NullTerminator null_terminator = NullTerminator::kYes);

    // 0 means flush whole buffer.
    void flush(SizeT bytes = 0);

private:
    void try_flush(SizeT bytes);

    Array<char, PG_MSG_BUFFER_SIZE> data_{};
    RingBufferIterator start_pos_{data_};
    RingBufferIterator current_pos_{data_};
    SharedPtr<boost::asio::ip::tcp::socket> socket_{};
};

} // namespace hybridsearch
