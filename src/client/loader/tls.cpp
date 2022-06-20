#include <std_include.hpp>
#include "tls.hpp"

#include <utils/nt.hpp>
#include <utils/binary_resource.hpp>

namespace tls
{
	namespace
	{
		utils::binary_resource tls_dll_file(TLS_DLL, "h1-tlsdll.dll");
	}

	PIMAGE_TLS_DIRECTORY allocate_tls_index()
	{
		static auto already_allocated = false;
		if (already_allocated)
		{
			throw std::runtime_error("Currently only a single allocation is supported!");
		}

		already_allocated = true;

		const auto dll_path = tls_dll_file.get_extracted_file();
		const auto tls_dll = utils::nt::library::load(dll_path);
		if (!tls_dll)
		{
			throw std::runtime_error("Failed to load TLS DLL");
		}

		return reinterpret_cast<PIMAGE_TLS_DIRECTORY>(tls_dll.get_ptr() + tls_dll.get_optional_header()
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
	}
}
