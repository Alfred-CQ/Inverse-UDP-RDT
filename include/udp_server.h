#ifndef __UDP_SERVER__
    #define __UDP_SERVER__

    #include "main_header.h"

    class UDP_Server
    {
        public:
           /* Constructors */
            UDP_Server              (string _ip, uint _port = PORT);
           ~UDP_Server              ();

           /** Methods **/
           /* Senders */
            void send_Response      (string message);

           /* Receivers */
            void recv_Requests      ();

           /* Utilities */
            void print_Information  ();
            
        private:
           /* Variables */
            uint            port;
            
            int             sockFD;
            int             bytes_recv;
            int             bytes_send;

            string          ip;
            string          current_path;
            string          resources_path;

            SOCK_ADDR_IN    addr;
            SOCK_ADDR_IN    client_addr;
            socklen_t       addr_len;

            char            recv_buffer[1024];
    };
        
#endif // !__UDP_SERVER__