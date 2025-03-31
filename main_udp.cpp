#include <bits/types/FILE.h>
#include <cstdint>
#include <time.h>
#include <filesystem>
#include <sys/mman.h>
#include <pthread.h>
#include "socket.h"
//#include "file_handling.h"

#define PACKET_SIZE         (65535-28-8)  //8bytes udp header + 4bytes seq + 4bytes total
#define BUFFER_SIZE         100016973       //1527*65499
#define FILE_TO_SEND        "./file_send"
#define FILE_TO_RECEIVE     "./file_receive"
#define TESTS               1

char thread_side[BUFFER_SIZE];           
char main_side[BUFFER_SIZE];           
unsigned int main_chunk = 0;
pthread_spinlock_t spinlock;


void* reader_thread(void*) {
    pthread_spin_lock(&spinlock);
    pthread_spin_unlock(&spinlock);
    return nullptr;
}


int main(int argc, char* argv[]){

    if(argc > 1){
        pthread_t reader; 
        pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
        pthread_spin_lock(&spinlock); 
        pthread_create(&reader, nullptr, reader_thread, nullptr);

        cout << "is server" << endl;
        unsigned char tempBuff[PACKET_SIZE+8];
        int receiving_socket = create_receiving_udp_socket();
        int seq = 0, old_seq = 0, self_seq = 0, test_number = 0, received = 0, fd_to_write;

        for(int i=0; i<TESTS; i++){
            fd_to_write = open(FILE_TO_RECEIVE, O_WRONLY);
            while(1){
                received = recv(receiving_socket, 
                                        tempBuff, 
                                        PACKET_SIZE+8, 
                                        0);
                if(received<1) continue;
                //cout << received << endl;
                //seq = *(uint32_t*)(&tempBuff[0]); total = *(uint32_t*)(&tempBuff[4]);
                std::memcpy(&seq, &tempBuff[0], sizeof(seq));
                self_seq++;
                if(seq - old_seq != 1){
                    cout << "--- " << seq << "-" << old_seq << "==" << seq-old_seq << endl;
                }
                old_seq = seq;
                main_chunk += received;

                //write(fd_to_write, tempBuff+8, received); 

                if(received < PACKET_SIZE){
                    cout << "received " << received << "=> RECEIVED ALL? " << self_seq << "/ test number " << ++test_number << endl;
                    seq = 0; 
                    old_seq = 0;
                    self_seq = 0;
                    close(fd_to_write);
                    //break;
                }
            }
        }
        
    }
    else{
        struct sockaddr_in addr_t;
        memset(&addr_t, 0, sizeof(addr_t));
        addr_t.sin_family = AF_INET;
        addr_t.sin_port   = htons(8080);
        inet_pton(AF_INET, "192.168.1.199", &(addr_t.sin_addr));
        int sending_socket = create_sending_udp_socket();
        cout << "is sender" << endl;

        //file handling        
        unsigned char sendBuff[PACKET_SIZE+8];
        uint32_t* sendBuff32_ptr = reinterpret_cast<uint32_t*>(sendBuff);
        uint32_t fd_file_size = 0, fd_read_bytes = 0, seq = 1, total = 0, fd_to_send;

        for(int i=0; i<TESTS; i++){
            fd_to_send   = open(FILE_TO_SEND, O_RDONLY); 
            fd_file_size = std::filesystem::file_size(FILE_TO_SEND);
            //total        = (fd_file_size / PACKET_SIZE) + (fd_file_size % PACKET_SIZE != 0);
            total        = (fd_file_size / PACKET_SIZE) + 1; //last packet len == 0;
            cout << "File size: " << fd_file_size << " bytes || total packets == " << total << endl;
            for(int j=0; j<total; j++){
                fd_read_bytes = read(fd_to_send, (sendBuff+8), PACKET_SIZE);
                sendBuff32_ptr[0] = seq++; sendBuff32_ptr[1] = total;
                sendto(sending_socket, 
                        sendBuff, 
                        fd_read_bytes+8,
                        0, 
                        (sockaddr*) &addr_t, 
                        sizeof(addr_t));

                //for(int i=0; i<1000000; i++){}
                usleep(400);

                //cout << fd_read_bytes << endl;

            }
            cout << "DONE!" << endl;
            seq = 1;
            close(fd_to_send);
        }
    }

    return 0;
}
