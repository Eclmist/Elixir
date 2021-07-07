/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

    Elixir is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core/elixir.h"
#include "api/api.h"

using namespace elixir;

static ElixirOptions g_RuntimeOptions;

void PrintTitle()
{
    using namespace std;
    cout << "Elixir Version " << EXR_VERSION_MAJOR << "." << EXR_VERSION_MINOR << "." << EXR_VERSION_PATCH << EXR_VERSION_PRERELEASEID;
    cout << ", Copyright (c) 2019-2020 Samuel Van Allen" << endl;
}

void PrintUsage(const exrChar* msg = nullptr)
{
    if (msg)
        fprintf(stderr, "elixir: %s\n\n", msg);

    using namespace std;
    cout << "Usage: elixir [options] <One or more scene files>" << endl << endl;
    cout << "Rendering Options: " << endl;
    cout << "   -h, --help              Display this help page" << endl;
    cout << "   -t, --numthreads        Specify the number of rendering threads to use" << endl;
    cout << "   -o, --out <fname>       Write the output image to a specified filename" << endl;
    cout << "   -s, --stamp             Stamp output filename with metadata" << endl;
    cout << "   -q, --quick             Reduce output quality for quick render" << endl;
    cout << "   -d, --debug             Render debug scene defined in code. To be deprecated." << endl;
    cout << "Logging Options: " << endl;
    cout << "   --quiet                 Suppress all non-error messages" << endl;
    cout << "For documentations, please refer to <http://docs.elixir.moe/>" << endl;

    #ifdef EXR_PLATFORM_WIN
        system("PAUSE");
    #endif
}

int main(int argc, exrChar *argv[])
{
    ElixirOptions options;
    std::vector<exrString> filenames;

    PrintTitle();

    // Process command-line arguments
    for (exrS32 i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
        {
            PrintUsage();
            return -1;
        }
        else if (!strcmp(argv[i], "--numthreads") || !strcmp(argv[i], "-t"))
            options.numThreads = exrMax(options.numThreads, exrU32(atoi(argv[++i])));
        else if (!strcmp(argv[i], "--out") || !strcmp(argv[i], "-o"))
            options.outputFile = argv[++i];
        else if (!strcmp(argv[i], "--stamp") || !strcmp(argv[i], "-s"))
            options.stampFile = true;
        else if (!strcmp(argv[i], "--quick") || !strcmp(argv[i], "-q"))
            options.quickRender = true;
        else if (!strcmp(argv[i], "--quiet"))
            options.quiet = true;
        else if (!strcmp(argv[i], "--debug") || !strcmp(argv[i], "-d"))
            options.debug = true;
        else 
            filenames.push_back(argv[i]);
    }

    ElixirInit(options);

    // Process scene description
    if (filenames.size() == 0)
    {
        if (options.debug)
            ElixirParseFile("-");
        else
        {
            PrintUsage();
            return -1;
        }
    }
    else
    {
        for (const exrString& f : filenames)
        {
            ElixirParseFile(f);
            break; // Only render the first file. This is a temp solution until the command
                   // args have been designed to make sense for multiple files (filename, etc.)
        }
    }

    exrInfoLine("Running Elixir with " << options.numThreads << " thread(s)");

    ElixirRender();
    ElixirCleanup();

#ifdef EXR_PLATFORM_WIN
    system("Pause");
#endif
   
    return 0;
}
