#include <iostream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/sem.h>
#define SEMAPHORE_KEY 54416
#define SEMAPHORE_COUNT 2
#include "logger_builder_concrete.h"
#include <nlohmann/json.hpp>
int semId;
void clear_resurs(){
    std::cout<<"dvv"<<std::endl;
    semctl(semId, 0, IPC_RMID, 0);
}
int main() {
    atexit(clear_resurs);
    int file_descriptor;
    const int file_length = 300;
    const char* path = "/home/dima/CLionProjects/logger_server/bred.txt";

    file_descriptor = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (file_descriptor == -1){
        perror("Problem with opening file");
        exit(1);
    }

    lseek(file_descriptor, file_length - 1, SEEK_SET);
    write(file_descriptor, "", 1);

    char* adr = reinterpret_cast<char*>(mmap(NULL, file_length, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0));
    if (adr == MAP_FAILED){
        perror("Problem with mmap");
        close(file_descriptor);
        exit(1);
    }
    std::cout << "logger_server started" << std::endl;

    semId = semget(SEMAPHORE_KEY, 2, IPC_CREAT | 0666);
    if (semId == -1) {
        perror("semget");
        exit(1);
    }
    close(file_descriptor);

    auto* semOps = new sembuf[2];
    semOps[0].sem_num = 0;
    semOps[0].sem_op = 0;
    semOps[0].sem_flg = 0;
    semOps[1].sem_num = 1;
    semOps[1].sem_op = 1;
    semOps[1].sem_flg = 0;
    semop(semId, semOps, 2);
    int client_count = 0;
    logger_builder* builder = new logger_builder_concrete();
    logger* logg = builder->add_json_configuration("config.json")->construct();

    int count = 0;                              // TODO: FOR DEBUG!
    while (true) {

        semOps[0].sem_op = -1;
        semOps[1].sem_op = 0;
        semop(semId, semOps, 2);
        std::string mess = adr;


        std::stringstream x;
        std::string word;
        std::string str;
        std::string severety;
        std::string command;

        x << mess;
        while (std::getline(x, word)) {
            if (word == "severity::trace" || word == "severity::debug" || word == "severity::information" || word == "severity::warning" || word == "severity::error" || word == "severity::critical"){
                severety = word;
            }else{
                str += word;
                str += '\n';
            }
        }
        std::map <std::string, logger::severity> maps;
        maps={
                {"severity::trace", logger::severity::trace},
                {"severity::debug", logger::severity::debug},
                {"severity::information", logger::severity::information},
                {"severity::warning", logger::severity::warning},
                {"severity::error", logger::severity::error},
                {"severity::critical", logger::severity::critical}
        };
        logg->log(str, maps[severety]);
        sprintf(adr, "%s", "ok");

        semOps[0].sem_op = 1;
        semOps[1].sem_op = 1;
        semop(semId, semOps, 2);
    }
}
