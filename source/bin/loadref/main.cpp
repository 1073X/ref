
#include <cfg/cmd_source.hpp>
#include <cfg/settings.hpp>
#include <filesystem>
#include <log/log.hpp>
#include <meta/info.hpp>

#include "ref/reg_var_str.hpp"
#include "ref/version.hpp"
#include "source/lib/loadref/loader.hpp"

int32_t main(int32_t argc, const char* argv[]) try {
    miu::ref::reg_var_str();
    miu::log::reset(miu::log::severity::DEBUG, 2048);

    miu::cfg::cmd_source source { argc, argv };
    miu::cfg::settings settings { &source };

    if (settings.optional<bool>("version", false)) {
        std::cout << miu::ref::version() << std::endl;

    } else if (settings.optional<bool>("usage", false)) {
        std::cout << miu::ref::version() << std::endl;
        std::cout << miu::ref::build_info() << std::endl;
        std::cout << miu::meta::info() << ": fill ref shmem with json file." << std::endl;
        std::cout << "\nUsage: loadref <path/to/file.json>" << std::endl;

    } else {
        auto json_file = settings.required<std::string>(0);
        if (!std::filesystem::exists(json_file)) {
            FATAL_ERROR<std::invalid_argument>("cann't find json [", json_file, "]");
        }

        miu::ref::loader::load(json_file);

        miu::log::dump();
    }

    return 0;
} catch (std::exception const& err) {
    miu::log::dump();
    std::cerr << err.what() << std::endl;
    return -1;
} catch (...) {
    miu::log::dump();
    std::cerr << "unknown error" << std::endl;
    return -1;
}
