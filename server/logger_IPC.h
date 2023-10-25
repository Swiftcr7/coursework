

#ifndef SERVER_LOGGER_IPC_H
#define SERVER_LOGGER_IPC_H
#define SEMAPHORE_K 54416
#define SEMAPHORE_COUNT 2
#include "logger.h"
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <map>
#include <sys/sem.h>

class logger_ipc final : public logger{
private:
    char* adr;
    int semid;

public:
    logger_ipc(const char* way){
        int fd = open(way, O_CREAT | O_RDWR);
        if (fd == -1){
            perror("Problem with opening file");
            exit(1);
        }


        adr = (char*)(mmap(NULL, fd, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if (adr == MAP_FAILED){
            perror("Problem with mmap");
            close(fd);
            exit(1);
        }
        close(fd);
        semid = semget(SEMAPHORE_K, SEMAPHORE_COUNT,  0666);
        if (semid == -1) {
            perror("semget");
            exit(1);
        }
//        auto* semOps = new sembuf[2];
//        semOps[0].sem_num = 0;
//        semOps[0].sem_op = 0;
//        semOps[0].sem_flg = 0;
//        semOps[1].sem_num = 1;
//        semOps[1].sem_op = -1;
//        semOps[1].sem_flg = 0;
//        semop(semid, semOps, 2);
//        sprintf(adr, "%s", "new client");
//        semOps[0].sem_op = 1;
//        semOps[1].sem_op = 0;
//        semop(semid, semOps, 2);
//
//        semOps[0].sem_op = -1;
//        semOps[1].sem_op = -1;
//        semop(semid, semOps, 2);
//        std::string answer = adr;
//        std::cout<<answer<<std::endl;
//        semOps[0].sem_op = 0;
//        semOps[1].sem_op = 1;
//        semop(semid, semOps, 2);
//        delete[] semOps;

    }

    logger const* log(const std::string& target, logger::severity severity) const override{
        auto* semOps = new sembuf[2];
        semOps[0].sem_num = 0;
        semOps[0].sem_op = 0;
        semOps[0].sem_flg = 0;
        semOps[1].sem_num = 1;
        semOps[1].sem_op = -1;
        semOps[1].sem_flg = 0;
        semop(semid, semOps, 2);
        std::map <logger::severity, std::string> maps;
        maps={
                {logger::severity::trace, "severity::trace"},
                {logger::severity::warning, "severity::warning"},
                {logger::severity::information,"severity::information"},
                {logger::severity::debug, "severity::debug"},
                {logger::severity::error, "severity::error"},
                {logger::severity::critical, "severity::critical"}
        };
        std::string message;
        message = target;
        message += '\n';
        message += maps[severity];
        sprintf(adr, "%s", message.c_str());
        semOps[0].sem_op = 1;
        semOps[1].sem_op = 0;
        semop(semid, semOps, 2);

        semOps[0].sem_op = -1;
        semOps[1].sem_op = -1;
        semop(semid, semOps, 2);
        std::string answer = adr;
        semOps[0].sem_op = 0;
        semOps[1].sem_op = 1;
        semop(semid, semOps, 2);
        delete[] semOps;
        return this;
    }

    ~logger_ipc() override{
//        auto* semOps = new sembuf[2];
//        semOps[0].sem_num = 0;
//        semOps[0].sem_op = 0;
//        semOps[0].sem_flg = 0;
//        semOps[1].sem_num = 1;
//        semOps[1].sem_op = -1;
//        semOps[1].sem_flg = 0;
//        semop(semid, semOps, 2);
//        sprintf(adr, "%s", "Exit");
//        semOps[0].sem_op = 1;
//        semOps[1].sem_op = 0;
//        semop(semid, semOps, 2);
//
//        semOps[0].sem_op = -1;
//        semOps[1].sem_op = -1;
//        semop(semid, semOps, 2);
//        std::string answer = adr;
//        std::cout<<answer<<std::endl;
//        semOps[0].sem_op = 0;
//        semOps[1].sem_op = 1;
    }
};
#endif //SERVER_LOGGER_IPC_H
