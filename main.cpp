#include <Utils/Debug.h>

#include <Utils/Platform/Platform.h>

#include <Utils/Locale/Encoding.h>
#include <Utils/Input/InputSystem.h>

#include <Core/Tests.h>

#include <Utils/Types/Thread.h>

int main(int argc, char* argv[]) {
    auto&& applicationPath = SR_PLATFORM_NS::GetApplicationPath().GetFolder();

    SR_UTILS_NS::Debug::Instance().Init(applicationPath, true, SR_UTILS_NS::Debug::Theme::Dark);
    SR_HTYPES_NS::Thread::Factory::Instance().SetMainThread();

    SR_UTILS_NS::Locale::SetLocale();

    SR_LOG("Starting application...");

    auto&& tests = IR_TESTS_NS::Tests();
    tests.RunAll();

    SR_LOG("Exiting application...");
    return 0;
}