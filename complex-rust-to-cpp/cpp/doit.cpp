#include "my_ffi.h"

using namespace my_ffi;

static float ProcessNode(const TreeNode& node) {
  switch (node.tag) {
    case TreeNode::Tag::Leaf:
      return node.AsLeaf();
    case TreeNode::Tag::Sum: {
      float result = 0.0;
      for (auto& child : node.AsSum().AsSpan()) {
        result += ProcessNode(child);
      }
      return result;
    }
    case TreeNode::Tag::Cmp: {
      float left = ProcessNode(*node.AsCmp()._0);
      float right = ProcessNode(*node.AsCmp()._1);
      return fabs(left - right) < 1e-6;
    }
  }
  assert(false && "Should be unreachable");
  return 0.0f;
}

extern "C" float DoTheMathInCPlusPlus(const TreeNode* node) {
  // This (silly) copy shouldn't leak or anything. Just here for demo purposes.
  TreeNode nodeCopy = *node;
  float answer = ProcessNode(*node);
  assert(answer == ProcessNode(nodeCopy));
  return answer;
}
