#include "../include/message.h"
#include "../include/utils.h"

/* Constructors */
Response::Response(string _resource_name, uint _stream, uint _number_segments)
{
    resource_name   = _resource_name;
    stream          = _stream;
    number_segments = _number_segments;
}

/* Methods */
// Core
void Response::insert_Segment(uint _sequence_number, uint _padding, char* _data)
{
    string data { _data };
    Segment* segment = new Segment(_padding, utils::make_Checksum(data), data);

    resource_segments.insert({_sequence_number, segment});
}

void Response::send_Segment(uint sequence_number)
{
    string segment_str = std::to_string(stream) +
                         utils::complete_Bytes( sequence_number, 5 ) +
                         utils::complete_Bytes( number_segments, 5 ) +
                         resource_segments[sequence_number]->padding_str +
                         resource_segments[sequence_number]->data +
                         resource_segments[sequence_number]->checksum_str;

    sendto( source_sockFD, &(segment_str.front()), segment_str.size(), 
            MSG_CONFIRM, addr_destination, SOCK_ADDR_SIZE );
}

void Response::build_Response(string folder)
{
    cout << "************************************************************\n"
                 << "      📬 Resource received in stream " << stream << endl;

    ofstream out_file (folder + "/" + resource_name);

    for (auto it = resource_segments.begin(); it != resource_segments.end(); ++it)
        out_file << (it->second)->data << "\n";

    print_Head();
    print_Tail();

    cout << "************************************************************\n";
}

// Getters
Segment* Response::get_Segment(uint sequence_number)
{
    return resource_segments[sequence_number];
}

uint Response::get_Stream()
{
    return stream;
}

uint Response::get_Numb_Segments()
{
    return number_segments;
}

uint Response::get_Counter()
{
    return this->resource_segments.size();
}

// Setters
void Response::set_Source(int _source_sockFD)
{
    source_sockFD = _source_sockFD;
}

void Response::set_Destination(sockaddr* _addr_destination)
{
    addr_destination = _addr_destination;
}

// Utils
void Response::print_Head(uint _number_segments)
{
    auto segment_it = resource_segments.begin();

    print_Segments(segment_it, _number_segments, "HEAD");
}

void Response::print_Tail(uint _number_segments)
{
    auto segment_it = (resource_segments.end());
    
    --segment_it;

    if (_number_segments <= resource_segments.size())
    {
        for (size_t i = 1; i < _number_segments; ++i)
            --segment_it;
    }
    
    print_Segments(segment_it, _number_segments, "TAIL");
}

void Response::print_Segments(map<uint, Segment*>::iterator segment_it, uint _number_segments, string title)
{
    cout << "***********************" << title << "****************************\n"
         << "* S | SEQUENCE # | MESSAGE # | PADDING |   DATA   | CKV *\n";

    if (_number_segments > resource_segments.size())
        print_One_Segment( segment_it->first );
    else
    {
        for(size_t i = 0; i < _number_segments; ++i, ++segment_it)
            print_One_Segment( segment_it->first );
    }
}

void Response::print_One_Segment(uint _sequence_number)
{
    cout << "| " << stream << setw(2);
    cout << "|"  << "   " << setfill ('0') << setw (5) << _sequence_number << setfill (' ') << setw(5);
    cout << "|"  << "   " << setfill ('0') << setw (5) << number_segments << setfill (' ') << setw(4);
    cout << "|"  << "   " << resource_segments[_sequence_number]->padding_str << setw (4);
    cout << "|"  << " " << resource_segments[_sequence_number]->data.substr(0,8) << " ";
    cout << "|  " << resource_segments[_sequence_number]->checksum_str << "  |" << setfill(' ') << endl;
}