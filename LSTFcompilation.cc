#include <iostream>
#include <random>

#include "pifo_pipeline_stage.h"
#include "pifo_pipeline.h"

int main() {
  try {
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> ele_dis(1, 2);

    // Traffic generation
    PIFOPacket test_packet;

    // Single PIFO pipeline stage consisting of
    // 1 priority and queue
    // (every PIFO pipeline stage has 1 calendar queue)
PIFOPipelineStage pifo1(1,
"ptr",
{{666, {Operation::TRANSMIT, {}}},},
[] (const auto & x) {if (x("ptr")==666){return x("slack");}
uint32_t y =0; return y;});
PIFOPipeline LSTF_pipeline({pifo1,});
PIFOPipeline mesh=LSTF_pipeline;
for (uint32_t i = 0; i < 10000; i++) {
      if(ele_dis(gen)==1){test_packet("class") = 1; test_packet("slack")=1;}
      else{test_packet("class") = 2; test_packet("slack")=2;}
      test_packet("time")= (int) i;
      test_packet("ptr") = 666;
      mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
    }

    std::cout << "Finished enqs\n";

    for (uint32_t i = 10000; i < 20000; i++) {
      auto result = mesh.deq(0, QueueType::PRIORITY_QUEUE, 0, i);
      std::cout << "Deq result is \"" << result << "\"" << std::endl;
    }

    assert_exception(mesh.deq(0, QueueType::PRIORITY_QUEUE, 0, 20000).initialized() == false);
  } catch (const std::exception & e) {
    std::cerr << "Caught exception in main " << std::endl << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}