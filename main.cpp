#include <Core/Block.h>

#include <asio/ip/tcp.hpp>
#include <asio/io_context.hpp>

int main(int argc, char*argv[]) {
    IR_TYPES_NS::uint256_t hash = "2938472983479283742038746283746283746287346729363475638475682743";

    std::cout << hash << std::endl;

    hash += 1;

    std::cout << hash << std::endl;

    asio::error_code errorCode;
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", errorCode), 80);
    asio::ip::tcp::socket socket(context);
    socket.connect(endpoint, errorCode);

    if (!errorCode) {
        std::cout << "Connected!" << std::endl;
    }
    else {
        std::cout << "Failed to connect to address: " << errorCode.message() << std::endl;
    }

    return 0;
}