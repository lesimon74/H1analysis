//
//  h1event.h
//  
//
//  Created by Simon Leisibach on 25.09.19.
//

#include <vector>

#ifndef h1event_h
#define h1event_h
// comment on the right side tells branch ordering.
struct H1Event {
   float ptds_d; // 61
   float etads_d; // 62
   float dm_d; // 63
   int ik; // 68
   int ipi; // 69
   int ipis; // 70
   float ptd0_d; // 72
   float md0_d; // 74
   float rpd0_t; // 94
   int ntracks; // 112 -> almost always has value 3
   std::vector<int> nhitrp; // 120, nhitrp.size() = ntracks
   std::vector<float> rstart; // 124, rstart.size() = ntracks
   std::vector<float> rend; // 125, rend.size() = ntracks
   std::vector<float> nlhk; // 128, nlhk.size() = ntracks
   std::vector<float> nlhpi; // 129, nlhpi.size() = ntracks
   int njets; // 137
};

#endif
