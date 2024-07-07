#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader head = seg.header();
    // ...
    
    if (head.syn == false) {
        if(_synReceived == false)
            return;
    }

    // extract data from the payload
    string datatoreceive = seg.payload().copy();

    bool endoffile = false;

    // first SYN received
    if (head.syn == true ) {
        if(_synReceived == false) {
            _synReceived = true;
            _isn = head.seqno;
            // This case handles if fin is received when we get the first byte of data
            if (head.fin == true) {
                _finReceived = true;
                endoffile = true;
            }
            _reassembler.push_substring(datatoreceive, 0, endoffile);
            return;
        }
    }

    // FIN received
    if (_synReceived == true ) {
        if(head.fin == true) {
            _finReceived = true;
            endoffile = true;
        }
        
    }

    // --- Hint ------
        // convert the seqno into absolute seqno
    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno - _synReceived;
    // --- Hint ------  

    // ... 
    _reassembler.push_substring(datatoreceive, stream_idx, endoffile);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (_synReceived == false) {
        return nullopt;
    }
    bool x = 0;
    if(_reassembler.empty() == true) {
        if(_finReceived == true) {
            x = 1;
        }
    }

    uint64_t t = _reassembler.ack_index() + 1 + x;
    WrappingInt32 ans = wrap(t,_isn);
    return ans;
}

size_t TCPReceiver::window_size() const { 
    size_t a = _capacity - _reassembler.stream_out().buffer_size();
	return a;
}//return 0; }
