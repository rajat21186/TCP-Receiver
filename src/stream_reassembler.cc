#include "stream_reassembler.hh"

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : buffer(capacity, '\0')
    , map(capacity, false)
    , _output(capacity)
    , _capacity(capacity),
    unass_index(0)
    , unassbytesize(0)
    , endoffile(false)
     {}




//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (eof == true) {
        endoffile = true;
    }
    size_t len = data.size();
    if (len == 0 ) {
        if(endoffile == true) {
            if(unassbytesize == 0) {
                _output.end_input();
                return;
            }
        }
        
    }
    // check invalid index
    if (index >= unass_index + _capacity) return;
    // this case handles if bytes are coming inorder or the bytes coming are greater than the expected index
    if (index >= unass_index) {
        size_t real;
        if(_capacity - _output.buffer_size() > len + index - unass_index) {
            real = len;
        }
        else{
            real = _capacity - _output.buffer_size()- index + unass_index;
        }
         
        if (len > real) {
            endoffile = false;
        }
        size_t i = 0;
        for (; i < real; i++) {
            if (map[i + index - unass_index] == true)
                continue;
            buffer[i + index - unass_index] = data[i];
            map[i + index - unass_index] = true;
            unassbytesize++;
        }
    }
    // this case handles if bytes coming are previously also received 
    else if (index + len > unass_index) {
        size_t real_len;
        if(_capacity - _output.buffer_size() > len - unass_index + index) {
            real_len = len - unass_index + index;
        }
        else{
            real_len = _capacity - _output.buffer_size();
        }
         
        if (len  > real_len + unass_index - index) {
            endoffile = false;
        }
        size_t i = 0;
        for (; i < real_len; i++) {
            if (map[i] == true)
                continue;
            buffer[i] = data[i + unass_index - index];
            map[i] = true;
            unassbytesize++;
        }
    }
    // this stores the data till it is unassembled and then finally when data is assembled  it writes to the _output bytestream
    
    string temp = "";
    while (true) {
        if(map.front()) {
            temp.push_back(buffer.front());
            buffer.pop_front();
            map.pop_front();
            buffer.push_back('\0');
            map.push_back(false);
            unass_index++;
            unassbytesize--;
        }
        else{
            break;
        }
    }
    if (temp != "") {
        _output.write(temp);
    }
    // this ends the input
    if (endoffile == true ) {
        if(unassbytesize == 0)
            _output.end_input();
    }
    

    
}

size_t StreamReassembler::unassembled_bytes() const { return unassbytesize; }

bool StreamReassembler::empty() const {  
    if(unassbytesize == 0) return true;
    return false;
 }

size_t StreamReassembler::ack_index() const { return unass_index; }
