#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <limits>

// Define convenient type aliases
using str = std::string;
using ifile = std::ifstream;
using ofile = std::ofstream;

namespace fs = std::filesystem;

// Aliases for standard input/output streams
auto& print = std::cout;
auto& scan = std::cin;

// Node struct representing a file in the circular linked list
struct FNode {
    str filestr;  // File path as a string
    FNode *next;  // Pointer to next node

    // Constructor initializes the file path and sets next pointer to nullptr
    FNode(str fstring){
        filestr = fstring;
        next = nullptr;
    }
};

// Circular linked list struct holding pointers to head and tail, and metadata
struct FCList{
    FNode* head;  // Pointer to first node
    FNode* tail;  // Pointer to last node
    int empty;    // Flag indicating if list is empty (1 = empty)
    int size;     // Number of nodes in the list
};

// Initialize a new circular linked list with a dummy head node
FCList* initflist(){
    FCList* flist = new FCList;
    flist->head =flist->tail = new FNode(""); // dummy node with empty string
    flist->empty = 1;   // list starts empty
    flist->size = 0;    // size zero initially
    return flist;
}

// Add a file path to the circular linked list
void addfile (FCList* filelist, str fstring){
    if (filelist->empty){
        // First real node being added: point head and tail to it, circularly linked
        filelist->head=filelist->tail= new FNode(fstring);
        filelist->head->next = filelist->tail->next = filelist->head;
        filelist->empty = 0;
        filelist->size ++;
    } else{
        // Insert new node after tail and link it circularly back to head
        FNode* newfile =new FNode(fstring);
        filelist->tail->next = newfile;
        newfile->next = filelist->head;
        filelist->tail=newfile;
        filelist->size ++;
    }
    return;
}

// Print all file paths stored in the list, or indicate if empty
void printflist(FCList* filelist){
    if (filelist->empty){
        print<<"No Files Found.\n";
        return;
    }
    FNode* current = filelist->head;
    print << "Files Found:\n";
    // Loop exactly size times to avoid infinite traversal
    for (int i = 0; i<filelist->size; i++){
        print <<(i+1)<<". "<< current->filestr<<"\n";
        current = current->next;
    }
    return;
}

// Clear previous two lines in terminal and print refreshed lines
void clearLinesAndRefresh(const std::string& line1, const std::string& line2) {
    std::cout << "\033[2A";        // Move cursor up 2 lines
    std::cout << "\r\033[2K" << line1 << "\n";  // Clear current line and write line1
    std::cout << "\r\033[2K" << line2 << std::flush;  // Clear current line and write line2, flush output
}

int main (){
    // Initialize the circular file list and set file extension to search for
    FCList* flist = initflist();
    str fext = ".txt";

    // Ask user for directory path (relative to home)
    str dirpath;
    print << "Directory Path From Home: ";
    scan >> dirpath;
    print << "\n";

    // Ask user for keyword to search within filenames
    str name2search;
    print << "File Name: ";
    scan >> name2search;
    print << "\n";

    try {
        // Recursively iterate through directory entries, skipping permission denied errors
        for ( const auto& entry : fs::recursive_directory_iterator(dirpath, fs::directory_options::skip_permission_denied)){

            if (entry.is_regular_file()){
                std::string filename = entry.path().filename().string();
                // Check extension and if filename contains keyword
                if (entry.path().extension() == fext && filename.find(name2search)!= std::string::npos){
                    print << "Found file: " << entry.path();
                    print << "\e[2K\r";  // Clear line after printing found file
                    addfile(flist, entry.path()); // Add file path to list
                }
            }
        } 
    } catch (const fs::filesystem_error& e){
        // Handle filesystem errors (e.g. permissions)
        std::cerr << "Filesystem error: "<< e.what() << "\n";
    }

    // Print all found files to user
    printflist(flist);
    print<<"\n";
    
    // Start navigation at head of the list
    FNode* current = flist->head;
    
    // Show navigation commands and current file
    print<<"Navigation:\n";
    print<< "Current File: "<< current->filestr<<"\n";
    print<<"<N>ext | <O>pen | <D>elete | <E>xit :";

    while (true) {
        char action;
        // Ignore leftover input until newline, then read single char action
        scan.ignore(std::numeric_limits<std::streamsize>::max(), '\n')>>action;

        switch (action) {
            case 'n':{
                // Move to next file in circular list
                current = current->next;
                break;}
            case 'o': {
                // Open current file using gedit (in background)
                str command = "gedit " + current->filestr + " &";
                std::system(command.c_str());
                break;
            }
            case 'd': {
                // Delete current file using rm (in background)
                str command = "rm " + current->filestr + " &";
                std::system(command.c_str());
                break;
            }
            case 'e':
                // Exit program
                print<<"\n";
                return 0;
            default:
                break;
        } 
        // Refresh displayed navigation lines with updated current file
        clearLinesAndRefresh("Current File: " + current->filestr,
                         "<N>ext | <O>pen | <D>elete | <E>xit : ");
    }
}
