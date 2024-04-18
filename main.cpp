#include <QCoreApplication>
#include <QScopeGuard>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <future>

#include <sentry.h>


void crash_unref_nullptr(uint sleep_sec);


int main(int argc, char *argv[])
{
    sentry_options_t *options = sentry_options_new();
    // DSN can be taken from the project settings in Sentry: Settings -> Projects -> Client Keys (DSN)
    sentry_options_set_dsn(options, "<dsn-goes-here>");
    // This is also the default-path. For further information and recommendations:
    // https://docs.sentry.io/platforms/native/configuration/options/#database-path
    sentry_options_set_database_path(options, ".sentry-native");
    sentry_options_set_handler_path(options, "../../libs/sentry-native/install/bin/crashpad_handler");
    sentry_options_set_release(options, "qgc@13");
    sentry_options_set_debug(options, 1);
    sentry_init(options);

    // Make sure everything flushes
    auto sentryClose = qScopeGuard([] { sentry_close(); });

    static_cast<void>(std::async(crash_unref_nullptr, 3));

    QCoreApplication a(argc, argv);
    return a.exec();
}

void crash_unref_nullptr(uint sleep_sec) {
    std::cerr << "Prepare to crash, sleeping for " << sleep_sec << " second(s)...\n";
    std::this_thread::sleep_for(std::chrono::seconds(sleep_sec));

    sentry_set_tag("Plane", "123");
    sentry_value_t user = sentry_value_new_object();
    sentry_value_set_by_key(user, "id", sentry_value_new_int32(43));
    sentry_value_set_by_key(user, "email", sentry_value_new_string("john.doe@example.com"));
    sentry_set_user(user);

    sentry_capture_event(sentry_value_new_message_event(
    /*   level */ SENTRY_LEVEL_INFO,
    /*  logger */ "custom",
    /* message */ "BEFORE CRASH"
    ));

    sentry_value_t crumb1 = sentry_value_new_breadcrumb("default", "Authenticated user");
    sentry_value_set_by_key(crumb1, "category", sentry_value_new_string("auth"));
    sentry_value_set_by_key(crumb1, "level", sentry_value_new_string("info"));
    sentry_add_breadcrumb(crumb1);
    sentry_value_t crumb2 = sentry_value_new_breadcrumb("ui", "Gamera Turned On");
    sentry_value_set_by_key(crumb2, "category", sentry_value_new_string("cam.1"));
    sentry_value_set_by_key(crumb2, "level", sentry_value_new_string("debug"));
    sentry_add_breadcrumb(crumb2);

    int* ptr = nullptr;
    *ptr = 42; // Dereferencing a null pointer
}

