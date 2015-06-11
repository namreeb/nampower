/*
    Copyright (c) 2015, namreeb (legal@namreeb.org)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those
    of the authors and should not be interpreted as representing official policies,
    either expressed or implied, of the FreeBSD Project.
*/

#define NAME    "nampower"
#define VERSION "v1.1"

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
#ifdef _DEBUG
            ("export,e", boost::program_options::value<std::string>(&exportFunc)->default_value("Load"), "export function to call upon injection")
#endif
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