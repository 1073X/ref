
#include <cfg/cmd_source.hpp>
#include <cfg/settings.hpp>
#include <filesystem>
#include <iomanip>
#include <log/log.hpp>
#include <meta/info.hpp>

#include "ref/database.hpp"
#include "ref/reg_var_str.hpp"
#include "ref/version.hpp"
#include "source/lib/ref/layout.hpp"

#include "json_builder.hpp"

int32_t main(int32_t argc, const char* argv[]) try {
    miu::cfg::cmd_source source { argc, argv };
    miu::cfg::settings settings { &source };

    if (settings.optional<bool>("version", false)) {
        std::cout << miu::ref::version() << std::endl;

    } else if (settings.optional<bool>("usage", false)) {
        std::cout << miu::ref::version() << std::endl;
        std::cout << miu::ref::build_info() << std::endl;
        std::cout << miu::meta::info() << ": dump ref shmem content to json." << std::endl;
        std::cout << "\nUsage: refview <ref name>" << std::endl;

    } else {
        auto ref_name = settings.required<std::string>(0);

        auto file_name = miu::com::strcat { ref_name, "ref" }.str();
        miu::shm::buffer buf { file_name, miu::shm::mode::READ };

        miu::ref::json_builder builder;

        auto layout = miu::ref::layout::open(buf.data());
        for (auto i = 0U; i < layout->num_of_instrument(); i++) {
            builder.append(layout->instruments() + i);
        }
        for (auto i = 0U; i < layout->num_of_tiktable(); i++) {
            builder.append(layout->tiktables() + i);
        }
        for (auto i = 0U; i < layout->num_of_schedule(); i++) {
            builder.append(layout->schedules() + i);
        }

        std::cout << std::setw(2) << builder.json() << std::endl;
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
