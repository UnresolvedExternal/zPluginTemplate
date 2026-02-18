add_subdirectory(union)
add_subdirectory(Service)

target_link_libraries(${CMAKE_PROJECT_NAME}
	PRIVATE
		union
		Service
)