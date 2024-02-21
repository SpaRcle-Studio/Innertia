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

#include <asio.hpp>

void CommonCycle(const SR_NETWORK_NS::Context::Ptr& pContext) {
    for (uint32_t i = 0; i < 25; ++i) {
        pContext->Poll();
        SR_PLATFORM_NS::Sleep(40);
    }

    /// SR_LOG("Press ESC to continue...");

    /// while (true) {
    ///     pContext->Poll();

    ///     SR_PLATFORM_NS::Sleep(1000);

    ///     SR_UTILS_NS::Input::Instance().Check();

    ///     if (SR_UTILS_NS::Input::Instance().GetKeyDown(SR_UTILS_NS::KeyCode::Escape)) {
    ///        break;
    ///     }
    /// }
}

int32_t TestAcceptor() {
    SR_INFO("Testing acceptor...");

    auto&& serverThread = std::thread([]() {
        auto&& pContext = SR_NETWORK_NS::Context::Create();

        if (!pContext->Run()) {
            SR_ERROR("[Server] Failed to run context!");
            return -1;
        }

        auto&& pAcceptor = pContext->CreateAcceptor(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80);

        pAcceptor->SetCallback([](auto&& pSocket) {
            SR_LOG("[Server] Accepted connection");
            pSocket->Close();
        });

        pAcceptor->StartAsync();

        CommonCycle(pContext);

        pAcceptor->Close();
        pContext->Stop();

        return 0;
    });

    auto&& clientThread = std::thread([]() {
        auto&& pContext = SR_NETWORK_NS::Context::Create();

        if (!pContext->Run()) {
            SR_ERROR("[Client] Failed to run context!");
            return -1;
        }

        for (uint8_t i = 0; i < 4; ++i) {
            auto&& pSocket = pContext->CreateSocket(SR_NETWORK_NS::SocketType::TCP);
            if (!pSocket->Connect("127.0.0.1", 80)) {
                SR_LOG("[Client] Failed to connect to address {} socket!", i);
                return -1;
            }
            else {
                SR_LOG("[Client] Connected {} socket!", i);
            }
            pSocket->Close();
        }
        pContext->Stop();
        return 0;
    });

    serverThread.join();
    clientThread.join();

    SR_INFO("Acceptor test passed!");

    return 0;
}

int32_t TestPeerToPeer(const SR_NETWORK_NS::Context::Ptr& pContext) {
    SR_INFO("Testing peer to peer...");

    std::vector<SR_NETWORK_NS::PeerToPeer::Ptr> peers;

    for (auto i = 0; i < 4; ++i) {
        auto&& pPeer = pContext->CreateP2P(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80 + i + 1);

        pPeer->SetOnAcceptCallback([](auto&& pP2P, auto&& pSocket) {
            SR_LOG("Accepted P2P connection! Server {}, Client {}", pP2P->GetAcceptor()->GetLocalPort(), pSocket->GetLocalPort());
        });

        if (!pPeer->Run()) {
            SR_ERROR("Failed to run peer to peer!");
            return -1;
        }

        if (i != 0) {
            if (!pPeer->Connect("127.0.0.1", 80 + i)) {
                SR_ERROR("Failed to connect to peer!");
                return -1;
            }
        }

        peers.push_back(pPeer);
    }

    CommonCycle(pContext);

    for (auto&& pPeer : peers) {
        pPeer->Close();
    }

    SR_INFO("Peer to peer test passed!");

    return 0;
}

void TestResolve(const std::string& name) {
    asio::io_service io_service;
    asio::ip::tcp::resolver resolver(io_service);
    asio::ip::tcp::resolver::query query(name, "");
    for(asio::ip::tcp::resolver::iterator i = resolver.resolve(query);
        i != asio::ip::tcp::resolver::iterator();
        ++i)
    {
        asio::ip::tcp::endpoint end = *i;

        SR_LOG("Resolved '{}': address: '{}'", name, end.address().to_string());
    }
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    auto&& applicationPath = SR_PLATFORM_NS::GetApplicationPath().GetFolder();
    SR_UTILS_NS::Debug::Instance().Init(applicationPath, true, SR_UTILS_NS::Debug::Theme::Dark);

    SR_UTILS_NS::Locale::SetLocale();

    SR_LOG("Starting application...");

    auto&& pContext = SR_NETWORK_NS::Context::Create();

    if (!pContext->Run()) {
        SR_ERROR("Failed to run context!");
        return -1;
    }

    //if (TestPeerToPeer(pContext) != 0) {
    //    SR_ERROR("Failed to test peer to peer!");
    //}

    if (TestAcceptor() != 0) {
        SR_ERROR("Failed to test acceptor!");
    }

    /// TestResolve("www.google.com");

    SR_LOG("Exiting application...");

    pContext->Stop();
    return 0;
}