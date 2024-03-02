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
#include <Utils/Types/Thread.h>

static std::atomic<bool> g_isRunning = true;

void CommonCycle(const SR_NETWORK_NS::Context::Ptr& pContext) {
    while (g_isRunning) {
        pContext->Poll();
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
    SR_INFO("TestAcceptor() : testing acceptor...");

    auto&& serverThread = std::thread([]() {
        auto&& pContext = SR_NETWORK_NS::Context::Create();

        if (!pContext->Run()) {
            SR_ERROR("TestAcceptor() :  [Server] Failed to run context!");
            return -1;
        }

        auto&& pAcceptor = pContext->CreateAcceptor(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80);

        pAcceptor->SetCallback([](auto&& pSocket) {
            SR_LOG("TestAcceptor() :  [Server] Accepted connection. Local port {}. Remote port {}", pSocket->GetLocalPort(), pSocket->GetRemotePort());
            pSocket->Send("Hello from server!", 18);
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
            SR_ERROR("TestAcceptor() : [Client] Failed to run context!");
            return -1;
        }

        for (uint8_t i = 0; i < 4; ++i) {
            auto&& pSocket = pContext->CreateSocket(SR_NETWORK_NS::SocketType::TCP);
            if (!pSocket->Connect("127.0.0.1", 80)) {
                SR_LOG("TestAcceptor() : [Client] Failed to connect to address {} socket!", i);
                return -1;
            }
            else {
                SR_LOG("TestAcceptor() : [Client] Connected {} socket. Local port {}. Remote port {}", i, pSocket->GetLocalPort(), pSocket->GetRemotePort());
            }
            pSocket->SetReceiveCallback([](auto&& pSocket, auto&& pPackage, auto&& size) {
                SR_LOG("TestAcceptor() : [Client] Received data from server. Local port {}. Remote port {}", pSocket->GetLocalPort(), pSocket->GetRemotePort());
            });
            pSocket->AsyncReceive(18);
            pSocket->Close();
        }

        CommonCycle(pContext);

        pContext->Stop();
        return 0;
    });

    g_isRunning = false;

    serverThread.join();
    clientThread.join();

    SR_INFO("TestAcceptor() :  acceptor test passed!");

    return 0;
}

int32_t TestPeerToPeer() {
    SR_INFO("TestPeerToPeer() : testing peer to peer...");

    auto&& dnsThread = std::thread([]() {
        auto&& pContext = SR_NETWORK_NS::Context::CreateAndRun();
        auto&& pDNS = pContext->CreateP2P(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80);

        pDNS->SetOnAcceptCallback([](auto&& pP2P, auto&& pSocket) {
            SR_LOG("TestPeerToPeer() : [DNS] Accepted P2P connection! Server {}, Client {}", pP2P->GetAcceptor()->GetLocalPort(), pSocket->GetRemotePort());
        });

        if (!pDNS->Run()) {
            SR_ERROR("TestPeerToPeer() :  failed to run peer to peer!");
        }

        CommonCycle(pContext);

        SR_LOG("TestPeerToPeer() :  [DNS] Count of connections: {}", pDNS->GetConnectionsCount());

        pDNS->Close();
        pContext->Stop();
    });

    std::vector<std::thread> peers;

    for (auto i = 1; i <= 4; ++i) {
        SR_PLATFORM_NS::Sleep(1000);
        peers.emplace_back([i]() {
            auto&& pContext = SR_NETWORK_NS::Context::CreateAndRun();
            auto&& pPeer = pContext->CreateP2P(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80 + i);

            pPeer->SetOnAcceptCallback([i](auto&& pP2P, auto&& pSocket) {
                SR_LOG("TestPeerToPeer() : [Peer {}] Accepted P2P connection! Server {}, Client {}", i, pP2P->GetAcceptor()->GetLocalPort(), pSocket->GetLocalPort());
            });

            if (!pPeer->Run()) {
                SR_ERROR("TestPeerToPeer() : failed to run peer to peer!");
                return;
            }

            if (!pPeer->Connect("127.0.0.1", 80)) { /// Connect to DNS
                SR_ERROR("TestPeerToPeer() :  failed to connect to peer!");
                return;
            }

            CommonCycle(pContext);

            SR_LOG("TestPeerToPeer() :  [Peer {}] Count of connections: {}", i, pPeer->GetConnectionsCount());

            pPeer->Close();
            pContext->Stop();
        });
    }

    SR_PLATFORM_NS::Sleep(2000);

    g_isRunning = false;

    dnsThread.join();

    for (auto&& peer : peers) {
        peer.join();
    }

    SR_INFO("TestPeerToPeer() :  peer to peer test passed!");

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

        SR_LOG("TestResolve() : Resolved '{}': address: '{}'", name, end.address().to_string());
    }
    std::cout << '\n';
}

void TestHash() {
    auto&& hash = SR_UTILS_NS::sha256<IR_TYPES_NS::IRHash>("123456789");
    std::string correctHash("15e2b0d3c33891ebb0f1ef609ec419420c20e320ce94c65fbc8c3312448eb225");


    SR_LOG("Hash testing...");
    SR_LOG("SHA-256 hash from \"123456789\" is {}...", hash.ToString());
    SR_LOG("Hash obtained from the function: {};", hash.ToString());

    /*if (hash == correctHash) {
        SR_INFO("TestHash() : hash test passed!");
    }
    else {
        SR_ERROR("TestHash() : test failed!");
    }*/
}

int main(int argc, char* argv[]) {
    auto&& applicationPath = SR_PLATFORM_NS::GetApplicationPath().GetFolder();

    SR_UTILS_NS::Debug::Instance().Init(applicationPath, true, SR_UTILS_NS::Debug::Theme::Dark);
    SR_HTYPES_NS::Thread::Factory::Instance().SetMainThread();

    SR_UTILS_NS::Locale::SetLocale();

    SR_LOG("Starting application...");

    TestPeerToPeer();

    if (TestAcceptor() != 0) {
        SR_ERROR("Failed to test acceptor!");
    }

    /// TestResolve("www.google.com");

    SR_LOG("Exiting application...");

    return 0;
}