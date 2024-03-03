#include <Utils/Debug.h>
#include <Utils/Platform/Platform.h>
#include <Utils/Types/Thread.h>
#include <Utils/Locale/Encoding.h>

#include <Core/Tests.h>

int main(int argc, char* argv[]) {
    auto&& applicationPath = SR_PLATFORM_NS::GetApplicationPath().GetFolder();

    SR_UTILS_NS::Debug::Instance().Init(applicationPath, true, SR_UTILS_NS::Debug::Theme::Dark);
    SR_HTYPES_NS::Thread::Factory::Instance().SetMainThread();

    SR_UTILS_NS::Locale::SetLocale();

    SR_LOG("Starting application...");

    //auto&& tests = IR_TESTS_NS::Tests();
    //tests.RunAll();

    IR_TESTS_NS::Tests::TestHash();

    IR_TESTS_NS::Tests::TestResolve("www.google.com");
    //IR_TESTS_NS::Tests::TestPinger();

    //SR_PLATFORM_NS::Sleep(100000);

    SR_LOG("Exiting application...");
    return 0;
}