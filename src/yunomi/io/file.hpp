#pragma once

#include <cstdio>
#include <unistd.h>
#include <string>
#include <sys/stat.h>

namespace yunomi {
    namespace io {
        class File {
        public:
            static void remove(std::string const &path) {
                int result = unlink(path.c_str());
                if(result != 0){
                    throw std::runtime_error("unlink failed");
                }
            }

            static bool exists(std::string const &path){
                struct stat buffer;
                int result = stat(path.c_str(), &buffer);
                return result == 0;
            }
        };
    }
}