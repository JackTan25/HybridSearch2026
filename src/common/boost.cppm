

module;

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

export module boost;

namespace boost {
namespace system {
export using boost::system::error_code;
}
namespace asio {
export using boost::asio::io_service;
namespace ip {
export using boost::asio::ip::tcp;
export using boost::asio::ip::make_address;
export using boost::asio::ip::address;
} // namespace ip
} // namespace asio
} // namespace boost
