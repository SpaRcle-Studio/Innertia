#include <Utils/Debug.h>
#include <Utils/Platform/Platform.h>
#include <Utils/Network/Socket.h>
#include <Utils/Locale/Encoding.h>

#include <Core/Block.h>

int main(int argc, char* argv[]) {
    auto&& applicationPath = SR_PLATFORM_NS::GetApplicationPath().GetFolder();
    SR_UTILS_NS::Debug::Instance().Init(applicationPath, true, SR_UTILS_NS::Debug::Theme::Dark);

    SR_UTILS_NS::Locale::SetLocale();

    SR_LOG("Starting application...");

    auto&& pSocket = SR_NETWORK_NS::Socket::Create(SR_NETWORK_NS::SocketType::TCP);
    if (!pSocket->Connect("127.0.0.1", 80)) {
        SR_LOG("Failed to connect to address");
        return -1;
    }
    else {
        SR_LOG("Connected!");
    }

    /*IR_TYPES_NS::uint256_t hash = "2938472983479283742038746283746283746287346729363475638475682743";

    std::cout << hash << std::endl;

    hash += 1;

    std::cout << hash << std::endl;
    */

    return 0;
}