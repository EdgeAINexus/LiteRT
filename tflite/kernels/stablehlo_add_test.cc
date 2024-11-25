/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tflite/c/c_api_types.h"
#include "tflite/c/common.h"
#include "tflite/core/subgraph.h"
#include "tflite/kernels/test_util.h"
#include "tflite/schema/schema_generated.h"

namespace tflite {
namespace {

using ::testing::ElementsAre;

class AddOpModel : public SingleOpModel {
 public:
  AddOpModel(const TensorData& input1, const TensorData& input2,
             const TensorData& output) {
    input1_ = AddInput(input1);
    input2_ = AddInput(input2);
    output_ = AddOutput(output);
    SetBuiltinOp(BuiltinOperator_STABLEHLO_ADD, BuiltinOptions_NONE, 0);
    SetBypassDefaultDelegates();
    BuildInterpreter({GetShape(input1_), GetShape(input2_)});
  }

  int input1() { return input1_; }
  int input2() { return input2_; }

  std::vector<float> GetOutput() { return ExtractVector<float>(output_); }

 protected:
  int input1_;
  int input2_;
  int output_;
};

TEST(StablehloElementwise, AddWorks) {
  AddOpModel model({TensorType_FLOAT32, {1, 2, 2, 1}},
                   {TensorType_FLOAT32, {1, 2, 2, 1}},
                   {TensorType_FLOAT32, {}});
  model.PopulateTensor<float>(model.input1(), {-2.0, 0.2, 0.7, 0.8});
  model.PopulateTensor<float>(model.input2(), {0.1, 0.2, 0.3, 0.5});
  ASSERT_EQ(model.Invoke(), kTfLiteOk);
  EXPECT_THAT(model.GetOutput(), ElementsAre(-1.9, 0.4, 1.0, 1.3));
}

}  // namespace
}  // namespace tflite