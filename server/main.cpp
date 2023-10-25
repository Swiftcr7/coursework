//#include <iostream>
//#include <string>
//#include <stdio.h>
//#include <sys/types.h>
//#include <stdlib.h>
//#include <chrono>
//#include <thread>
//
//#include <unistd.h>
//#include <string.h>
//#include <sys/time.h>
//#include <fcntl.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <fstream>
//#include <iostream>
//#include <sys/types.h>
//#include <unistd.h>
//
//#define SHARED_MEMORY_KEY 12345
//#define SEMAPHORE_KEY 54321
//
//#define SHARED_MEMORY_SIZE 4096
//#define SEMAPHORE_COUNT 1
//
//#define SERVER_KEY_PATHNAME "/tmp/mqueue_server_key"
//#define PROJECT_ID 'M'
//
//using namespace std;
//
//#ifdef __linux__
//
//
//#include <sys/ipc.h>
//#include <sys/shm.h>
//#include <sys/sem.h>
//#include <sys/msg.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <sys/uio.h>
//#include <sys/wait.h>
//#include <sys/mman.h>
//#include <sys/sem.h>
//
//void file_mapping(){
//    std::cout << "server server" << endl;
//    int semId = semget(SEMAPHORE_KEY, SEMAPHORE_COUNT, 0666);
//    if (semId == -1) {
//        perror("semget");
//        exit(1);
//    }
//
//    std::cout << "server unblocking client, waiting" << endl;
//    struct sembuf semOps[1] = { 0, 1, 0 }; // Операция ожидания
//    semop(semId, semOps, 1);
//
//    std::chrono::milliseconds timespan(1000);
//    std::this_thread::sleep_for(timespan);
//
//    semOps[0].sem_op = -1; // Операция увеличения
//    semop(semId, semOps, 1);
//
//    std::cout << "server reading data, writing result" << endl;
//    semOps[0].sem_op = 1;
//    semop(semId, semOps, 1);
//
//
//}
//
//#endif

//int main() {
//    file_mapping();
//}


//-----------------------------------------

#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "logger_IPC.h"
#include "database.h"
#define SEMAPHORE_KEY 54431
using namespace std;


int semId;
#include <sys/sem.h>


void file_mapping(){
    int file_descriptor;
    const int file_length = 400;
    const char* path = "/home/dima/CLionProjects/client_2/nothing.txt";

    file_descriptor = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (file_descriptor == -1){
        std::cout<<"problem with semaphore"<<std::endl;
        exit(1);
    }

    lseek(file_descriptor, file_length - 1, SEEK_SET);
    write(file_descriptor, "", 1);

    char* adr = reinterpret_cast<char*>(mmap(NULL, file_length, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0));
    if (adr == MAP_FAILED){
        std::cout<<"problem with mmap"<<std::endl;
        close(file_descriptor);
        exit(1);
    }

    std::cout << "server started" << endl;
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
    logger* logg = new logger_ipc("/home/dima/CLionProjects/logger_server/bred.txt");
    auto* databas = new database(logg);
    while (true) {
        std::cout << "server waiting client" << endl;
        semOps[0].sem_op = -1;
        semOps[1].sem_op = 0;
        semop(semId, semOps, 2);
        std::string mess = adr;
//        if (mess == "new client"){
//            client_count++;
//            sprintf(adr, "%s", "Add client");
//            semOps[0].sem_op = 1;
//            semOps[1].sem_op = 1;
//            semop(semId, semOps, 2);
//            continue;
//        }
//        if (mess == "Exit"){
//            sprintf(adr, "%s", "Client exit");
//            semOps[0].sem_op = 1;
//            semOps[1].sem_op = 1;
//            semop(semId, semOps, 2);
//            if (client_count != 1){
//                client_count--;
//                continue;
//            }else if (client_count == 1){
//                break;
//            }
//        }
        std::cout << "server reading data, writing result" << endl;

        stringstream x;
        std::string word;
        std::string key;
        std::string value;
        std::string command;
        x<<mess;
        int c = 0;
        while (std::getline(x, word)){
            if (c == 0){
                command = word;
            }else if (c == 1){
                key = word;
            }else if (c == 2){
                value = word;
            }
            c++;
        }
        std::string message;
        try {
            message = databas->parsing_enter(command, key, value);
        }catch (const std::logic_error& er){
            message = er.what();
            logg->log(er.what(), logger::severity::warning);
        }
        // TODO: FOR DEBUG!
        sprintf(adr, "%s", message.c_str());

        semOps[0].sem_op = 1;
        semOps[1].sem_op = 1;
        semop(semId, semOps, 2);

    }
    delete[] semOps;
    delete databas;
    delete logg;
}


void clear_resurs(){
    std::cout<<"dvv"<<std::endl;
    semctl(semId, 0, IPC_RMID, 0);
}

int main() {
    //file_mapping();
    atexit(clear_resurs);
    file_mapping();
    return 0;


}
