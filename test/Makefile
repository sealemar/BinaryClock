# Copyright [2013] [Sergey Markelov]
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
#
# developed by Sergey Markelov (11/10/2013)
#

.PHONY: lib

INC          := -include errno.h
POST_INCLUDE := -include logger.h -include test_ut/test_ut.h
SOURCES_DIRS  = test_ut
PROG         := tests
CTAGS_FILE   := ../etc/test.tags
CTAGS_DIR    := ../test

include ../include/Makefile.include

$(BIN_DIR)/$(PROG): lib

lib:
	make -C ../lib
