#pragma once

#include <Arduino.h>
#include <string.h>

void myFileSystem_Init();

String myFileSystem_info();

bool write_to_FileSystem(char* filename, char* data);

bool format_FileSystem();

bool check_if_file_exists_in_FileSystem(char* filename);

String read_file_in_FileSystem(char* filename);

uint32_t return_free_HEAP();