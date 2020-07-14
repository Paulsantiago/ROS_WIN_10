set(FIND_MCCL_PATHS
 ${CMAKE_CURRENT_SOURCE_DIR})
 
find_path(MCCL_FUNC_INCLUDE_DIR MCCL_RTX.h
		PATH_SUFFIXES include 
		PATHS ${FIND_MCCL_PATHS})

find_path(MCCL_INCLUDE_DIR MCCL.h
		PATH_SUFFIXES include 
		PATHS ${FIND_MCCL_PATHS})
		
		#message(${FIND_MCCL_PATHS})
		#message("-----------------------")
#find_library(wdapi1230_LIBRARY             NAMES wdapi1230.dll            PATH_SUFFIXES lib            PATHS ${FIND_BASICSTATICLIB_PATHS}            )    
find_library(MCCL_LIBRARY_lib
             NAMES MCCL_Client
			 PATH_SUFFIXES LIBX64
			 PATHS ${FIND_MCCL_PATHS}
					) 
 

		