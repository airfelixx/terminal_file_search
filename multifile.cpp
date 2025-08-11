#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <limits>

using str = std::string;
using ifile = std::ifstream;
using ofile = std::ofstream;

namespace fs = std::filesystem;

auto& print = std::cout;
auto& scan = std::cin;

struct FNode {
    str filestr;
    FNode *next;

    FNode(str fstring){
        filestr = fstring;
        next = nullptr;
    }
};

struct FCList{
    FNode* head;
    FNode* tail;
    int empty;
    int size;
};

FCList* initflist(){
    FCList* flist = new FCList;
    flist->head =flist->tail = new FNode("");
    flist->empty = 1;
    flist->size = 0;
    return flist;
}

void addfile (FCList* filelist, str fstring){
    if (filelist->empty){
        filelist->head=filelist->tail= new FNode(fstring);
        filelist->head->next = filelist->tail->next = filelist->head;
        filelist->empty = 0;
        filelist->size ++;
    } else{
        FNode* newfile =new FNode(fstring);
        filelist->tail->next = newfile;
        newfile->next = filelist->head;
        filelist->tail=newfile;
        filelist->size ++;
    }
    return;
}

void printflist(FCList* filelist){
    if (filelist->empty){
        print<<"No Files Found.\n";
        return;
    }
    FNode* current = filelist->head;
    print << "Files Found:\n";
    for (int i = 0; i<filelist->size; i++){
        print <<(i+1)<<". "<< current->filestr<<"\n";
        current = current->next;
    }
    return;
}

void clearLinesAndRefresh(const std::string& line1, const std::string& line2) {
    std::cout << "\033[2A";        // Move cursor up 2 lines
    std::cout << "\r\033[2K" << line1 << "\n";  // Clear line and write first line
    std::cout << "\r\033[2K" << line2 << std::flush;  // Clear line and write second line
}


int main (){
    FCList* flist = initflist();
    str fext = ".txt";

    str dirpath;
    print << "Directory Path From Home: ";
    scan >> dirpath;
    print << "\n";

    str name2search;
    print << "File Name: ";
    scan >> name2search;
    print << "\n";

    try {
        for ( const auto& entry : fs::recursive_directory_iterator(dirpath, fs::directory_options::skip_permission_denied)){

            if (entry.is_regular_file()){
                std::string filename = entry.path().filename().string();
                if (entry.path().extension() == fext && filename.find(name2search)!= std::string::npos){
                    print << "Found file: " << entry.path();
                    print << "\e[2K\r";
                    addfile(flist, entry.path());
                }
            }
        } 
    } catch (const fs::filesystem_error& e){
        std::cerr << "Filesystem error: "<< e.what() << "\n";
    }
    printflist(flist);
    print<<"\n";
    
    FNode* current = flist->head;
    
    print<<"Navigation:\n";
    print<< "Current File: "<< current->filestr<<"\n";
    print<<"<N>ext | <O>pen | <D>elete | <E>xit :";

    while (true) {
        char action;
        scan.ignore(std::numeric_limits<std::streamsize>::max(), '\n')>>action; // discard newline

        switch (action) {
            case 'n':{
                current = current->next;
                break;}
            case 'o': {
                str command = "gedit " + current->filestr + " &";
                std::system(command.c_str());
                break;
            }
            case 'd': {
                str command = "rm " + current->filestr + " &";
                std::system(command.c_str());
                break;
            }
            case 'e':
                print<<"\n";
                return 0;
            default:
                break;
        } 
        clearLinesAndRefresh("Current File: " + current->filestr,
                         "<N>ext | <O>pen | <D>elete | <E>xit : ");
    }
}
