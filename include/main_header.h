#ifndef __MAIN_HEADER_H__
#define __MAIN_HEADER_H__

/* Libraries */
// NETWORK - SOCKETS
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// UNIX
#include <unistd.h>
#include <errno.h>

// C
#include <string.h>

// STL
#include <iostream>
#include <iomanip> 

#include <string>
#include <vector>
#include <map>

#include <filesystem>
#include <fstream>

#include <thread>

/* Usings */
using std::cout;            using std::cin;   
using std::endl;            using std::setw;
using std::setfill;
using std::string;          using std::ifstream;
using std::ofstream;        using std::vector;
using std::thread;          using std::map;

// Namespaces
namespace fs = std::filesystem;

/* Typedefs */
// Connection
typedef struct sockaddr_in  SOCK_ADDR_IN;
typedef struct sockaddr     SOCK_ADDR;
typedef struct in_addr      IN_ADDR;
typedef struct hostent      HOST;

// Program
typedef unsigned int        uint;
typedef long unsigned int   luint;
typedef vector<string>      RESOURCES;

/* Definitions */
// Sizes
#define MAX_MESSAGE_SIZE    1024
#define MAX_DATA_SIZE       10
#define CHAR_BITS_SIZE      8

#define MIN_MESSAGE_SIZE    0

#define SOCK_ADDR_SIZE      sizeof(sockaddr)
#define REQUEST_NAME_SIZE   102
#define PADDING_SIZE        3

#define NUMBER_STREAMS      10

#define HEAD_MESSAGES       3
#define TAIL_MESSAGES       3    

// Connection
#define PORT                5000

// Commands
#define ALL                 "-all"
#define START               "-start"

// Directories
#define RESOURCES_DIR       "resources"
#define REQUESTS_DIR        "requests"

// States
#define AVAILABLE           true
#define UNAVAILABLE         false

#endif // !__MAIN_HEADER_H__