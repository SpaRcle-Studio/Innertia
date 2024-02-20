#include <Utils/Debug.h>
#include <Utils/Platform/Platform.h>
#include <Utils/Network/Socket.h>
#include <Utils/Network/Server.h>
#include <Utils/Network/Client.h>
#include <Utils/Network/Acceptor.h>
#include <Utils/Locale/Encoding.h>
#include <Utils/Input/InputSystem.h>

#include <Core/Block.h>

int main(int argc, char* argv[]) {
    auto&& applicationPath = SR_PLATFORM_NS::GetApplicationPath().GetFolder();
    SR_UTILS_NS::Debug::Instance().Init(applicationPath, true, SR_UTILS_NS::Debug::Theme::Dark);

    SR_UTILS_NS::Locale::SetLocale();

    SR_LOG("Starting application...");

    auto&& pContext = SR_NETWORK_NS::Context::Create();
    auto&& pAcceptor = pContext->CreateAcceptor(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80);

    pAcceptor->Start([](auto&& pSocket) {
        SR_LOG("Accepted connection");
    });

    auto&& pSocket = pContext->CreateSocket(SR_NETWORK_NS::SocketType::TCP);
    if (!pSocket->Connect("127.0.0.1", 80)) {
        SR_LOG("Failed to connect to address");
        return -1;
    }
    else {
        SR_LOG("Connected!");
    }

    while (true) {
        pContext->Pool();

        SR_UTILS_NS::Input::Instance().Check();

        if (SR_UTILS_NS::Input::Instance().GetKeyDown(SR_UTILS_NS::KeyCode::Escape)) {
            break;
        }
    }

    SR_LOG("Exiting application...");

    return 0;
}