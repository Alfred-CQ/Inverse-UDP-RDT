#ifndef __SEGMENT__
    #define __SEGMENT__

    #include "main_header.h"
    #include "utils.h"
        
    struct Segment
    {
       /* Variables */
        string  padding_str;
        string  data;
        string  checksum_str;

       /* Constructors and Destructor */
        Segment (uint _padding, uint _checksum, string _data) : padding_str (utils::complete_Bytes(_padding, PADDING_SIZE)),
                                                                data        (_data),
                                                                checksum_str(std::to_string(_checksum))
                                                                {}
       ~Segment ();
    };
    
#endif // !__SEGMENT__