/*
 * Copyright 2012 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef FOLLY_EXPERIMENTAL_SYMBOLIZER_ELF_H_
# error This file must be included from Elf.h
#endif

namespace facebook {
namespace symbolizer {

template <class Fn>
const ElfW(Shdr)* ElfFile::iterateSections(Fn fn) const {
  const ElfW(Shdr)* ptr = &at<ElfW(Shdr)>(elfHeader().e_shoff);
  for (size_t i = 0; i < elfHeader().e_shnum; i++, ptr++) {
    if (fn(*ptr)) {
      return ptr;
    }
  }

  return nullptr;
}

template <class Fn>
const ElfW(Shdr)* ElfFile::iterateSectionsWithType(uint32_t type, Fn fn)
  const {
  return iterateSections(
      [&](const ElfW(Shdr)& sh) {
        return sh.sh_type == type && fn(sh);
      });
}

template <class Fn>
const char* ElfFile::iterateStrings(const ElfW(Shdr)& stringTable, Fn fn)
  const {
  validateStringTable(stringTable);

  const char* start = file_ + stringTable.sh_offset;
  const char* end = start + stringTable.sh_size;

  const char* ptr = start;
  while (ptr != end && !fn(ptr)) {
    ptr += strlen(ptr) + 1;
  }

  return ptr != end ? ptr : nullptr;
}


}  // namespace symbolizer
}  // namespace facebook

