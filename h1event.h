//
//  h1event.h
//  
//
//  Created by Simon Leisibach on 25.09.19.
//

#ifndef h1event_h
#define h1event_h

#include <cstdint>
#include <vector>


// comment on the right side tells branch ordering.
struct H1Event {
   float ptds_d; // 61
   float etads_d; // 62
   float dm_d; // 63
   std::int32_t ik; // 68
   std::int32_t ipi; // 69
   std::int32_t ipis; // 70
   float ptd0_d; // 72
   float md0_d; // 74
   float rpd0_t; // 94
   std::int32_t ntracks; // 112 -> almost always has value 3
   
   struct ntrack {
      std::int32_t nhitrp; //120
      float rstart; // 124
      float rend; // 125
      float nlhk; // 128
      float nlhpi; // 129
   };
   
   std::vector<ntrack> trackInfo; // trackInfo.size() = ntracks
   std::int32_t njets; // 137
};

#endif
