if(NOT KERNEL_DIR)
	set(KERNEL_DIR /usr/src/linux-headers-5.4.0-42-generic)	
endif()


function(compile_module obj)
	set(TARGET_NAME ${obj})
	add_custom_target(${TARGET_NAME} ALL cp -f ${CMAKE_CURRENT_SOURCE_DIR}/*.c  ${CMAKE_CURRENT_BINARY_DIR}/
						COMMAND cp -f ${CMAKE_CURRENT_SOURCE_DIR}/*.h  ${CMAKE_CURRENT_BINARY_DIR}/
				    	)

	list(LENGTH ARGN argn_len)
	set(i 0)
	set(depend_objlist "")
	while( i LESS ${argn_len})  
		list(GET ARGN ${i} argn_value)  
		set(depend_objlist "${depend_objlist} ${argn_value}.o") 
		math(EXPR i "${i} + 1")  
	endwhile() 
  
  	add_custom_command(TARGET ${TARGET_NAME}
						POST_BUILD
						COMMAND echo "CONFIG_MODULE_SIG=y" > ${CMAKE_CURRENT_BINARY_DIR}/Makefile
						COMMAND echo "CONFIG_MODULE_SIG_FORCE=y" > ${CMAKE_CURRENT_BINARY_DIR}/Makefile
						COMMAND echo "CONFIG_MODULE_SIG_ALL=y" > ${CMAKE_CURRENT_BINARY_DIR}/Makefile
						COMMAND echo "ccflags-y += -g" > ${CMAKE_CURRENT_BINARY_DIR}/Makefile
						COMMAND echo "CC += -g" > ${CMAKE_CURRENT_BINARY_DIR}/Makefile
						COMMAND echo "obj-m := ${obj}.o" >> ${CMAKE_CURRENT_BINARY_DIR}/Makefile
						COMMAND echo "${obj}-objs := ${depend_objlist}" >>${CMAKE_CURRENT_BINARY_DIR}/Makefile
						COMMAND make -C ${KERNEL_DIR} M=${CMAKE_CURRENT_BINARY_DIR} modules EXTRA_CFLAGS="-g"
						)
endfunction()

compile_module(EndpointUSBDriver main cobjc cmap)
