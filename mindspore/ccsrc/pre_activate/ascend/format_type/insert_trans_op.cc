/**
 * Copyright 2019 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pre_activate/ascend/format_type/insert_trans_op.h"
#include <memory>
#include <vector>
#include "utils/utils.h"
#include "pre_activate/ascend/ascend_helper.h"
#include "session/anf_runtime_algorithm.h"
#include "device/kernel_info.h"
#include "kernel/oplib/oplib.h"
#include "utils/context/ms_context.h"

namespace mindspore {
namespace opt {
const BaseRef InsertTransOp::DefinePattern() const {
  std::shared_ptr<Var> V = std::make_shared<CondVar>(UnVisited);
  std::shared_ptr<Var> Xs = std::make_shared<SeqVar>();
  return VectorRef({V, Xs});
}

bool IsGraphOutput(const AnfNodePtr &node, const std::vector<AnfNodePtr> &outputs) {
  auto iter = std::find(outputs.begin(), outputs.end(), node);
  if (iter != outputs.end()) {
    return true;
  }

  return false;
}

const AnfNodePtr InsertTransOp::Process(const FuncGraphPtr &func_graph, const AnfNodePtr &node,
                                        const EquivPtr &) const {
  if (node == nullptr || !AnfAlgo::IsRealKernel(node)) {
    return nullptr;
  }
  AnfNodePtr front_node;
  auto kernel_graph = func_graph->cast<std::shared_ptr<session::KernelGraph>>();
  if (kernel_graph != nullptr && kernel_graph->IsInternalOutput(node)) {
    front_node = kernel_graph->GetFrontNodeByInternalOutput(node);
  }
  AnfAlgo::SetNodeAttr(kAttrVisited, MakeValue(true), node);
  MS_LOG(DEBUG) << "====process op: " << node->DebugString();
  AnfNodePtr new_node = InsertTransOpForInput(func_graph, node, kernel_select_);
  auto ms_context = MsContext::GetInstance();
  MS_EXCEPTION_IF_NULL(ms_context);
  if (ms_context->execution_mode() == kPynativeMode && !ms_context->enable_pynative_hook()) {
    if (IsGraphOutput(node, AnfAlgo::GetAllOutput(func_graph->output(), {prim::kPrimTupleGetItem}))) {
      return new_node;
    }
  }
  auto final_node = InsertTransOpForOutput(func_graph, new_node, kernel_select_);
  if (kernel_graph != nullptr && front_node != nullptr) {
    auto old_node = kernel_graph->GetInternalOutputByFrontNode(front_node);
    kernel_graph->ReplaceInternalOutput(old_node, final_node);
  }
  return final_node;
}
}  // namespace opt
}  // namespace mindspore
