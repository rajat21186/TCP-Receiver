#include "wrapping_integers.hh"
#include<bits/stdc++.h>
#include <iostream>
#include<math.h>
using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) { 
    WrappingInt32 a = isn + (uint32_t)(n); 
    return a;
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
	uint64_t ans = 0;
    uint64_t ans1 = 0;
    double x = pow(2,32);
    if (n - isn < 0) {
        
        ans = uint64_t(n - isn + static_cast<uint64_t>(x));
    } else {
        ans = uint64_t(n - isn);
    }
    if (checkpoint <= ans) 
        return ans;
    double x1 = checkpoint / x;
    uint64_t temp = static_cast<uint64_t>(x1);
    
    ans = ans | (temp * static_cast<uint64_t>(x));
    while (ans <= checkpoint)
        ans += static_cast<uint64_t>(x);
    ans1 = ans - static_cast<uint64_t>(x);

    
    if(ans - checkpoint > checkpoint - ans1) return ans1;
    return ans;
    
 //return 0;
}
