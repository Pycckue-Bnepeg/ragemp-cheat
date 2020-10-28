#
# This file was taken from RakNet 4.082.
# Please see licenses/RakNet license.txt for the underlying license and related copyright.
#
#
# Modified work: Copyright (c) 2019, SLikeSoft UG (haftungsbeschränkt)
#
# This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
# license found in the license.txt file in the root directory of this source tree.
#

project(PostgreSQLInterface)
IF (NOT WIN32 AND UNIX)
	FILE(GLOB ALL_HEADER_SRCS *.h)
	FILE(GLOB ALL_CPP_SRCS *.cpp)
	FINDPOSTGRE()
	include_directories(./ ${SLIKENET_HEADER_FILES} ${POSTGRESQL_INCLUDE_DIR}) 
	add_library(LibPostgreSQLInterface STATIC ${ALL_CPP_SRCS} ${ALL_HEADER_SRCS})
	target_link_libraries (LibPostgreSQLInterface ${SLIKENET_COMMON_LIBS} ${POSTGRESQL_LIBRARIES})
	VSUBFOLDER(LibPostgreSQLInterface "DependentExtensions")
ENDIF(NOT WIN32 AND UNIX)

