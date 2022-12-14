#ifndef __UDP_CLIENT__
    #define __UDP_CLIENT__

    #include "main_header.h"
    #include "message.h"

    class UDP_Client
    {
        public:
           /* Constructors and Destructor*/
            UDP_Client              (string _ip, uint _port);
           ~UDP_Client              ();
            
           /** Methods **/
           /* Senders */
            void send_Request       (string resource_request);
            void resend_Request     (uint stream, uint sequence_number, string type);

           /* Receivers */
            void recv_Responses     ();

           /* RDT */
            void time_Out           (uint stream, uint sequence_number);

           /* Utilities */
            void print_Information  ();

        private:
           /* Variables */
            uint                port;
            uint                delay_value;

            int                 sockFD;
            int                 bytes_recv;
            int                 bytes_send;
            
            SOCK_ADDR_IN        server_addr;
            HOST*               host;
            socklen_t           addr_len;

            char*               recv_buffer;

            vector<bool>        streams_status;
            vector<Response*>   streams;
            RESOURCES           resource_names;

            string              current_path;
            string              requests_path;
    };

#endif // !__UDP_CLIENT__