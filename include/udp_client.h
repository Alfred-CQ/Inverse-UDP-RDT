#ifndef __UDP_CLIENT__
    #define __UDP_CLIENT__

    #include "main_header.h"

    class UDP_Client
    {
        public:
           /* Constructors and Destructor*/
            UDP_Client              (string _ip, uint _port);
           ~UDP_Client              ();
            
           /** Methods **/
           /* Senders */
            void send_Request       (string resource_request);

           /* Receivers */
            void recv_Responses     ();

           /* Utilities */
            void print_Information  ();

        private:
           /* Variables */
            uint            port;

            int             sockFD;
            int             bytes_send;
            
            SOCK_ADDR_IN    server_addr;
            HOST*           host;
            socklen_t       addr_len;

            char*           recv_buffer;
    };

#endif // !__UDP_CLIENT__