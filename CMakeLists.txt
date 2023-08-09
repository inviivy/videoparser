cmake_minimum_required(VERSION 3.0)

project(ProjectName)

set(CMAKE_CXX_STANDARD 20)

set(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/media/src/h264_bit_reader.cc
	${CMAKE_CURRENT_SOURCE_DIR}/media/src/h264_parser.cc
	${CMAKE_CURRENT_SOURCE_DIR}/media/src/h265_nalu_parser.cc
	${CMAKE_CURRENT_SOURCE_DIR}/media/src/h265_parser.cc
)

add_library(h265parser STATIC ${SRCS})

target_include_directories(h265parser PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/base
	${CMAKE_CURRENT_SOURCE_DIR}/base/numeric
	${CMAKE_CURRENT_SOURCE_DIR}/logging
	${CMAKE_CURRENT_SOURCE_DIR}/media
	${CMAKE_CURRENT_SOURCE_DIR}/media/include
)
