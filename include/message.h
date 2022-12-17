#ifndef __MESSAGE__
    #define __MESSAGE__

    #include "main_header.h"
    #include "segment.h"

    class Response
    {
        public: 
           /* Constructor and Destructor*/
            Response                (string _resource_name, uint _stream, uint _number_segments);
           ~Response                ();

           /** Methods **/
           // Core
            void insert_Segment     (uint sequence_number, uint padding, char* data);
            void send_Segment       (uint sequence_number);
            void build_Response     (string folder);

           // Getters
            Segment* get_Segment        (uint sequence_number);
            uint     get_Stream         ();
            uint     get_Numb_Segments  ();
            uint     get_Counter        ();

           // Setters
            void set_Source         (int _source_sockFD);
            void set_Destination    (sockaddr* _addr_destination);

           // Utils
            void print_Head         (uint _number_segments = HEAD_MESSAGES);
            void print_Tail         (uint _number_segments = TAIL_MESSAGES);
            void print_Segments     ( map<uint, Segment*>::iterator segment_it, 
                                          uint _number_segments, string title );

            void print_One_Segment  (uint _sequence_number);

        private:
           /* Variables */
            int                     source_sockFD;
            sockaddr*               addr_destination;

            uint                    stream;
            uint                    number_segments;

            string                  resource_name;

            map<uint, Segment*>     resource_segments;
    };
    
#endif // !__MESSAGE__