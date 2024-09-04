#!/usr/bin/env bash
# Copyright 2024 The AI Edge LiteRT Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
set -ex

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GEN_DIR="gen"

if [ ! -d /third_party_tensorflow ]; then
  # Running on host.
  cd ${SCRIPT_DIR}
  rm -fr ${GEN_DIR}

  docker build . -t tflite-builder -f tflite-android.Dockerfile

  docker run -v ${SCRIPT_DIR}/../third_party/tensorflow:/third_party_tensorflow \
    -v ${SCRIPT_DIR}:/script_dir \
    -e RELEASE_VERSION=1.0.1 \
    --entrypoint /script_dir/build_maven_with_docker.sh tflite-builder

  echo "Output can be found here:"
  tree ${GEN_DIR}
  
  exit 0
else
  # Running inside docker container, download the SDK first.
  licenses=('y' 'y' 'y' 'y' 'y' 'y' 'y')
  printf '%s\n' "${licenses[@]}" | sdkmanager --licenses
  sdkmanager \
    "build-tools;${ANDROID_BUILD_TOOLS_VERSION}" \
    "platform-tools" \
    "platforms;android-${ANDROID_API_LEVEL}"

  cd /third_party_tensorflow

  # Run configure.
  configs=(
    '/usr/bin/python3'
    '/usr/lib/python3/dist-packages'
    'N'
    'N'
    'Y'
    '/usr/lib/llvm-18/bin/clang'
    '-Wno-sign-compare -Wno-c++20-designator -Wno-gnu-inline-cpp-without-extern'
    'y'
    '/android/sdk'
  )
  printf '%s\n' "${configs[@]}" | ./configure

  bash /script_dir/build_android_package.sh
fi
