#include <iostream>
#include <cstdio>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "/home/dima/CLionProjects/server/logger_IPC.h"
#include <sys/sem.h>
#include <fstream>

#define SEMAPHORE_KEY 54431
#define SEMAPHORE_COUNT 2
using namespace std;

void dialogue(){
    logger* _logger = new logger_ipc("/home/dima/CLionProjects/logger_server/bred.txt");
    if (_logger != nullptr) _logger->log("Client has started working", logger::severity::warning);
    std::cout<<"------------------------------------------------------------------------"<<std::endl;
    std::cout<<"-------------------------The dialogue has begun-------------------------"<<std::endl;
    if (_logger != nullptr){
        _logger->log("------------------------------------------------------------------------", logger::severity::warning);
        _logger->log("-------------------------The dialogue has begun-------------------------", logger::severity::warning);
    }
    std::string choice = " ";
    int file_descriptor;
    const int file_length = 400;
    const char* path = "/home/dima/CLionProjects/client_2/nothing.txt";

    file_descriptor = open(path, O_CREAT | O_RDWR);
    if (file_descriptor == -1){
        perror("Problem with opening file");
        exit(1);
    }


    char* adr = (char*)(mmap(NULL, file_length, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0));
    if (adr == MAP_FAILED){
        perror("Problem with mmap");
        close(file_descriptor);
        exit(1);
    }
    close(file_descriptor);
    int semId = semget(SEMAPHORE_KEY, SEMAPHORE_COUNT,  0666);
    if (semId == -1) {
        perror("semget");
        exit(1);
    }
    if (_logger != nullptr) _logger->log("Client waiting for connection...", logger::severity::warning);
    auto* semOps = new sembuf[2];
    std::string answer;
//    semOps[0].sem_num = 0;
//    semOps[0].sem_op = 0;
//    semOps[0].sem_flg = 0;
//    semOps[1].sem_num = 1;
//    semOps[1].sem_op = -1;
//    semOps[1].sem_flg = 0;
//    semop(semId, semOps, 2);
//    sprintf(adr, "%s", "new client");
//    semOps[0].sem_op = 1;
//    semOps[1].sem_op = 0;
//    semop(semId, semOps, 2);
//
//    semOps[0].sem_op = -1;
//    semOps[1].sem_op = -1;
//    semop(semId, semOps, 2);
//    std::string answer = adr;
//    std::cout<<answer<<std::endl;
//    if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
//    semOps[0].sem_op = 0;
//    semOps[1].sem_op = 1;
    //semop(semId, semOps, 2);
    while (choice != "4"){
        semOps[0].sem_num = 0;
        semOps[0].sem_op = 0;
        semOps[0].sem_flg = 0;
        semOps[1].sem_num = 1;
        semOps[1].sem_op = -1;
        semOps[1].sem_flg = 0;
        semop(semId, semOps, 2);
        if (_logger != nullptr) _logger->log("Client waiting for connection...", logger::severity::warning);
        if (_logger != nullptr){
            _logger->log("------------------------------Select action-----------------------------\n1) Enter a command manually 2) Run commands in a file 3) Reset database 4) Exit database 5) Restore database\n(You need to write only a number, without brackets and words)\nInput: ", logger::severity::warning);
        }
        std::cout<<"------------------------------Select action-----------------------------\n1) Enter a command manually 2) Run commands in a file 3) Reset database 4) Exit database 5) Restore database\n(You need to write only a number, without brackets and words)\nInput:"<<std::endl;
        std::cin.clear();
        std::getline(std::cin, choice);
        if (choice == "1"){

            std::string command;
            if (_logger != nullptr) _logger->log("\n1) add pool\n2) delete pool\n3) add scheme\n4) delete scheme\n5) add collection\n6) delete collection\n7) insert\n8) remove\n9) get key information\n10) read range\n11) update key\n12) saving state\n(You need to write only a number, without brackets and words)\nCommand:", logger::severity::warning);

            std::cout<<"\n1) add pool\n2) delete pool\n3) add scheme\n4) delete scheme\n5) add collection\n6) delete collection\n7) insert\n8) remove\n9) get key information\n10) read range\n11) update key\n12) saving state\n(You need to write only a number, without brackets and words)\nCommand:"<<std::endl;
            std::cin.clear();
            std::getline(std::cin, command);
            if (command == "1"|| command == "2" || command == "3" || command == "4" || command == "5" || command == "6" || command == "7" || command == "8" || command == "9" || command == "10" || command == "11" || command == "12"){
                if (command == "1"){
                    if (_logger != nullptr) _logger->log("Enter the name of pool and command exactly as shown below\nAdd pool: (pool name) [{name  of allocator} {size for allocation, if you need it} {allocator mode, if you need it}]\nEnter the name of pool and the command\nCommand: ", logger::severity::warning);

                    std::cout<<"Enter the name of pool and command exactly as shown below\nAdd pool: (pool name) [{name  of allocator} {size for allocation, if you need it} {allocator mode, if you need it}]\nEnter the name of pool and the command\nCommand: "<<std::endl;

                }else if (command == "2"){
                    if (_logger != nullptr) _logger->log("Enter the name of pool and command exactly as shown below\nDelete pool: (pool name)\nEnter the name of pool and the command\nCommand: ", logger::severity::warning);

                    std::cout<<"Enter the name of pool and command exactly as shown below\nDelete pool: (pool name)\nEnter the name of pool and the command\nCommand: "<<std::endl;

                }else if (command == "3"){
                    if (_logger != nullptr) _logger->log("Enter the name of pool, name of scheme and command exactly as shown below\nAdd scheme: (pool name) (scheme name)\nEnter the name of pool, name of scheme and command\nCommand: ", logger::severity::warning);

                    std::cout<<"Enter the name of pool, name of scheme and command exactly as shown below\nAdd scheme: (pool name) (scheme name)\nEnter the name of pool, name of scheme and command\nCommand: "<<std::endl;
                }else if (command == "4"){
                    if (_logger != nullptr) _logger->log("Enter the name of pool, name of scheme and command exactly as shown below\nDelete scheme: (pool name) (scheme name)\nEnter the name of pool, name of scheme and command\nCommand: ", logger::severity::warning);

                    std::cout<<"Enter the name of pool, name of scheme and command exactly as shown below\nDelete scheme: (pool name) (scheme name)\nEnter the name of pool, name of scheme and command\nCommand: "<<std::endl;

                }else if (command == "5"){
                    if (_logger != nullptr) _logger->log("Enter the name of pool, name of scheme, name of collection, the name of the tree in which you want to store the data and command exactly as shown below\nAdd collection: (pool name) (scheme name) (collection name) [tree name]\nEnter the name of pool, name of scheme, name of collection and command\nCommand: ", logger::severity::warning);

                    std::cout<<"Enter the name of pool, name of scheme, name of collection, the name of the tree in which you want to store the data and command exactly as shown below\nAdd collection: (pool name) (scheme name) (collection name) [tree name]\nEnter the name of pool, name of scheme, name of collection and command\nCommand: "<<std::endl;

                }else if (command == "6"){
                    if (_logger != nullptr) _logger->log("Enter the name of pool, name of scheme, name of collection and command exactly as shown below\nDelete collection: (pool name) (scheme name) (collection name)\nEnter the name of pool, name of scheme, name of collection and command\nCommand: ", logger::severity::warning);

                    std::cout<<"Enter the name of pool, name of scheme, name of collection and command exactly as shown below\nDelete collection: (pool name) (scheme name) (collection name)\nEnter the name of pool, name of scheme, name of collection and command\nCommand: "<<std::endl;

                }else if (command == "7"){
                    if (_logger != nullptr) _logger->log("Enter the location map, key and value exactly as shown below\nInsert: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nValue: (name student) (surname student) (patronymic student) (exam_date) (start date) (estimation) (name teacher) (surname teacher) (patronymic teacher)\nEnter the command and map to the node\nCommand and map: ", logger::severity::warning);

                    std::cout<<"Enter the location map, key and value exactly as shown below\nInsert: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nValue: (name student) (surname student) (patronymic student) (exam_date) (start date) (estimation) (name teacher) (surname teacher) (patronymic teacher)\nEnter the command and map to the node\nCommand and map: "<<std::endl;

                }else if (command == "8"){
                    if (_logger != nullptr) _logger->log("Enter the location map, key and value exactly as shown below\nRemove: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nEnter the command and map to the node\nCommand and map: ", logger::severity::warning);

                    std::cout<<"Enter the location map, key and value exactly as shown below\nRemove: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nEnter the command and map to the node\nCommand and map: "<<std::endl;
                }else if (command == "9"){
                    if (_logger != nullptr) _logger->log("Enter the location map and key exactly as shown below\nGet key information: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nEnter the command and map to the node\nCommand and map: ", logger::severity::warning);

                    std::cout<<"nter the location map and key exactly as shown below\nGet key information: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nEnter the command and map to the node\nCommand and map: "<<std::endl;

                }else if (command == "10"){
                    if (_logger != nullptr) _logger->log("Enter the location map and keys exactly as shown below\nRead range: (pool name) (scheme name) (collection name)\n[Key{beginning}: (id_session) (id_student) (name subject) (reporting_format)]\n[Key{ending}: (id_session) (id_student) (name subject) (reporting_format)]\nIf you want the output from the very beginning or to the very end, leave [] empty in the appropriate places. Words {beginning} and {ending} do not need to be entered", logger::severity::warning);

                    std::cout<<"Enter the location map and keys exactly as shown below\nRead range: (pool name) (scheme name) (collection name)\n[Key{beginning}: (id_session) (id_student) (name subject) (reporting_format)]\n[Key{ending}: (id_session) (id_student) (name subject) (reporting_format)]\nIf you want the output from the very beginning or to the very end, leave [] empty in the appropriate places. Words {beginning} and {ending} do not need to be entered"<<std::endl;

                }else if (command == "11"){
                    if (_logger != nullptr) _logger->log("Enter the location map, key and value exactly as shown below\nUpdate key: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nValue: (name student) (surname student) (patronymic student) (exam_date) (start date) (estimation) (name teacher) (surname teacher) (patronymic teacher)\nEnter the command and map to the node\nCommand and map: ", logger::severity::warning);

                    std::cout<<"Enter the location map, key and value exactly as shown below\nUpdate key: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nValue: (name student) (surname student) (patronymic student) (exam_date) (start date) (estimation) (name teacher) (surname teacher) (patronymic teacher)\nEnter the command and map to the node\nCommand and map: "<<std::endl;

                }else if (command == "12"){
                    if (_logger != nullptr) _logger->log("Transfer the file and the current state of the database will be written to this file", logger::severity::warning);
                    std::cout<<"Transfer the file and the current state of the database will be written to this file"<<std::endl;
                }

                if (command == "12") {
                    if (_logger != nullptr) _logger->log("Enter the path to the file", logger::severity::warning);
                    std::cout<<"Enter the path to the file"<<std::endl;
                    std::cin.clear();
                    std::string way;
                    std::getline(std::cin, way);
                    if (way.empty()) {
                        if (_logger != nullptr) _logger->log("The path is empty", logger::severity::warning);
                        std::cout<<"The path is empty"<<std::endl;
                        continue;
                    }
                    try {
                        std::string message = "Saving state: ";
                        message += '\n';
                        message += way;
                        semOps[0].sem_op = 1;
                        semOps[1].sem_op = 0;
                        sprintf(adr, "%s", message.c_str());
                        semop(semId, semOps, 2);

                        semOps[0].sem_op = -1;
                        semOps[1].sem_op = -1;
                        semop(semId, semOps, 2);
                        answer = adr;
                        std::cout<<answer<<std::endl;
                        if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
                        semOps[0].sem_op = 0;
                        semOps[1].sem_op = 1;
                        semop(semId, semOps, 2);
                        continue;
                    } catch (const std::logic_error &er) {
                        if (_logger != nullptr) _logger->log(er.what(), logger::severity::warning);
                        semOps[0].sem_op = 0;
                        semOps[1].sem_op = 1;
                        semop(semId, semOps, 2);
                        continue;
                    }
                }
                std::string command_str;
                std::string key_str;
                std::string value_str;
                std::cin.clear();
                std::getline(std::cin, command_str);
                if (command == "7" || command == "11" || command == "10"){
                    if (_logger != nullptr) _logger->log("Enter the key\nkey: ", logger::severity::warning);
                    std::cout<<"Enter the key\nkey: "<<std::endl;
                    std::cin.clear();
                    std::getline(std::cin, key_str);
                    if (command == "10"){
                        if (_logger != nullptr) _logger->log("Enter the key\nKey: ", logger::severity::warning);
                        std::cout<<"Enter the key\nKey: "<<std::endl;
                    }else{
                        if (_logger != nullptr) _logger->log("Enter the value\nvalue: ", logger::severity::warning);
                        std::cout<<"Enter the value\nvalue: "<<std::endl;
                    }
                    std::cin.clear();
                    std::getline(std::cin, value_str);
                }else if (command == "8" || command == "9") {
                    if (_logger != nullptr) _logger->log("Enter the key\nkey: ", logger::severity::warning);
                    std::cout<<"Enter the key\nkey: "<<std::endl;
                    std::cin.clear();
                    std::getline(std::cin, key_str);
                    value_str = "";


                }else{
                    key_str = "";
                    value_str = "";
                }
                try{
                    std::string message;
                    message += command_str;
                    message += '\n';
                    message += key_str;
                    message += '\n';
                    message += value_str;
                    sprintf(adr, "%s", message.c_str());
                    semOps[0].sem_op = 1;
                    semOps[1].sem_op = 0;
                    semop(semId, semOps, 2);

                    semOps[0].sem_op = -1;
                    semOps[1].sem_op = -1;
                    semop(semId, semOps, 2);
                    answer = adr;
                    std::cout<<answer<<std::endl;
                    if (_logger != nullptr) _logger->log("client taking result", logger::severity::warning);
                    semOps[0].sem_op = 0;
                    semOps[1].sem_op = 1;
                    semop(semId, semOps, 2);
                    //parsing_enter(command_str, key_str, value_str);
                    if (_logger != nullptr) _logger->log("The command has been successfully executed. You can enter the data again", logger::severity::warning);
                    std::cout<<"The command has been successfully executed. You can enter the data again"<<std::endl;

                    continue;
                }catch (const std::logic_error& er){
                    std::string message = "An error occurred during the execution of the program\n";
                    message += er.what();
                    if (_logger != nullptr) _logger->log(message, logger::severity::warning);
                    std::cout<<message<<std::endl;
                    if (_logger != nullptr) _logger->log("You can enter the data again", logger::severity::warning);
                    std::cout<<"You can enter the data again"<<std::endl;
                    semOps[0].sem_op = 0;
                    semOps[1].sem_op = 1;
                    semop(semId, semOps, 2);
                    continue;
                }
            }else{
                if (_logger != nullptr) _logger->log("Incorrect input, check the data and try again", logger::severity::warning);
                std::cout<<"Incorrect input, check the data and try again"<<std::endl;
                semOps[0].sem_op = 0;
                semOps[1].sem_op = 1;
                semop(semId, semOps, 2);
                continue;
            }
        }else if (choice == "2") {
            if (_logger != nullptr) _logger->log("Enter the path to the file", logger::severity::warning);
            std::cout<<"Enter the path to the file"<<std::endl;
            std::cin.clear();
            std::string way;
            std::getline(std::cin, way);
            if (way.empty()) {
                if (_logger != nullptr) _logger->log("The path is empty", logger::severity::warning);
                std::cout<<"The path is empty"<<std::endl;
                continue;
            }
            try {
                if (_logger != nullptr) _logger->log("------------The parsing in file has started working-----------", logger::severity::warning);
                std::cout<<"------------The parsing in file has started working-----------"<<std::endl;
                std::ifstream file_user;
                file_user.open(way);
                if (!file_user.is_open()){
                    throw std::logic_error("Incorrect path to file");
                }
                std::string command_line;
                std::string key_line;
                std::string value_line;
                while (std::getline(file_user, command_line)){
                    if (command_line.find("Insert: ") == 0 || command_line.find("Update key: ") == 0 || command_line.find("Read range: ") == 0){
                        if (!std::getline(file_user, key_line)){
                            file_user.close();
                            throw std::logic_error("Incorrect key");
                        }
                        if (!std::getline(file_user, value_line)){
                            file_user.close();
                            throw std::logic_error("Incorrect value");
                        }
                    }else if (command_line.find("Update key: ") == 0 || command_line.find("Get key information: ") == 0  || command_line.find("Remove: ") == 0){
                        if (!std::getline(file_user, key_line)){
                            file_user.close();
                            throw std::logic_error("Incorrect key");
                        }
                    }else if (command_line.find("Add pool: ") == 0 || command_line.find("Delete pool: ") == 0 || command_line.find("Add scheme: ") == 0 || command_line.find("Delete scheme: ") == 0 || command_line.find("Add collection: ") == 0 || command_line.find("Delete collection: ") == 0) {
                        try {
                            std::string message;
                            message += command_line;
                            message += '\n';
                            message += key_line;
                            message += '\n';
                            message += value_line;
                            sprintf(adr, "%s", message.c_str());
                            semOps[0].sem_op = 1;
                            semOps[1].sem_op = 0;
                            semop(semId, semOps, 2);

                            semOps[0].sem_op = -1;
                            semOps[1].sem_op = -1;
                            semop(semId, semOps, 2);
                            answer = adr;
                            std::cout<<answer<<std::endl;
                            semOps[0].sem_op = 0;
                            semOps[1].sem_op = 1;
                            semop(semId, semOps, 2);
                            semOps[0].sem_op = 0;
                            semOps[1].sem_op = -1;
                            semop(semId, semOps, 2);
                            if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);

                        } catch (const std::logic_error &er) {
                            file_user.close();
                            throw er;
                        }

                        continue;
                    }else if (command_line.find("Saving state:") == 0){
                        if (!std::getline(file_user, key_line)) {
                            file_user.close();
                            throw std::logic_error("Incorrect key");
                        }
                        if (key_line.empty()) {
                            file_user.close();
                            throw std::logic_error("file is empty");
                        }
                        try {
                            std::string message = "Saving state: ";
                            message += '\n';
                            message += way;
                            semOps[0].sem_op = 1;
                            semOps[1].sem_op = 0;
                            sprintf(adr, "%s", message.c_str());
                            semop(semId, semOps, 2);

                            semOps[0].sem_op = -1;
                            semOps[1].sem_op = -1;
                            semop(semId, semOps, 2);
                            answer = adr;
                            std::cout<<answer<<std::endl;
                            if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
                            continue;
                        } catch (const std::logic_error &er) {
                            file_user.close();
                            throw er;
                        }
                    }else{
                        file_user.close();
                        throw std::logic_error("Incorrect command");
                    }
                    try{
                        std::string message;
                        message += command_line;
                        message += '\n';
                        message += key_line;
                        message += '\n';
                        message += value_line;
                        semOps[0].sem_op = 1;
                        semOps[1].sem_op = 0;
                        semop(semId, semOps, 2);
                        sprintf(adr, "%s", message.c_str());
                        semOps[0].sem_op = -1;
                        semOps[1].sem_op = -1;
                        semop(semId, semOps, 2);
                        answer = adr;

                        std::cout<<answer<<std::endl;
                        semOps[0].sem_op = 0;
                        semOps[1].sem_op = 1;
                        semop(semId, semOps, 2);
                        semOps[0].sem_op = 0;
                        semOps[1].sem_op = -1;
                        semop(semId, semOps, 2);
                        if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);

                    }catch (const std::logic_error& er){
                        file_user.close();
                        throw er;
                    }

                }

                file_user.close();
                if (_logger != nullptr) _logger->log("------------The parsing in file has finished working-----------", logger::severity::warning);
                std::cout<<"------------The parsing in file has finished working-----------"<<std::endl;
                //parsing_in_file(way);
            } catch (const std::logic_error &er) {
                std::string flag;
                if (_logger != nullptr) _logger->log(er.what(), logger::severity::warning);
                if (_logger != nullptr)
                    _logger->log(
                            "Error in parsing file!\nSelect action\n1) Continue 2) Reset and continues 3) Exit database\n(You need to write only a number, without brackets and words)\nInput:",
                            logger::severity::warning);
                std::cout<<"Error in parsing file!\nSelect action\n1) Continue 2) Reset and continues 3) Exit database\n(You need to write only a number, without brackets and words)\nInput:"<<std::endl;
                std::cin.clear();
                std::getline(std::cin, flag);
                if (flag == "1") {
                    semOps[0].sem_op = 0;
                    semOps[1].sem_op = 1;
                    semop(semId, semOps, 2);
                    continue;
                } else if (flag == "2") {
                    if (_logger != nullptr) _logger->log("Reset database", logger::severity::warning);
                    std::cout<<"Reset database"<<std::endl;
                    try {
                        //reset_database();
                        semOps[0].sem_op = 1;
                        semOps[1].sem_op = 0;
                        semop(semId, semOps, 2);
                        sprintf(adr, "%s", "Reset database:");
                        semOps[0].sem_op = -1;
                        semOps[1].sem_op = -1;
                        semop(semId, semOps, 2);
                        answer = adr;
                        std::cout<<answer<<std::endl;
                        if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
                        semOps[0].sem_op = 0;
                        semOps[1].sem_op = 1;
                        semop(semId, semOps, 2);
                    } catch (const std::logic_error &er) {
                        if (_logger != nullptr) _logger->log(er.what(), logger::severity::warning);
                    }

                    continue;
                } else if (flag == "3") {
                    if (_logger != nullptr)
                        _logger->log("------------------------Exiting----------------------------",
                                     logger::severity::warning);
                    std::cout<<"------------------------Exiting----------------------------"<<std::endl;
//                    sprintf(adr, "%s", "Exit");
//                    semOps[0].sem_op = 1;
//                    semOps[1].sem_op = 0;
//                    semop(semId, semOps, 2);
//
//                    semOps[0].sem_op = -1;
//                    semOps[1].sem_op = -1;
//                    semop(semId, semOps, 2);
//                    answer = adr;
//                    std::cout<<answer<<std::endl;
//                    if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
//                    semOps[0].sem_op = 0;
//                    semOps[1].sem_op = 1;
//                    semop(semId, semOps, 2);
                    semOps[0].sem_op = 0;
                    semOps[1].sem_op = 1;
                    semop(semId, semOps, 2);
                    break;
                } else {
                    semOps[0].sem_op = 0;
                    semOps[1].sem_op = 1;
                    semop(semId, semOps, 2);
                    if (_logger != nullptr) _logger->log("Error: incorrect flag", logger::severity::warning);
                    std::cout<<"Error: incorrect flag"<<std::endl;
                    continue;
                }
            }
            std::string flag;
            if (_logger != nullptr)
                _logger->log(
                        "Parsing file finished successfully!\nSelect action\n1) Continue 2)  Exit database\n(You need to write only a number, without brackets and words)\nInput:",
                        logger::severity::warning);
            std::cout<<"Parsing file finished successfully!\nSelect action\n1) Continue 2)  Exit database\n(You need to write only a number, without brackets and words)\nInput:"<<std::endl;
            std::cin.clear();
            std::getline(std::cin, flag);
            if (flag == "1") {
                semOps[0].sem_op = 0;
                semOps[1].sem_op = 1;
                semop(semId, semOps, 2);
                continue;
            } else if (flag == "2") {
                if (_logger != nullptr)
                    _logger->log("------------------------Exiting----------------------------",
                                 logger::severity::warning);
                std::cout<<"------------------------Exiting----------------------------"<<std::endl;
//                semOps[0].sem_op = 1;
//                semOps[1].sem_op = 0;
//                semop(semId, semOps, 2);
//                sprintf(adr, "%s", "Exit");
//                semOps[0].sem_op = -1;
//                semOps[1].sem_op = -1;
//                semop(semId, semOps, 2);
//                answer = adr;
//                std::cout<<answer<<std::endl;
//                if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
//                semOps[0].sem_op = 0;
//                semOps[1].sem_op = 1;
//                semop(semId, semOps, 2);
                semOps[0].sem_op = 0;
                semOps[1].sem_op = 1;
                semop(semId, semOps, 2);
                break;
            } else {
                if (_logger != nullptr) _logger->log("Error: incorrect flag", logger::severity::warning);
                std::cout<<"Error: incorrect flag"<<std::endl;
                semOps[0].sem_op = 0;
                semOps[1].sem_op = 1;
                semop(semId, semOps, 2);
                continue;
            }
        } else if (choice == "5"){
            if (_logger != nullptr) _logger->log("Enter the path to the file", logger::severity::warning);
            std::cout<<"Enter the path to the file"<<std::endl;
            std::cin.clear();
            std::string way;
            std::getline(std::cin, way);
            if (way.empty()) {
                if (_logger != nullptr) _logger->log("The path is empty", logger::severity::warning);
                std::cout<<"The path is empty"<<std::endl;
                continue;
            }
            std::string message = "Restore database: ";
            message += '\n';
            message += way;
            semOps[0].sem_op = 1;
            semOps[1].sem_op = 0;
            semop(semId, semOps, 2);
            sprintf(adr, "%s", message.c_str());
            semOps[0].sem_op = -1;
            semOps[1].sem_op = -1;
            semop(semId, semOps, 2);
            answer = adr;
            std::cout<<answer<<std::endl;
            if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
            semOps[0].sem_op = 0;
            semOps[1].sem_op = 1;
            semop(semId, semOps, 2);
        }else if (choice == "3"){
            try{
                semOps[0].sem_op = 1;
                semOps[1].sem_op = 0;
                semop(semId, semOps, 2);
                sprintf(adr, "%s", "Reset database:");
                semOps[0].sem_op = -1;
                semOps[1].sem_op = -1;
                semop(semId, semOps, 2);
                answer = adr;
                std::cout<<answer<<std::endl;
                if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
                semOps[0].sem_op = 0;
                semOps[1].sem_op = 1;
                semop(semId, semOps, 2);
            }catch (const std::logic_error& er){
                if (_logger != nullptr) _logger->log(er.what(), logger::severity::warning);
            }
            if (_logger != nullptr) _logger->log("The command has been successfully executed. You can enter the data again", logger::severity::warning);
            std::cout<<"The command has been successfully executed. You can enter the data again"<<std::endl;
            continue;
        }else if (choice == "4"){
//            semOps[0].sem_op = 1;
//            semOps[1].sem_op = 0;
//            semop(semId, semOps, 2);
//            sprintf(adr, "%s", "Exit");
//            semOps[0].sem_op = -1;
//            semOps[1].sem_op = -1;
//            semop(semId, semOps, 2);
//            answer = adr;
//            std::cout<<answer<<std::endl;
//            if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
//            semOps[0].sem_op = 0;
//            semOps[1].sem_op = 1;
//            semop(semId, semOps, 2);
            if (_logger != nullptr) _logger->log("-------------------Exiting--------------------", logger::severity::warning);
            std::cout<<"-------------------Exiting--------------------"<<std::endl;
            semOps[0].sem_op = 0;
            semOps[1].sem_op = 1;
            semop(semId, semOps, 2);

            break;
        }else{
            if (_logger != nullptr) _logger->log("--------------------Incorrect input.Exiting-------------------", logger::severity::warning);
            std::cout<<"--------------------Incorrect input.Exiting-------------------"<<std::endl;
//            semOps[0].sem_op = 1;
//            semOps[1].sem_op = 0;
//            semop(semId, semOps, 2);
//            sprintf(adr, "%s", "Exit");
//            semOps[0].sem_op = -1;
//            semOps[1].sem_op = -1;
//            semop(semId, semOps, 2);
//            answer = adr;
//            std::cout<<answer<<std::endl;
//            if (_logger != nullptr) _logger->log("Client taking result", logger::severity::warning);
//            semOps[0].sem_op = 0;
//            semOps[1].sem_op = 1;
//            semop(semId, semOps, 2);

            semOps[0].sem_op = 0;
            semOps[1].sem_op = 1;
            semop(semId, semOps, 2);
            break;
        }
    }
    if (_logger != nullptr) _logger->log("--------------------The dialogue has over --------------------", logger::severity::warning);
    if (_logger != nullptr) _logger->log("--------------------------------------------------------------", logger::severity::warning);
    std::cout<<"--------------------The dialogue has over --------------------"<<std::endl;
    std::cout<<"--------------------------------------------------------------"<<std::endl;
}



//void file_mapping(){
//    int file_descriptor;
//    const int file_length = 100;
//    const char* path = "/home/dima/CLionProjects/client_2/nothing.txt";
//
//    file_descriptor = open(path, O_CREAT | O_RDWR);
//    if (file_descriptor == -1){
//        perror("Problem with opening file");
//        exit(1);
//    }
//
//
//    char* adr = (char*)(mmap(NULL, file_length, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0));
//    if (adr == MAP_FAILED){
//        perror("Problem with mmap");
//        close(file_descriptor);
//        exit(1);
//    }
//    close(file_descriptor);
//
//    std::cout << "client started" << endl;
//    int semId = semget(SEMAPHORE_KEY, SEMAPHORE_COUNT,  0666);
//    if (semId == -1) {
//        perror("semget");
//        exit(1);
//    }
//    std::chrono::milliseconds timing(5000);         // TODO: FOR DEBUG!
//
//    std::cout << "waiting for connection..." << endl;
//    auto* semOps = new sembuf[2];
//    semOps[0].sem_num = 0;
//    semOps[0].sem_op = 0;
//    semOps[0].sem_flg = 0;
//    semOps[1].sem_num = 1;
//    semOps[1].sem_op = -1;
//    semOps[1].sem_flg = 0;
//    semop(semId, semOps, 2);
//    std::cout << "waiting for connection..." << endl;
//    std:: string mess ;
//    std:: string key ;
//    std:: string value;
//    std::getline(std::cin, mess);
//    std::cin.clear();
//    std::getline(std::cin, key);
//    std::cin.clear();
//    std::getline(std::cin, value);
//    std::cin.clear();
//    mess += '\n';
//    mess += key;
//    mess += '\n';
//    mess += value;
//
//    sprintf(adr, "%s", mess.c_str());
//    std::cout << "client dumping data" << endl;
//    std::this_thread::sleep_for(timing);            // TODO: FOR DEBUG!
//
//    semOps[0].sem_op = 1;
//    semOps[1].sem_op = 0;
//    semop(semId, semOps, 2);
//    semOps[0].sem_op = -1;
//    semOps[1].sem_op = -1;
//    semop(semId, semOps, 2);
//    std::string answer = adr;
//    std::cout<<answer<<std::endl;
//
//    std::cout << "client taking result" << endl;
//    std::this_thread::sleep_for(timing);            // TODO: FOR DEBUG!
//
//    semOps[0].sem_op = 0;
//    semOps[1].sem_op = 1;
//    semop(semId, semOps, 2);
//
//    std::cout << "ok" << endl;
//
//    delete[] semOps;
//}




int main() {

    dialogue();

}
