//
// Created by innerviewer on 2024-03-02.
//

#include <Core/Tests.h>

#include <Utils/macros.h>
#include <Utils/stdInclude.h>

#include <Utils/Network/Socket.h>
#include <Utils/Network/PeerToPeer.h>
#include <Utils/Network/Acceptor.h>

#include <Utils/Platform/Platform.h>

#include <asio.hpp>

namespace IR_TESTS_NS {
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

    bool Tests::TestAcceptor() {
        SR_INFO("TestAcceptor() : testing acceptor...");

        bool result = true;

        auto&& serverThread = std::thread([&result]() {
            auto&& pContext = SR_NETWORK_NS::Context::Create();

            if (!pContext->Run()) {
                SR_ERROR("TestAcceptor() :  [Server] Failed to run context!");
                result = false;
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

        auto&& clientThread = std::thread([&result]() {
            auto&& pContext = SR_NETWORK_NS::Context::Create();

            if (!pContext->Run()) {
                SR_ERROR("TestAcceptor() : [Client] Failed to run context!");
                result = false;
                return -1;
            }

            for (uint8_t i = 0; i < 4; ++i) {
                auto&& pSocket = pContext->CreateSocket(SR_NETWORK_NS::SocketType::TCP);
                if (!pSocket->Connect("127.0.0.1", 80)) {
                    SR_LOG("TestAcceptor() : [Client] Failed to connect to address {} socket!", i);
                    result = false;
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
        return result;
    }

    bool Tests::TestPeerToPeer() {
        SR_INFO("TestPeerToPeer() : testing peer to peer...");

        bool result = true;

        auto&& dnsThread = std::thread([&result]() {
            auto&& pContext = SR_NETWORK_NS::Context::CreateAndRun();
            auto&& pDNS = pContext->CreateP2P(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80);

            pDNS->SetOnAcceptCallback([](auto&& pP2P, auto&& pSocket) {
                SR_LOG("TestPeerToPeer() : [DNS] Accepted P2P connection! Server {}, Client {}", pP2P->GetAcceptor()->GetLocalPort(), pSocket->GetRemotePort());
            });

            if (!pDNS->Run()) {
                SR_ERROR("TestPeerToPeer() :  failed to run peer to peer!");
                result = false;
            }

            CommonCycle(pContext);

            SR_LOG("TestPeerToPeer() :  [DNS] Count of connections: {}", pDNS->GetConnectionsCount());

            pDNS->Close();
            pContext->Stop();
        });

        std::vector<std::thread> peers;

        for (auto i = 1; i <= 4; ++i) {
            SR_PLATFORM_NS::Sleep(1000);
            peers.emplace_back([i, &result]() {
                auto&& pContext = SR_NETWORK_NS::Context::CreateAndRun();
                auto&& pPeer = pContext->CreateP2P(SR_NETWORK_NS::SocketType::TCP, "127.0.0.1", 80 + i);

                pPeer->SetOnAcceptCallback([i, &result](auto&& pP2P, auto&& pSocket) {
                    SR_LOG("TestPeerToPeer() : [Peer {}] Accepted P2P connection! Server {}, Client {}", i, pP2P->GetAcceptor()->GetLocalPort(), pSocket->GetLocalPort());
                });

                if (!pPeer->Run()) {
                    SR_ERROR("TestPeerToPeer() : failed to run peer to peer!");
                    result = false;
                    return;
                }

                if (!pPeer->Connect("127.0.0.1", 80)) { /// Connect to DNS
                    SR_ERROR("TestPeerToPeer() :  failed to connect to peer!");
                    result = false;
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
        return result;
    }

    void Tests::TestResolve(const std::string& name) {
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

    bool Tests::TestHash() {
        auto&& hash = SR_UTILS_NS::sha256<SR_TYPES_NS::HashT<32>>("123456789");
        std::string correctHash("15e2b0d3c33891ebb0f1ef609ec419420c20e320ce94c65fbc8c3312448eb225");


        SR_LOG("TestHash() : testing SHA256 implementation...");
        SR_LOG("TestHash() : SHA-256 hash from \"123456789\" is {}...", hash.to_string());

        if (hash.to_string() != correctHash) {
            SR_ERROR("TestHash() : SHA-256 hash is incorrect!");
            return false;
        }

        SR_LOG("TestHash() : calculated SHA-256 hash is correct!");
        return true;
    }

    Tests::Tests() {
        m_tests.emplace_back(TestAcceptor);
        m_tests.emplace_back(TestPeerToPeer);
        m_tests.emplace_back(TestHash);
    }

    void Tests::RunAll() const {
        std::string log;
        auto&& successes = 0;

        for (auto&& test : m_tests) {
            if (test()) {
                ++successes;
            }
        }

        IR_TESTS_NS::Tests::TestResolve("www.google.com");

        if (successes != m_tests.size()) {
            SR_ERROR("Tests::RunAll() : tests failed: {}/{}.\n", successes, m_tests.size());
            return;
        }

        SR_LOG("Tests::RunAll() : all tests passed successfully: {}/{}!\n", successes, m_tests.size());
    }
}