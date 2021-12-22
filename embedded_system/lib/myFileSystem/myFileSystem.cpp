#include <Arduino.h>
#include <string.h>
#include <LittleFS.h>

// custom libs
#include "myFileSystem.h"

// local variables

bool file_system_status = false;
// filesystem modes: “r”, “w”, “a”, “r+”, “w+”, “a+”

void myFileSystem_Init()
{
    file_system_status = LittleFS.begin();
}

String myFileSystem_info(){
    // Fills FSInfo structure with information about the file system. Returns true is successful, false otherwise.
    FSInfo fs_info;
    LittleFS.info(fs_info);
    
    return "FS total bytes: " + String(fs_info.totalBytes / 8) + ", FS used bytes: " + String(fs_info.usedBytes / 8);
}

bool write_to_FileSystem(char* path, char* data){
    File cur_file = LittleFS.open(path, "w");
    if(!cur_file) return false;
    cur_file.print(data);
    cur_file.close();
    return true;
}

bool format_FileSystem(){
    return LittleFS.format(); // returns true if done
}

bool check_if_file_exists_in_FileSystem(char* path){
    return LittleFS.exists(path); // true if it exists
}

String read_file_in_FileSystem(char* path)
{
    if (check_if_file_exists_in_FileSystem(path)){
        File cur_file = LittleFS.open(path, "r");
    if (!cur_file)
        return "Error";
    String output = "";
    // Data from file
    while(cur_file.available()){
    output += cur_file.read();
  }
    cur_file.close(); // Close file
    return output;
    } else{
        return "Does Not Exist";
    }
}

//return free heap, unsigned int
uint32_t return_free_HEAP(){
    return ESP.getFreeHeap();
}
