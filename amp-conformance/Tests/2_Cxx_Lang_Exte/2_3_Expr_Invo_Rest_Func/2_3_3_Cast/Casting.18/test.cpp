// Copyright (c) Microsoft
// All rights reserved
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache Version 2.0 License for specific language governing permissions and limitations under the License.
/// <tags>P2</tags>
/// <summary>define class c; int c::func(int) __GPU; then int (c::*pfn)(int) __GPU = reinterpret_cast <( (int (*)(int)) >(func); call pfn in non __GPU context</summary>

#include <amptest.h>

class c
{
public:
    int f(int) __GPU
    {
        return 1;
    }
};

bool test()
{
    int flag = 0;

    bool passed = true;

    int v = 0;

    int (c::*pfn)(int) = reinterpret_cast<int (c::*)(int)>(&c::f);

    c o;

    flag = (o.*pfn)(v);

    if (flag != 1)
    {
        return false;
    }

    return passed;
}

int main(int argc, char **argv)
{
    return test() ? 0 : 1;
}

