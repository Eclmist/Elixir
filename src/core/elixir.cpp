/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019 Samuel Van Allen - All rights reserved.

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

void PrintUsage(const exrChar* msg = nullptr)
{
    if (msg)
        fprintf(stderr, "elixir: %s\n\n", msg);

    using namespace std;
    cout << "Elixir Version " << EXR_VERSION_MAJOR << "." << EXR_VERSION_MINOR << "." << EXR_VERSION_PATCH << EXR_VERSION_PRERELEASEID;
    cout << ", Copyright (c) 2019 Samuel Van Allen" << endl << endl;
    cout << "Usage: elixir [options] <One or more scene files>" << endl << endl;
    cout << "Rendering Options: " << endl;
    cout << "   -h, --help              Display this help page" << endl;
    cout << "   -t, --numthreads        Specify the number of rendering threads to use" << endl;
    cout << "   -o, --out <fname>       Write the output image to a specified filename" << endl;
    cout << "   -q, --quick             Reduce output quality for quick render" << endl;
    cout << "   -d, --debug             Render debug scene defined in code. To be deprecated." << endl;
    cout << "Logging Options: " << endl;
    cout << "   --quiet                 Suppress all non-error messages" << endl;
    cout << "For documentations, please refer to <http://docs.elixir.moe/>" << endl;
}

int main(int argc, exrChar *argv[])
{
    ElixirOptions options;
    std::vector<exrString> filenames;

    if (argc <= 1)
        return -1;

    // Process command-line arguments
    for (exrS32 i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--numthreads") || !strcmp(argv[i], "-t"))
            options.numThreads = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--out") || !strcmp(argv[i], "-o"))
            options.outputFile = argv[++i];
        else if (!strcmp(argv[i], "--quick") || !strcmp(argv[i], "-q"))
            options.quickRender = true;
        else if (!strcmp(argv[i], "--quiet"))
            options.quiet = true;
        else if (!strcmp(argv[i], "--debug") || !strcmp(argv[i], "-d"))
            options.debug = true;
        else if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
            PrintUsage();
        else 
            filenames.push_back(argv[i]);
    }

    ElixirInit(options);

    // Process scene description
    if (filenames.size() == 0 || options.debug)
    {
        ElixirParseFile("-");
    }
    else
    {
        for (const exrString& f : filenames)
        {
            ElixirParseFile(f);
        }
    }

    ElixirRender();
    ElixirCleanup();
   
#ifdef EXR_PLATFORM_WIN
    system("PAUSE");
#endif

    return 0;
}
