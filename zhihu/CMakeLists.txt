PROJECT(test)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
if(COMMAND camke_policy)
	camke_policy(SET CMP0003 NEW)
endif(COMMAND camke_policy)

FIND_PACKAGE( OpenCV REQUIRED)

#Declare teh target (an executable)
ADD_EXECUTABLE(zhihu zhihu.cpp)
TARGET_LINK_LIBRARIES(zhihu ${OpenCV_LIBS})
#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
