/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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
#ifndef TENSORFLOW_LITE_TOCO_TOCO_CONVERT_H_
#define TENSORFLOW_LITE_TOCO_TOCO_CONVERT_H_

#include <string>

#include "tensorflow/core/lib/core/status.h"
#include "tflite/toco/args.h"
#include "tflite/toco/model_flags.pb.h"
#include "tflite/toco/toco_flags.pb.h"

namespace toco {

absl::Status Convert(const std::string& graph_def_contents,
                     const TocoFlags& toco_flags, const ModelFlags& model_flags,
                     std::string* output_file_contents,
                     int64_t* arithmetic_ops_count = nullptr);

absl::Status Convert(const ParsedTocoFlags& parsed_toco_flags,
                     const ParsedModelFlags& parsed_model_flags);
}  // namespace toco

#endif  // TENSORFLOW_LITE_TOCO_TOCO_CONVERT_H_