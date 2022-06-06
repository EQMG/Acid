set(_component_name Network)
set(_component_library Acid_${_component_name})
set(_component_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/Ftp.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpDataChannel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponse.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponseDirectory.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponseListing.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/Http.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/HttpRequest.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/HttpResponse.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/IpAddress.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Packet.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Socket.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketSelector.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Tcp/TcpListener.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Tcp/TcpSocket.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Udp/UdpSocket.hpp
		)
set(_component_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/Ftp.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpDataChannel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponse.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponseDirectory.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponseListing.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/Http.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/HttpRequest.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/HttpResponse.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/IpAddress.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Packet.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Socket.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketSelector.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Tcp/TcpListener.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Tcp/TcpSocket.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Udp/UdpSocket.cpp
		)
set(_component_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp
		)

add_library(${_component_library}
        ${_component_headers}
        ${_component_sources}
		${_component_generated_headers}
        )
add_library(Acid::${_component_name} ALIAS ${_component_library})

include(GenerateExportHeader)
generate_export_header(${_component_library} EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp")

target_compile_features(${_component_library} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid::Core
		)
set_target_properties(${_component_library}
		PROPERTIES
		FOLDER "Acid"
		EXPORT_NAME "${_component_name}"
		)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_component_generated_headers})

install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_SOURCE_DIR}" "${_component_headers}")
install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_component_generated_headers}")

set(ACID_LIBRARIES "${ACID_LIBRARIES};${_component_library}" PARENT_SCOPE)
