// nampower injector

#define NAME    "nampower"
#define VERSION "v1.0"

#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>

#include <hadesmem/process_list.hpp>
#include <hadesmem/injector.hpp>
#include <hadesmem/process.hpp>

int main(int argc, char *argv[])
{
    try
    {
        std::cout << NAME << " " << VERSION << " injector" << std::endl;

        std::wstring dll, program;
        std::string exportFunc = "Load";
        bool enableConsole;

        boost::program_options::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "display help message")
            ("console,c", "enable wow console")
            ("dll,d", boost::program_options::wvalue<std::wstring>(&dll)->default_value(L"nampower.dll", "nampower.dll"), "dll to inject into program")
            ("program,p", boost::program_options::wvalue<std::wstring>(&program)->default_value(L"wow.exe", "wow.exe"), "program name");

        boost::program_options::variables_map vm;

        try
        {
            boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
            boost::program_options::notify(vm);

            if (vm.count("help"))
            {
                std::cout << desc << std::endl;
                return EXIT_SUCCESS;
            }

            enableConsole = !!(vm.count("console"));
        }
        catch (boost::program_options::error const &e)
        {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            std::cerr << desc << std::endl;
            return EXIT_FAILURE;
        }

        std::vector<std::wstring> createArgs;

        if (enableConsole)
            createArgs.push_back(L"-console");

        const hadesmem::CreateAndInjectData injectData =
            hadesmem::CreateAndInject(program, L"", std::begin(createArgs), std::end(createArgs), dll, exportFunc, hadesmem::InjectFlags::kPathResolution);

        std::cout << "Injected.  Process ID: " << injectData.GetProcess().GetId() << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cerr << std::endl << "Error: " << std::endl;
        std::cerr << boost::diagnostic_information(e) << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}