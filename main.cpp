#include <Utils/Debug.h>
#include <Utils/Platform/Platform.h>
#include <Utils/Network/Socket.h>
#include <Utils/Network/Server.h>
#include <Utils/Network/PeerToPeer.h>
#include <Utils/Network/Client.h>
#include <Utils/Network/Acceptor.h>
#include <Utils/Locale/Encoding.h>
#include <Utils/Input/InputSystem.h>

#include <Core/Block.h>

void CommonCycle(const SR_NETWORK_NS::Context::Ptr& pContext) {
    pContext->Pool();

    SR_PLATFORM_NS::Sleep(1000);

    /// SR_LOG("Press ESC to continue...");

    /// while (true) {
    ///     pContext->Pool();

    ///     SR_PLATFORM_NS::Sleep(1000);

    ///     SR_UTILS_NS::Input::Instance().Check();

    ///     if (SR_UTILS_NS::Input::Instance().GetKeyDown(SR_UTILS_NS::KeyCode::Escape)) {
    ///        break;
    ///     }
    /// }
}

int32_t TestAcceptor(const SR_NETWORK_NS::Context::Ptr& pContext) {
    SR_INFO("Testing acceptor...");

    auto&& pAcceptor = pContext->CreateAcceptor(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80);

    pAcceptor->StartAsync([](auto&& pSocket) {
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

    CommonCycle(pContext);

    SR_INFO("Acceptor test passed!");

    return 0;
}

int32_t TestPeerToPeer(const SR_NETWORK_NS::Context::Ptr& pContext) {
    SR_INFO("Testing peer to peer...");

    std::vector<SR_NETWORK_NS::PeerToPeer::Ptr> peers;

    for (auto i = 0; i < 2; ++i) {
        auto&& pPeer = pContext->CreateP2P(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80);

        pPeer->SetOnAcceptCallback([](auto&& pP2P, auto&& pSocket) {
            SR_LOG("Accepted P2P connection {}", (void*)pP2P.Get());
        });

        if (!pPeer->Run()) {
            SR_ERROR("Failed to run peer to peer!");
            return -1;
        }

        peers.push_back(pPeer);
    }

    CommonCycle(pContext);

    SR_INFO("Peer to peer test passed!");

    return 0;
}

int main(int argc, char* argv[]) {
    auto&& applicationPath = SR_PLATFORM_NS::GetApplicationPath().GetFolder();
    SR_UTILS_NS::Debug::Instance().Init(applicationPath, true, SR_UTILS_NS::Debug::Theme::Dark);

    SR_UTILS_NS::Locale::SetLocale();

    SR_LOG("Starting application...");

    auto&& pContext = SR_NETWORK_NS::Context::Create();

    // if (TestPeerToPeer(pContext) != 0) {
    //     SR_ERROR("Failed to test peer to peer!");
    // }

    if (TestAcceptor(pContext) != 0) {
        SR_ERROR("Failed to test acceptor!");
    }

    SR_LOG("Exiting application...");

    return 0;
}