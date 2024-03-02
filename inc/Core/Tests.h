//
// Created by innerviewer on 2024-03-02.
//
#ifndef INNERTIA_TESTS_H
#define INNERTIA_TESTS_H

namespace IR_TESTS_NS {
    class Tests {
    public:
        Tests();
    public:
        void RunAll() const;

    public:
        static bool TestAcceptor();
        static bool TestPeerToPeer();
        static void TestResolve(const std::string& name);
        static bool TestHash();

    private:
        std::vector<std::function<bool()>> m_tests;
    };
}

#endif //INNERTIA_TESTS_H
