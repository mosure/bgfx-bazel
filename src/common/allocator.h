#pragma once

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/filepath.h>
#include <bx/string.h>


namespace example {

bx::FileReaderI* getFileReader();
bx::FileWriterI* getFileWriter();
bx::AllocatorI*  getAllocator();

void setCurrentDir(const char* _dir);

void alloc_init();

} // namespace example
