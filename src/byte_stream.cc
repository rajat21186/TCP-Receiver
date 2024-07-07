#include "byte_stream.hh"

#include <algorithm>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */


using namespace std;

ByteStream::ByteStream(const size_t capa): stream(),_capacity(capa){}

size_t ByteStream::write(const string &data) {
 	size_t n = data.size();
 	size_t ans = 0;
	size_t i = 0;
	for(; i < n; i++) {
		if(_capacity - stream.size() <= 0) {
			break;
		}
		stream.push_back(data[i]);
		ans++;
		byteswritten++;
		
		
	}
	 
	return ans;
	
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
	size_t canPeek = min(len,stream.size());
	string ans = "";
	size_t i = 0;
	for(; i < canPeek; i++) {
		ans.push_back(stream[i]);
	}
	return ans;


  //return {};
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
	if(len > stream.size()) {
		set_error();
		return;
	}
	size_t i = 0;
	for(; i < len; i++){
		stream.pop_front();
		bytesread++;
	}

}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
	string ans = "";
	if(len > stream.size()) {
		set_error();
		return ans;
	}
	size_t i= 0;
	for(; i < len; i++) {
		char c = stream.front();
		ans.push_back(c);
		stream.pop_front();
		bytesread++;
	}
	return ans;
  //return {};
}

void ByteStream::end_input() {endwrite = true; }

bool ByteStream::input_ended() const { return endwrite;}//return false;}

size_t ByteStream::buffer_size() const {return stream.size();} //return 0; }

bool ByteStream::buffer_empty() const {return stream.size() == 0;}//return false; }

bool ByteStream::eof() const { return endwrite && buffer_empty();}//return false; }

size_t ByteStream::bytes_written() const { return byteswritten;} //return 0; }

size_t ByteStream::bytes_read() const { return bytesread;} //return 0;  }

size_t ByteStream::remaining_capacity() const { return _capacity - stream.size();}//rq
										  //eturn 0; }
