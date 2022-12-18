// Copyright (c) 2022 Sultim Tsyrendashiev
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "FolderObserver.h"

namespace fs = std::filesystem;

namespace RTGL1
{
namespace
{
    constexpr auto CHECK_FREQUENCY = std::chrono::milliseconds( 100 );

    auto GetAllFolderFiles( const fs::path& folder )
    {
        std::set< FolderObserver::DependentFile > result;

        for( const fs::directory_entry& entry : fs::recursive_directory_iterator( folder ) )
        {
            result.insert( FolderObserver::DependentFile{
                .path          = entry.path(),
                .lastWriteTime = entry.last_write_time(),
            } );
        }

        return result;
    }
}
}

RTGL1::FolderObserver::FolderObserver( fs::path _folder ) : folder( std::move( _folder ) ) {}

void RTGL1::FolderObserver::RecheckFiles()
{
    if( Clock::now() - lastCheck < CHECK_FREQUENCY )
    {
        return;
    }

    std::set< DependentFile > curAllFiles = GetAllFolderFiles( folder );

    for( const auto& cur : curAllFiles )
    {
        bool foundInPrev = false;

        for( const auto& prev : prevAllFiles )
        {
            // if file previously existed
            if( cur.path == prev.path )
            {
                // if was changed
                if( cur.lastWriteTime != prev.lastWriteTime )
                {
                    CallSubsbribers(
                        &IFileDependency::OnFileChanged, MakeFileType( cur.path ), cur.path );
                }

                foundInPrev = true;
                break;
            }
        }

        // if new file
        if( !foundInPrev )
        {
            CallSubsbribers(
                &IFileDependency::OnFileChanged, MakeFileType( cur.path ), cur.path );
        }
    }

    prevAllFiles = std::move( curAllFiles );
    lastCheck    = Clock::now();
}
