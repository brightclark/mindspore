# Copyright 2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

"""ClearZero op"""

from mindspore.ops.op_info_register import op_info_register


@op_info_register("""{
    "op_name": "ClearZero",
    "imply_type": "AutoDiff",
    "fusion_type": "ELEMWISE",
    "attr": [
        {
            "name": "pad_mod",
            "param_type": "optional",
            "type": "string"
        },
        {
            "name": "window",
            "param_type": "optional",
            "type": "int"
        },
        {
            "name": "pad",
            "param_type": "optional",
            "type": "int"
        },
        {
            "name": "stride",
            "param_type": "optional",
            "type": "int"
        }
    ],
    "inputs": [
        {
            "index": 0,
            "dtype": [
                "int32", "float16", "float32", "int32", "float16", "float32"
            ],
            "format": [
                "DefaultFormat", "DefaultFormat", "DefaultFormat", "NC1HWC0", "NC1HWC0", "NC1HWC0"
            ],
            "name": "x"
        }
    ],
    "outputs": [
    ]
}""")
def _clear_zero_akg():
    """MaxPoolGradWithArgmax AutoDiff register"""
    return
